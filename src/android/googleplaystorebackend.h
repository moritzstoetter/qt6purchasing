#ifndef GOOGLEPLAYSTOREBACKEND_H
#define GOOGLEPLAYSTOREBACKEND_H

#include <QObject>
#include <QCoreApplication>
#include <QJniEnvironment>
#include <QJniObject>
#include "backend/store/abstractstorebackend.h"

class GooglePlayStoreBackend : public AbstractStoreBackend
{
    Q_OBJECT

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

    static void debugMessage(JNIEnv * env, jobject object, jstring message);
    static void billingResponseReceived(JNIEnv * env, jobject object, jint billingResponseCode);
    static void connectedChangedHelper(JNIEnv * env, jobject object, jboolean connected);
    static void productRegistered(JNIEnv * env, jobject object, jstring productJson);
    static void purchaseSucceeded(JNIEnv * env, jobject object, jstring purchaseJson);
    static void purchaseRestored(JNIEnv * env, jobject object, jstring purchaseJson);
    static void purchaseFailed(JNIEnv * env, jobject object, jint billingResponseCode);
    static void purchaseConsumed(JNIEnv * env, jobject object, jstring purchaseJson);

    void startConnection() override;
    void registerProduct(AbstractProduct * product) override;
    void purchaseProduct(AbstractProduct * product) override;
    void consumePurchase(AbstractTransaction * transaction) override;

private:
    QJniObject * _googlePlayBillingJavaClass = nullptr;
};

#endif // GOOGLEPLAYSTOREBACKEND_H
