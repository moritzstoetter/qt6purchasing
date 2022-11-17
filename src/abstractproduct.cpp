#include "abstractproduct.h"
#include "globaldef.h"
#include "abstracttransaction.h"

AbstractProduct::AbstractProduct(QObject * parent) : QObject(parent)
{
    connect(this, &AbstractProduct::identifierChanged, this, &AbstractProduct::registerInStore);
    connect(this, &AbstractProduct::storeChanged, this, &AbstractProduct::registerInStore);
}

void AbstractProduct::setDescription(QString value)
{
    _description = value;
    emit descriptionChanged();
}

void AbstractProduct::setIdentifier(const QString &value)
{
    _identifier = value;
    emit identifierChanged();
}

void AbstractProduct::setPrice(const QString &value)
{
    _price = value;
    emit priceChanged();
}

void AbstractProduct::setProductType(ProductType type)
{
    _productType = type;
    emit productTypeChanged();
}

void AbstractProduct::setTitle(const QString &value)
{
    _title = value;
    emit titleChanged();
}

void AbstractProduct::setStatus(ProductStatus status)
{
    _status = status;
    qCDebug(Logging::store) << "Product" << _identifier << _status;
    emit statusChanged();
}

void AbstractProduct::registerInStore()
{
    if (!AbstractStoreBackend::instance()) {
        qCCritical(Logging::store) << "Store unavailable!";
        return;
    }

    if(!AbstractStoreBackend::instance()->isConnected()) {
        qCCritical(Logging::store) << "No connection to store!";
        return;
    }

    if (_identifier.isEmpty()) {
        qCCritical(Logging::store) << "Product has no id!";
        return;
    }

    setStatus(AbstractProduct::PendingRegistration);
    AbstractStoreBackend::instance()->registerProduct(this);
}


void AbstractProduct::purchase()
{
    if (!AbstractStoreBackend::instance()) {
        qCCritical(Logging::store) << "Store unavailable!";
        return;
    }

    if(!AbstractStoreBackend::instance()->isConnected()) {
        qCCritical(Logging::store) << "No connection to store!";
        return;
    }

    if (_identifier.isEmpty()) {
        qCCritical(Logging::store) << "Product has no id!";
        return;
    }

    if (_status != AbstractProduct::Registered) {
        qCDebug(Logging::store) << "Trying to purchase an unregistered Product. Aborting";
        return;
    }

    AbstractStoreBackend::instance()->purchaseProduct(this);
}
