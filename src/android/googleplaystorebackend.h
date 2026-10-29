#ifndef GOOGLEPLAYSTOREBACKEND_H
#define GOOGLEPLAYSTOREBACKEND_H

#include <QObject>
#include <QCoreApplication>
#include <QJniEnvironment>
#include <QJniObject>
#include <qt6purchasing/abstractstorebackend.h>

class GooglePlayStoreBackend : public AbstractStoreBackend
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Store)

public:
    enum BillingResponseCode {
        OK,
        USER_CANCELED,
        SERVICE_UNAVAILABLE,
        BILLING_UNAVAILABLE,
        ITEM_UNAVAILABLE,
        DEVELOPER_ERROR,
        ERROR,
        ITEM_ALREADY_OWNED,
        ITEM_NOT_OWNED,
        FEATURE_NOT_SUPPORTED = -2,
        SERVICE_DISCONNECTED = -1,
        SERVICE_TIMEOUT = -3,
    };
    Q_ENUM(BillingResponseCode)

    explicit GooglePlayStoreBackend(QObject * parent = nullptr);
    ~GooglePlayStoreBackend();

    static void debugMessageJniCallback(JNIEnv * env, jobject object, jstring message);
    static void billingResponseReceivedJniCallback(JNIEnv * env, jobject object, jint billingResponseCode);
    static void connectedChangedHelperJniCallback(JNIEnv * env, jobject object, jboolean connected);
    static void productRegisteredJniCallback(JNIEnv * env, jobject object, jstring productJson);
    static void
    productRegistrationFailedJniCallback(JNIEnv * env, jobject object, jstring productId, jint billingResponseCode);
    static void purchaseSucceededJniCallback(JNIEnv * env, jobject object, jstring purchaseJson);
    static void purchasePendingJniCallback(JNIEnv * env, jobject object, jstring purchaseJson);
    static void purchaseRestoredJniCallback(JNIEnv * env, jobject object, jstring purchaseJson);
    static void purchaseFailedJniCallback(JNIEnv * env, jobject object, jstring productId, jint billingResponseCode);
    static void purchaseConsumedJniCallback(JNIEnv * env, jobject object, jstring purchaseJson);
    static void restorePurchasesSucceededJniCallback(JNIEnv * env, jobject object, jint count);
    static void restorePurchasesFailedJniCallback(JNIEnv * env, jobject object, jint billingResponseCode);

    void startConnection() override;
    void registerProduct(AbstractProduct * product) override;
    void purchaseProduct(AbstractProduct * product) override;
    void consumePurchase(Transaction transaction) override;
    bool canMakePurchases() const override;

    // Transaction processing control
    void enableProcessing() override;

protected:
    void restorePurchasesImpl() override;

private:
    void processQueuedTransactions();
    static PurchaseError mapBillingResponseToPurchaseError(int billingResponseCode);
    static QString getBillingResponseMessage(int billingResponseCode);

    // Queued transaction data
    QList<QJsonObject> _queuedPurchaseSucceeded;
    QList<QJsonObject> _queuedPurchaseRestored;
    QList<QJsonObject> _queuedPurchasePending;

    static GooglePlayStoreBackend * s_currentInstance;
    QJniObject * _googlePlayBillingJavaClass = nullptr;
};

#endif // GOOGLEPLAYSTOREBACKEND_H
