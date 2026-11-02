package io.github.moritzstoetter.qt6purchasing;

import android.app.Activity;
import android.content.Context;

import com.android.billingclient.api.*;
import com.android.billingclient.api.BillingClient.BillingResponseCode;
import com.android.billingclient.api.BillingClient.ProductType;

import java.util.ArrayList;
import java.util.List;

import org.json.*;

public class GooglePlayBilling {

    private static native void debugMessage(String message);
    private static native void connectedChangedHelper(boolean connected);
    private static native void billingResponseReceived(int billingResponseCode);
    private static native void productRegistered(String productJson);
    private static native void productRegistrationFailed(String productId, int billingResponseCode);

    private static native void purchaseSucceeded(String purchaseJson);
    private static native void purchasePending(String purchaseJson);
    private static native void purchaseRestored(String purchaseJson);
    private static native void purchaseFailed(String productId, int billingResponseCode);
    private static native void purchaseConsumed(String purchaseJson);
    private static native void purchaseAcknowledged(String purchaseJson);
    private static native void purchaseFinalizeFailed(String purchaseJson, int billingResponseCode);
    private static native void restorePurchasesSucceeded(int count);
    private static native void restorePurchasesFailed(int billingResponseCode);

    private Context context;
    private PurchasesUpdatedListener purchasesUpdatedListener;
    private PurchasesResponseListener purchasesResponseListener;
    private BillingClient billingClient;
    private String pendingProductId = null;

    public GooglePlayBilling(Context cnt) {
        System.out.println("GooglePlayBilling constructor called with context argument " + cnt);
        this.context = cnt;

        purchasesUpdatedListener = new PurchasesUpdatedListener() {
            public void onPurchasesUpdated(BillingResult billingResult, List<Purchase> purchases) {
                if (billingResult.getResponseCode() == BillingResponseCode.OK && purchases != null) {
                    // Success - clear pending and check purchase states
                    pendingProductId = null;
                    for (Purchase purchase : purchases) {
                        if (purchase.getPurchaseState() == Purchase.PurchaseState.PENDING) {
                            // Purchase is awaiting approval (e.g., parental approval, payment method verification)
                            purchasePending(purchaseJson(purchase));
                        } else if (purchase.getPurchaseState() == Purchase.PurchaseState.PURCHASED) {
                            // Purchase completed successfully
                            purchaseSucceeded(purchaseJson(purchase));
                        } else {
                            // Unhandled purchase state - treat as unknown error
                            debugMessage("Unhandled purchase state: " + purchase.getPurchaseState());
                            purchaseFailed(productIdOf(purchase), BillingResponseCode.ERROR);
                        }
                    }
                } else {
                    // No purchase object on failure; use the id saved when the purchase began
                    String productId = pendingProductId != null ? pendingProductId : "unknown";
                    pendingProductId = null;  // Clear after use
                    purchaseFailed(productId, billingResult.getResponseCode());
                }
            }
        };

        purchasesResponseListener = new PurchasesResponseListener() {
            @Override
            public void onQueryPurchasesResponse(BillingResult billingResult, List<Purchase> purchases) {
                if (billingResult.getResponseCode() == BillingResponseCode.OK) {
                    int count = 0;
                    if (purchases != null && !purchases.isEmpty()) {
                        for (Purchase purchase : purchases) {
                            purchaseRestored(purchaseJson(purchase));
                            count++;
                        }
                    }
                    restorePurchasesSucceeded(count);
                } else {
                    debugMessage("Restore purchases failed with response code: " + billingResult.getResponseCode());
                    restorePurchasesFailed(billingResult.getResponseCode());
                }
            }
        };

        billingClient = BillingClient.newBuilder(this.context)
                .setListener(purchasesUpdatedListener)
                .enablePendingPurchases(PendingPurchasesParams.newBuilder().enableOneTimeProducts().build())
                .enableAutoServiceReconnection()
                .build();
    }

    private static String productIdOf(Purchase purchase) {
        List<String> products = purchase.getProducts();
        return !products.isEmpty() ? products.get(0) : "unknown";
    }

    private static String purchaseJson(Purchase purchase) {
        JSONObject json = new JSONObject();
        try {
            json.put("orderId", purchase.getOrderId());
            json.put("productId", productIdOf(purchase));
            json.put("purchaseToken", purchase.getPurchaseToken());
        } catch (JSONException e) {
            debugMessage("Failed to build purchase JSON: " + e);
        }
        return json.toString();
    }

    private static String productJson(ProductDetails details) {
        ProductDetails.OneTimePurchaseOfferDetails offer = details.getOneTimePurchaseOfferDetails();
        JSONObject json = new JSONObject();
        try {
            json.put("productId", details.getProductId());
            json.put("title", details.getTitle());
            json.put("description", details.getDescription());
            json.put("price", offer != null ? offer.getFormattedPrice() : "");
        } catch (JSONException e) {
            debugMessage("Failed to build product JSON: " + e);
        }
        return json.toString();
    }

    private void queryProductDetails(String productId, ProductDetailsResponseListener listener) {
        List<QueryProductDetailsParams.Product> productList = new ArrayList<>();
        productList.add(QueryProductDetailsParams.Product.newBuilder()
            .setProductId(productId)
            .setProductType(ProductType.INAPP)
            .build());
        QueryProductDetailsParams params = QueryProductDetailsParams.newBuilder()
            .setProductList(productList)
            .build();
        billingClient.queryProductDetailsAsync(params, listener);
    }

    private void queryPurchases() {
        QueryPurchasesParams params = QueryPurchasesParams.newBuilder()
            .setProductType(ProductType.INAPP)
            .build();
        billingClient.queryPurchasesAsync(params, purchasesResponseListener);
    }


    public void startConnection() {
        debugMessage("Starting connection to Google Play Store");

        billingClient.startConnection(new BillingClientStateListener() {
            @Override
            public void onBillingSetupFinished(BillingResult billingResult) {
                billingResponseReceived(billingResult.getResponseCode());
                if (billingResult.getResponseCode() ==  BillingResponseCode.OK) {
                    connectedChangedHelper(true);
                    // The BillingClient is ready. query purchases here.
                    queryPurchases();

                } else {
                    connectedChangedHelper(false);
                }
            }
            @Override
            public void onBillingServiceDisconnected() {
                debugMessage("Billing service disconnected; it reconnects on the next request");
            }
        });
    };

    public void registerProduct(final String productId) {
        queryProductDetails(productId,
            new ProductDetailsResponseListener() {
                @Override
                public void onProductDetailsResponse(BillingResult billingResult,
                        QueryProductDetailsResult result) {
                    billingResponseReceived(billingResult.getResponseCode());

                    if (billingResult.getResponseCode() != BillingResponseCode.OK) {
                        productRegistrationFailed(productId, billingResult.getResponseCode());
                        return;
                    }

                    for (UnfetchedProduct unfetched : result.getUnfetchedProductList()) {
                        debugMessage("Product " + unfetched.getProductId() + " not fetched, status code: "
                                + unfetched.getStatusCode());
                        productRegistrationFailed(unfetched.getProductId(), BillingResponseCode.ITEM_UNAVAILABLE);
                    }

                    for (ProductDetails details : result.getProductDetailsList()) {
                        productRegistered(productJson(details));
                    }
                }
            });
    };

    public void purchaseProduct(final Activity activity, final String productId) {
        queryProductDetails(productId,
            new ProductDetailsResponseListener() {
                @Override
                public void onProductDetailsResponse(BillingResult billingResult,
                        QueryProductDetailsResult result) {
                    if (billingResult.getResponseCode() != BillingResponseCode.OK) {
                        purchaseFailed(productId, billingResult.getResponseCode());
                        return;
                    }

                    if (result.getProductDetailsList().isEmpty()) {
                        purchaseFailed(productId, BillingResponseCode.ITEM_UNAVAILABLE);
                        return;
                    }

                    ProductDetails details = result.getProductDetailsList().get(0);
                    BillingFlowParams.ProductDetailsParams.Builder productParams =
                        BillingFlowParams.ProductDetailsParams.newBuilder().setProductDetails(details);
                    ProductDetails.OneTimePurchaseOfferDetails offer = details.getOneTimePurchaseOfferDetails();
                    if (offer != null)
                        productParams.setOfferToken(offer.getOfferToken());

                    List<BillingFlowParams.ProductDetailsParams> productParamsList = new ArrayList<>();
                    productParamsList.add(productParams.build());
                    BillingFlowParams billingFlowParams = BillingFlowParams.newBuilder()
                        .setProductDetailsParamsList(productParamsList)
                        .build();

                    activity.runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            pendingProductId = productId;
                            // A failed launch is also delivered to onPurchasesUpdated(), which reports it
                            billingClient.launchBillingFlow(activity, billingFlowParams);
                        }
                    });
                }
            });
    };

    public void consumePurchase(final String jsonPurchaseString) {
        ConsumeResponseListener listener = new ConsumeResponseListener() {
            @Override
            public void onConsumeResponse (BillingResult billingResult, String purchaseToken) {
                if (billingResult.getResponseCode() == BillingResponseCode.OK) {
                    purchaseConsumed(jsonPurchaseString);
                }
            }
        };

        try {
            JSONObject obj = new JSONObject(jsonPurchaseString);
            String purchaseToken = obj.getString("purchaseToken");

            ConsumeParams consumeParams = ConsumeParams.newBuilder()
                .setPurchaseToken(purchaseToken)
                .build();

            billingClient.consumeAsync(consumeParams, listener);

        } catch (JSONException e) {
            throw new RuntimeException(e);
        }
    }

    public void acknowledgePurchase(final String jsonPurchaseString) {
        final String purchaseToken;
        try {
            purchaseToken = new JSONObject(jsonPurchaseString).getString("purchaseToken");
        } catch (JSONException e) {
            purchaseFinalizeFailed(jsonPurchaseString, BillingResponseCode.DEVELOPER_ERROR);
            return;
        }

        QueryPurchasesParams params = QueryPurchasesParams.newBuilder()
            .setProductType(ProductType.INAPP)
            .build();
        billingClient.queryPurchasesAsync(params, new PurchasesResponseListener() {
            @Override
            public void onQueryPurchasesResponse(BillingResult billingResult, List<Purchase> purchases) {
                if (billingResult.getResponseCode() == BillingResponseCode.OK && purchases != null) {
                    for (Purchase purchase : purchases) {
                        if (purchase.getPurchaseToken().equals(purchaseToken) && purchase.isAcknowledged()) {
                            purchaseAcknowledged(jsonPurchaseString);
                            return;
                        }
                    }
                }
                sendAcknowledgement(jsonPurchaseString, purchaseToken);
            }
        });
    }

    private void sendAcknowledgement(final String jsonPurchaseString, String purchaseToken) {
        AcknowledgePurchaseParams params = AcknowledgePurchaseParams.newBuilder()
            .setPurchaseToken(purchaseToken)
            .build();
        billingClient.acknowledgePurchase(params, new AcknowledgePurchaseResponseListener() {
            @Override
            public void onAcknowledgePurchaseResponse(BillingResult billingResult) {
                if (billingResult.getResponseCode() == BillingResponseCode.OK)
                    purchaseAcknowledged(jsonPurchaseString);
                else
                    purchaseFinalizeFailed(jsonPurchaseString, billingResult.getResponseCode());
            }
        });
    }

    public void queryExistingPurchases() {
        debugMessage("Manual restore purchases triggered - calling queryPurchasesAsync");
        queryPurchases();
    }

    public void endConnection() {
        billingClient.endConnection();
    }
}
