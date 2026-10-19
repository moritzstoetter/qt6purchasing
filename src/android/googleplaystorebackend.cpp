#include "googleplaystorebackend.h"
#include "googleplaystoreproduct.h"

#include <QJniEnvironment>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>
#include <QCoreApplication>

// Helper functions for GooglePlayStoreTransaction <-> JSON conversion
static Transaction transactionFromJson(const QJsonObject &json)
{
    Transaction transaction;
    transaction.orderId = json.value("orderId").toString();
    transaction.productId = json.value("productId").toString();
    transaction.purchaseToken = json.value("purchaseToken").toString();
    return transaction;
}

static QJsonObject transactionToJson(Transaction transaction)
{
    QJsonObject json;
    json["orderId"] = transaction.orderId;
    json["productId"] = transaction.productId;
    json["purchaseToken"] = transaction.purchaseToken;
    return json;
}

GooglePlayStoreBackend * GooglePlayStoreBackend::s_currentInstance = nullptr;

GooglePlayStoreBackend::GooglePlayStoreBackend(QObject * parent) : AbstractStoreBackend(parent)
{
    _googlePlayBillingJavaClass = new QJniObject(
        "com/climbsmart/crimptronic/GooglePlayBilling",
        "(Landroid/content/Context;)V",
        QNativeInterface::QAndroidApplication::context().object()
    );

    JNINativeMethod methods[] {
        {"debugMessage", "(Ljava/lang/String;)V", reinterpret_cast<void *>(debugMessageJniCallback)},
        {"billingResponseReceived", "(I)V", reinterpret_cast<void *>(billingResponseReceivedJniCallback)},
        {"connectedChangedHelper", "(Z)V", reinterpret_cast<void *>(connectedChangedHelperJniCallback)},
        {"productRegistered", "(Ljava/lang/String;)V", reinterpret_cast<void *>(productRegisteredJniCallback)},
        {"productRegistrationFailed", "(Ljava/lang/String;I)V", reinterpret_cast<void *>(productRegistrationFailedJniCallback)},
        {"purchaseSucceeded", "(Ljava/lang/String;)V", reinterpret_cast<void *>(purchaseSucceededJniCallback)},
        {"purchasePending", "(Ljava/lang/String;)V", reinterpret_cast<void *>(purchasePendingJniCallback)},
        {"purchaseRestored", "(Ljava/lang/String;)V", reinterpret_cast<void *>(purchaseRestoredJniCallback)},
        {"purchaseFailed", "(Ljava/lang/String;I)V", reinterpret_cast<void *>(purchaseFailedJniCallback)},
        {"purchaseConsumed", "(Ljava/lang/String;)V", reinterpret_cast<void *>(purchaseConsumedJniCallback)},
        {"restorePurchasesSucceeded", "(I)V", reinterpret_cast<void *>(restorePurchasesSucceededJniCallback)},
        {"restorePurchasesFailed", "(I)V", reinterpret_cast<void *>(restorePurchasesFailedJniCallback)},
    };
    QJniEnvironment env;
    jclass objectClass = env->GetObjectClass(_googlePlayBillingJavaClass->object<jobject>());
    env->RegisterNatives(objectClass, methods, sizeof(methods) / sizeof(methods[0]));
    env->DeleteLocalRef(objectClass);

    this->startConnection();

    Q_ASSERT(QThread::currentThread() == QCoreApplication::instance()->thread());
    s_currentInstance = this;
}

GooglePlayStoreBackend::~GooglePlayStoreBackend()
{
    if (s_currentInstance == this)
        s_currentInstance = nullptr;

    delete _googlePlayBillingJavaClass;
}

/*static*/ void GooglePlayStoreBackend::debugMessageJniCallback(JNIEnv * env, jobject object, jstring message)
{
    const char * messageCStr = env->GetStringUTFChars(message, nullptr);
    qDebug() << messageCStr;
    env->ReleaseStringUTFChars(message, messageCStr);
}

/*static*/ void GooglePlayStoreBackend::billingResponseReceivedJniCallback(JNIEnv * env, jobject object, jint value)
{
    qDebug() << "Billing response received:" << static_cast<GooglePlayStoreBackend::BillingResponseCode>(value);
}

/*static*/ void GooglePlayStoreBackend::connectedChangedHelperJniCallback(JNIEnv * env, jobject object, jboolean connected)
{
    GooglePlayStoreBackend * backend = GooglePlayStoreBackend::s_currentInstance;
    if (!backend) {
        qCritical() << "Google Play billing callback received but backend instance is null";
        return;
    }
    backend->setConnected(connected);
    backend->setCanMakePurchases(backend->canMakePurchases());
}

void GooglePlayStoreBackend::startConnection()
{
    _googlePlayBillingJavaClass->callMethod<void>("startConnection");
}

void GooglePlayStoreBackend::registerProduct(AbstractProduct * product)
{
    _googlePlayBillingJavaClass->callMethod<void>(
        "registerProduct", "(Ljava/lang/String;)V", QJniObject::fromString(product->identifier()).object<jstring>()
    );
}

/*static*/ void GooglePlayStoreBackend::productRegisteredJniCallback(JNIEnv * env, jobject object, jstring message)
{
    GooglePlayStoreBackend * backend = GooglePlayStoreBackend::s_currentInstance;
    if (!backend) {
        qCritical() << "Google Play product registration callback received but backend instance is null";
        return;
    }

    const char * jsonCStr = env->GetStringUTFChars(message, nullptr);
    QJsonObject json = QJsonDocument::fromJson(jsonCStr).object();
    env->ReleaseStringUTFChars(message, jsonCStr);

    GooglePlayStoreProduct * product =
        reinterpret_cast<GooglePlayStoreProduct *>(backend->product(json["productId"].toString()));

    if (product) {
        product->setJson(json);
        product->setDescription(json["description"].toString());
        product->setPrice(json["price"].toString());
        product->setTitle(json["title"].toString());
        product->setStatus(AbstractProduct::Registered);

        emit backend->productRegistered(product);
    } else {
        qCritical() << "Registered a product that's not in the list of products. This is not handled.";
    }
}

/*static*/ void GooglePlayStoreBackend::productRegistrationFailedJniCallback(
    JNIEnv * env, jobject object, jstring productId, jint billingResponseCode
)
{
    GooglePlayStoreBackend * backend = GooglePlayStoreBackend::s_currentInstance;
    if (!backend) {
        qCritical() << "Google Play product registration failed callback received but backend instance is null";
        return;
    }

    const char * productIdCStr = env->GetStringUTFChars(productId, nullptr);
    QString prodId = QString::fromUtf8(productIdCStr);
    env->ReleaseStringUTFChars(productId, productIdCStr);

    qWarning() << "Product registration failed for" << prodId << "with billing response code:" << billingResponseCode;

    AbstractProduct * product = backend->product(prodId);
    if (product)
        product->setStatus(AbstractProduct::Unknown);
    else
        qWarning() << "Could not find product to update status:" << prodId;
}

void GooglePlayStoreBackend::purchaseProduct(AbstractProduct * product)
{
    QByteArray jsonSkuDetails = QJsonDocument(reinterpret_cast<GooglePlayStoreProduct *>(product)->json()).toJson();

    _googlePlayBillingJavaClass->callMethod<void>(
        "purchaseProduct",
        "(Landroid/app/Activity;Ljava/lang/String;)V",
        QNativeInterface::QAndroidApplication::context(),
        QJniObject::fromString(jsonSkuDetails).object<jstring>()
    );
}

void GooglePlayStoreBackend::consumePurchase(Transaction transaction)
{
    qDebug() << "Android consumePurchase called for:" << transaction.orderId
             << "purchaseToken:" << transaction.purchaseToken;

    // Only call consumeAsync for Consumable products
    AbstractProduct * product = this->product(transaction.productId);
    if (!product) {
        qWarning() << "Cannot find product for transaction:" << transaction.productId;
        emit consumePurchaseFailed(transaction);
        return;
    }

    // Only consumables need fulfillment
    if (product->productType() != AbstractProduct::Consumable) {
        qDebug() << "Product is not consumable (type:" << product->productType() << "), no fulfillment needed";
        emit consumePurchaseSucceeded(transaction);
        return;
    }

    // For consumables, we need to report fulfillment to Google Play Store
    qDebug() << "Android: consumePurchase called for" << transaction.orderId;

    _googlePlayBillingJavaClass->callMethod<void>(
        "consumePurchase",
        "(Ljava/lang/String;)V",
        QJniObject::fromString(QJsonDocument(transactionToJson(transaction)).toJson()).object<jstring>()
    );
}

void GooglePlayStoreBackend::restorePurchasesImpl()
{
    qDebug() << "Android restorePurchasesImpl() called - triggering manual queryPurchasesAsync";
    _googlePlayBillingJavaClass->callMethod<void>("queryExistingPurchases");
}

bool GooglePlayStoreBackend::canMakePurchases() const
{
    // For Android, we can make purchases if we're connected to the billing service
    return isConnected();
}

/*static*/ void GooglePlayStoreBackend::purchaseSucceededJniCallback(JNIEnv * env, jobject object, jstring message)
{
    GooglePlayStoreBackend * backend = GooglePlayStoreBackend::s_currentInstance;
    if (!backend) {
        qCritical() << "Google Play purchase callback received but backend instance is null";
        return;
    }

    const char * jsonCStr = env->GetStringUTFChars(message, nullptr);
    QJsonObject json = QJsonDocument::fromJson(jsonCStr).object();
    env->ReleaseStringUTFChars(message, jsonCStr);

    if (!backend->processingEnabled()) {
        qDebug() << "Android: purchaseSucceeded received but processing not enabled - queueing";
        backend->_queuedPurchaseSucceeded.append(json);
        return;
    }

    auto transaction = transactionFromJson(json);
    emit backend->purchaseSucceeded(transaction);
}

/*static*/ void GooglePlayStoreBackend::purchasePendingJniCallback(JNIEnv * env, jobject object, jstring message)
{
    GooglePlayStoreBackend * backend = GooglePlayStoreBackend::s_currentInstance;
    if (!backend) {
        qCritical() << "Google Play pending purchase callback received but backend instance is null";
        return;
    }

    const char * jsonCStr = env->GetStringUTFChars(message, nullptr);
    QJsonObject json = QJsonDocument::fromJson(jsonCStr).object();
    env->ReleaseStringUTFChars(message, jsonCStr);

    if (!backend->processingEnabled()) {
        qDebug() << "Android: purchasePending received but processing not enabled - queueing";
        backend->_queuedPurchasePending.append(json);
        return;
    }

    auto transaction = transactionFromJson(json);
    qDebug() << "Android purchase pending for product:" << transaction.productId;

    // Find the product and emit a pending signal
    AbstractProduct * product = backend->product(transaction.productId);
    if (product) {
        qDebug() << "Emitting purchase pending for product:" << transaction.productId;
        emit backend->purchasePending(transaction);
    } else {
        qWarning() << "Could not find product for pending purchase:" << transaction.productId;
    }
}

/*static*/ void GooglePlayStoreBackend::purchaseRestoredJniCallback(JNIEnv * env, jobject object, jstring message)
{
    GooglePlayStoreBackend * backend = GooglePlayStoreBackend::s_currentInstance;
    if (!backend) {
        qCritical() << "Google Play purchase callback received but backend instance is null";
        return;
    }

    const char * jsonCStr = env->GetStringUTFChars(message, nullptr);
    QJsonObject json = QJsonDocument::fromJson(jsonCStr).object();
    env->ReleaseStringUTFChars(message, jsonCStr);

    if (!backend->processingEnabled()) {
        qDebug() << "Android: purchaseRestored received but processing not enabled - queueing";
        backend->_queuedPurchaseRestored.append(json);
        return;
    }

    auto transaction = transactionFromJson(json);
    emit backend->purchaseRestored(transaction);
}

/*static*/ void GooglePlayStoreBackend::purchaseFailedJniCallback(JNIEnv * env, jobject object, jstring productId, jint billingResponseCode)
{
    GooglePlayStoreBackend * backend = GooglePlayStoreBackend::s_currentInstance;
    if (!backend) {
        qCritical() << "Google Play purchase callback received but backend instance is null";
        return;
    }

    const char * productIdCStr = env->GetStringUTFChars(productId, nullptr);
    QString prodId = QString::fromUtf8(productIdCStr);
    env->ReleaseStringUTFChars(productId, productIdCStr);

    PurchaseError error = mapBillingResponseToPurchaseError(billingResponseCode);
    QString message = getBillingResponseMessage(billingResponseCode);
    emit backend->purchaseFailed(prodId, static_cast<int>(error), billingResponseCode, message);
}

/*static*/ void GooglePlayStoreBackend::purchaseConsumedJniCallback(JNIEnv * env, jobject object, jstring message)
{
    GooglePlayStoreBackend * backend = GooglePlayStoreBackend::s_currentInstance;
    if (!backend) {
        qCritical() << "Google Play purchase callback received but backend instance is null";
        return;
    }

    const char * jsonCStr = env->GetStringUTFChars(message, nullptr);
    QJsonObject json = QJsonDocument::fromJson(jsonCStr).object();
    env->ReleaseStringUTFChars(message, jsonCStr);

    auto transaction = transactionFromJson(json);
    emit backend->consumePurchaseSucceeded(transaction);
}

/*static*/ void GooglePlayStoreBackend::restorePurchasesSucceededJniCallback(JNIEnv * env, jobject object, jint count)
{
    GooglePlayStoreBackend * backend = GooglePlayStoreBackend::s_currentInstance;
    if (!backend) {
        qWarning() << "Android: restorePurchasesSucceeded received but backend instance is null";
        return;
    }

    qDebug() << "Android: Restore purchases completed successfully. Count:" << count;
    emit backend->restorePurchasesSucceeded(count);
}

/*static*/ void GooglePlayStoreBackend::restorePurchasesFailedJniCallback(JNIEnv * env, jobject object, jint billingResponseCode)
{
    GooglePlayStoreBackend * backend = GooglePlayStoreBackend::s_currentInstance;
    if (!backend) {
        qWarning() << "Android: restorePurchasesFailed received but backend instance is null";
        return;
    }

    qDebug() << "Android: Restore purchases failed with billing response code:" << billingResponseCode;

    PurchaseError mappedError = mapBillingResponseToPurchaseError(billingResponseCode);
    QString message = getBillingResponseMessage(billingResponseCode);

    emit backend->restorePurchasesFailed(static_cast<int>(mappedError), billingResponseCode, message);
}

/*static*/ AbstractStoreBackend::PurchaseError
GooglePlayStoreBackend::mapBillingResponseToPurchaseError(int billingResponseCode)
{
    switch (static_cast<BillingResponseCode>(billingResponseCode)) {
    case USER_CANCELED:
        return PurchaseError::UserCanceled;
    case SERVICE_UNAVAILABLE:
        return PurchaseError::NetworkError;
    case BILLING_UNAVAILABLE:
        return PurchaseError::ServiceUnavailable;
    case ITEM_UNAVAILABLE:
        return PurchaseError::ItemUnavailable;
    case ITEM_NOT_OWNED:
        return PurchaseError::ItemNotOwned;
    case ITEM_ALREADY_OWNED:
        return PurchaseError::AlreadyPurchased;
    case DEVELOPER_ERROR:
        return PurchaseError::DeveloperError;
    case FEATURE_NOT_SUPPORTED:
        return PurchaseError::NotAllowed;
    case SERVICE_DISCONNECTED:
        return PurchaseError::ServiceUnavailable;
    case SERVICE_TIMEOUT:
        return PurchaseError::NetworkError;
    case ERROR:
    default:
        return PurchaseError::UnknownError;
    }
}

/*static*/ QString GooglePlayStoreBackend::getBillingResponseMessage(int billingResponseCode)
{
    switch (static_cast<BillingResponseCode>(billingResponseCode)) {
    case OK:
        return "Success";
    case USER_CANCELED:
        return "User canceled the purchase";
    case SERVICE_UNAVAILABLE:
        return "Google Play service is unavailable";
    case BILLING_UNAVAILABLE:
        return "Google Play billing is unavailable";
    case ITEM_UNAVAILABLE:
        return "The requested item is not available for purchase";
    case DEVELOPER_ERROR:
        return "Invalid arguments provided to the API";
    case ERROR:
        return "Fatal error during API action";
    case ITEM_ALREADY_OWNED:
        return "The user already owns this item";
    case ITEM_NOT_OWNED:
        return "The user does not own this item";
    case FEATURE_NOT_SUPPORTED:
        return "The requested feature is not supported";
    case SERVICE_DISCONNECTED:
        return "Google Play service is disconnected";
    case SERVICE_TIMEOUT:
        return "Google Play service timed out";
    default:
        return QString("Unknown billing response code: %1").arg(billingResponseCode);
    }
}

void GooglePlayStoreBackend::enableProcessing()
{
    if (processingEnabled())
        return;

    AbstractStoreBackend::enableProcessing();

    qDebug() << "Android: Processing enabled - processing queued transactions";
    processQueuedTransactions();
}

void GooglePlayStoreBackend::processQueuedTransactions()
{
    qDebug() << "Android: Processing" << _queuedPurchaseSucceeded.size() << "queued purchaseSucceeded,"
             << _queuedPurchaseRestored.size() << "queued purchaseRestored," << _queuedPurchasePending.size()
             << "queued purchasePending";

    // Process queued purchase succeeded
    for (const auto &json : _queuedPurchaseSucceeded) {
        auto transaction = transactionFromJson(json);
        emit purchaseSucceeded(transaction);
    }
    _queuedPurchaseSucceeded.clear();

    // Process queued purchase restored
    for (const auto &json : _queuedPurchaseRestored) {
        auto transaction = transactionFromJson(json);
        emit purchaseRestored(transaction);
    }
    _queuedPurchaseRestored.clear();

    // Process queued purchase pending
    for (const auto &json : _queuedPurchasePending) {
        auto transaction = transactionFromJson(json);
        qDebug() << "Processing queued Android purchase pending for product:" << transaction.productId;

        AbstractProduct * product = this->product(transaction.productId);
        if (product) {
            emit purchasePending(transaction);
        } else {
            qWarning() << "Could not find product for queued pending purchase:" << transaction.productId;
        }
    }
    _queuedPurchasePending.clear();
}
