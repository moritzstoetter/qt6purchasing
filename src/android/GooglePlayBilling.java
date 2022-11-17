package com.climbsmart.crimptronic;

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

    private static native void purchaseSucceeded(String purchaseJson);
    private static native void purchaseRestored(String purchaseJson);
    private static native void purchaseFailed(int billingResponseCode);
    private static native void purchaseConsumed(String purchaseJson);

    private Context context;
    private PurchasesUpdatedListener purchasesUpdatedListener;
    private PurchasesResponseListener purchasesResponseListener;
    private BillingClient billingClient;

    public GooglePlayBilling(Context cnt) {
        System.out.println("GooglePlayBilling constructor called with context argument " + cnt);
        this.context = cnt;

        purchasesUpdatedListener = new PurchasesUpdatedListener() {
            public void onPurchasesUpdated(BillingResult billingResult, List<Purchase> purchases) {
                if (billingResult.getResponseCode() == BillingResponseCode.OK && purchases != null) {
                    for (Purchase purchase : purchases) {
                        purchaseSucceeded(purchase.getOriginalJson());
                    }
                } else if (billingResult.getResponseCode() == BillingResponseCode.USER_CANCELED) {
                    // Handle an error caused by a user cancelling the purchase flow.
                    purchaseFailed(billingResult.getResponseCode());
                } else {
                    // Handle any other error codes.
                    purchaseFailed(billingResult.getResponseCode());
                }
            }
        };

        purchasesResponseListener = new PurchasesResponseListener() {
            @Override
            public void onQueryPurchasesResponse(BillingResult billingResult, List<Purchase> purchases) {
                if (!purchases.isEmpty()) {
                    for (Purchase purchase : purchases) {
                        purchaseRestored(purchase.getOriginalJson());
                    }
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
                    if (skuDetailsList == null)
//                        throw new NullPointerException("skuDetailsList is null");
                        return;

                    billingResponseReceived(billingResult.getResponseCode());
                    productRegistered(skuDetailsList.get(0).getOriginalJson());
                }
            });
    };

    public void purchaseProduct(Activity activity, final String jsonSkuDetails) {
        try {
            SkuDetails purchaseThis = new SkuDetails(jsonSkuDetails);

            BillingFlowParams billingFlowParams = BillingFlowParams.newBuilder()
                .setSkuDetails(purchaseThis)
                .build();
            int responseCode = billingClient.launchBillingFlow(activity, billingFlowParams).getResponseCode();
        } catch (JSONException e) {
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
}
