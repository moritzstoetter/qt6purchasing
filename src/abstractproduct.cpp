#include <qt6purchasing/abstractproduct.h>
#include <qt6purchasing/abstractstorebackend.h>

AbstractProduct::AbstractProduct(QObject * parent) : QObject(parent)
{
    connect(this, &AbstractProduct::identifierChanged, this, &AbstractProduct::updateIsReadyForRegister);
    connect(this, &AbstractProduct::productTypeChanged, this, &AbstractProduct::updateIsReadyForRegister);
    connect(this, &AbstractProduct::isReadyForRegisterChanged, this, &AbstractProduct::registerInStore);
}

AbstractStoreBackend * AbstractProduct::findStoreBackend() const
{
    QObject * p = parent();
    while (p) {
        if (auto * store = qobject_cast<AbstractStoreBackend *>(p))
            return store;
        p = p->parent();
    }
    return nullptr;
}

void AbstractProduct::setDescription(QString value)
{
    if (_description == value)
        return;

    _description = value;
    emit descriptionChanged();
}

void AbstractProduct::setIdentifier(const QString &value)
{
    if (_identifier == value)
        return;

    _identifier = value;
    emit identifierChanged();
}

void AbstractProduct::setPrice(const QString &value)
{
    if (_price == value)
        return;

    _price = value;
    emit priceChanged();
}

void AbstractProduct::setProductType(ProductType type)
{
    if (_productType == type)
        return;

    _productType = type;
    emit productTypeChanged();
}

void AbstractProduct::setTitle(const QString &value)
{
    if (_title == value)
        return;

    _title = value;
    emit titleChanged();
}

void AbstractProduct::setStatus(ProductStatus status)
{
    if (_status == status)
        return;

    _status = status;
    qDebug() << "Product" << _identifier << _status;
    emit statusChanged();
}

void AbstractProduct::setMicrosoftStoreId(const QString &value)
{
    if (_microsoftStoreId == value)
        return;

    _microsoftStoreId = value;
    emit microsoftStoreIdChanged();
}

void AbstractProduct::registerInStore()
{
    auto * store = findStoreBackend();
    if (!store) {
        qCritical() << "Product not child of a store backend!";
        return;
    }

    if (!store->isConnected()) {
        qDebug() << "No connection to store - will register when connected";
        return;
    }

    if (_identifier.isEmpty()) {
        qDebug() << "Product has no id - skipping registration";
        return;
    }

    if (_status == PendingRegistration || _status == Registered) {
        qDebug() << "Product" << _identifier << "already registered or pending";
        return;
    }

    setStatus(AbstractProduct::PendingRegistration);
    store->registerProduct(this);
}

void AbstractProduct::updateIsReadyForRegister()
{
    bool newReadyState = (_productType != ProductType::None) && (!_identifier.isEmpty());

    if (_isReadyForRegister == newReadyState)
        return;

    _isReadyForRegister = newReadyState;
    emit isReadyForRegisterChanged();
}

// Queued, so the failure arrives after purchase() returns, like every other purchase result
static void
queuePurchaseFailed(AbstractProduct * product, AbstractStoreBackend::PurchaseError error, const QString &message)
{
    QMetaObject::invokeMethod(
        product,
        [product, error, message]() {
            emit product->purchaseFailed(static_cast<int>(error), 0, message);
        },
        Qt::QueuedConnection
    );
}

static void queuePurchaseFailed(
    AbstractStoreBackend * store,
    const QString &productId,
    AbstractStoreBackend::PurchaseError error,
    const QString &message
)
{
    QMetaObject::invokeMethod(
        store,
        [store, productId, error, message]() {
            emit store->purchaseFailed(productId, static_cast<int>(error), 0, message);
        },
        Qt::QueuedConnection
    );
}

void AbstractProduct::purchase()
{
    auto * store = findStoreBackend();
    if (!store) {
        qCritical() << "Product not child of a store backend!";
        queuePurchaseFailed(
            this, AbstractStoreBackend::PurchaseError::DeveloperError, "Product not child of a store backend"
        );
        return;
    }

    if (_identifier.isEmpty()) {
        qWarning() << "Cannot purchase - product has no identifier";
        queuePurchaseFailed(this, AbstractStoreBackend::PurchaseError::DeveloperError, "Product has no identifier");
        return;
    }

    if (!store->isConnected()) {
        qWarning() << "Cannot purchase - store not connected";
        queuePurchaseFailed(
            store, _identifier, AbstractStoreBackend::PurchaseError::ServiceUnavailable, "Store not connected"
        );
        return;
    }

    if (_status != AbstractProduct::Registered) {
        qWarning() << "Cannot purchase unregistered product:" << _identifier;
        queuePurchaseFailed(
            store, _identifier, AbstractStoreBackend::PurchaseError::ItemUnavailable, "Product not registered"
        );
        return;
    }

    store->purchaseProduct(this);
}
