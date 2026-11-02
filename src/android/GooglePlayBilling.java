package io.github.moritzstoetter.qt6purchasing;

import android.app.Activity;
import android.content.Context;

import com.android.billingclient.api.*;
import com.android.billingclient.api.BillingClient.BillingResponseCode;
import com.android.billingclient.api.SkuDetailsParams.*;
import com.android.billingclient.api.BillingClient.SkuType;

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
                            purchasePending(purchase.getOriginalJson());
                        } else if (purchase.getPurchaseState() == Purchase.PurchaseState.PURCHASED) {
                            // Purchase completed successfully
                            purchaseSucceeded(purchase.getOriginalJson());
                        } else {
                            // Unhandled purchase state - treat as unknown error
                            debugMessage("Unhandled purchase state: " + purchase.getPurchaseState());
                            // Product id is available directly from the purchase
                            List<String> skus = purchase.getSkus();
                            String productId = !skus.isEmpty() ? skus.get(0) : "unknown";
                            purchaseFailed(productId, BillingResponseCode.ERROR);
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
                            purchaseRestored(purchase.getOriginalJson());
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
                .enablePendingPurchases()
                .build();
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
                    billingClient.queryPurchasesAsync(SkuType.INAPP, purchasesResponseListener);

                } else {
                    connectedChangedHelper(false);
                }
            }
            @Override
            public void onBillingServiceDisconnected() {
                // Try to restart the connection on the next request to
                // Google Play by calling the startConnection() method.
                connectedChangedHelper(false);
            }
        });
    };

    public void registerProduct(final String productId) {
        List<String> skuList = new ArrayList<> ();
        skuList.add(productId);
        SkuDetailsParams.Builder params = SkuDetailsParams.newBuilder();
        params.setSkusList(skuList).setType(SkuType.INAPP);
        billingClient.querySkuDetailsAsync(params.build(),
            new SkuDetailsResponseListener() {
                @Override
                public void onSkuDetailsResponse(BillingResult billingResult,
                        List<SkuDetails> skuDetailsList) {
                    billingResponseReceived(billingResult.getResponseCode());

                    if (billingResult.getResponseCode() != BillingResponseCode.OK) {
                        productRegistrationFailed(productId, billingResult.getResponseCode());
                        return;
                    }

                    if (skuDetailsList == null || skuDetailsList.isEmpty()) {
                        productRegistrationFailed(productId, BillingResponseCode.OK);
                        return;
                    }

                    productRegistered(skuDetailsList.get(0).getOriginalJson());
                }
            });
    };

    public void purchaseProduct(Activity activity, final String jsonSkuDetails) {
        try {
            // Extract and store the product ID before launching billing flow
            JSONObject obj = new JSONObject(jsonSkuDetails);
            pendingProductId = obj.getString("productId");

            SkuDetails purchaseThis = new SkuDetails(jsonSkuDetails);

            BillingFlowParams billingFlowParams = BillingFlowParams.newBuilder()
                .setSkuDetails(purchaseThis)
                .build();
            int responseCode = billingClient.launchBillingFlow(activity, billingFlowParams).getResponseCode();
        } catch (JSONException e) {
            pendingProductId = null;  // Clean up on error
            throw new RuntimeException(e);
        }
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

    public void queryExistingPurchases() {
        debugMessage("Manual restore purchases triggered - calling queryPurchasesAsync");
        billingClient.queryPurchasesAsync(SkuType.INAPP, purchasesResponseListener);
    }
}
