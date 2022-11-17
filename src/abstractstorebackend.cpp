#include "abstractstorebackend.h"
#include "globaldef.h"
#include "abstractproduct.h"
#include "abstracttransaction.h"

AbstractStoreBackend * AbstractStoreBackend::_instance = nullptr;

AbstractStoreBackend::AbstractStoreBackend(QObject * parent) : QObject(parent)
{
    _instance = this;

    qCDebug(Logging::store) << "Creating store backend";

    connect(this, &AbstractStoreBackend::connectedChanged, [this](bool connected) {
        _connected = connected;
        if (connected) {
            qCDebug(Logging::store) << "Connected to store";
            qCDebug(Logging::store) << "Found" << _products.size() << "product(s) awaiting registration";
            for (AbstractProduct * product : _products) {
                product->registerInStore();
            }
        } else {
            qCDebug(Logging::store) << "Disconnected from store";
        }
    });
    connect(this, &AbstractStoreBackend::productRegistered, [](AbstractProduct * product){
        qCDebug(Logging::store) << "Product registered:" << product->identifier();
    });
    connect(this, &AbstractStoreBackend::purchaseSucceeded, [this](AbstractTransaction * transaction){
        qCDebug(Logging::store) << "purchaseSucceeded:" << transaction->orderId();

        AbstractProduct * ap = product(transaction->productId());
        if (ap) {
            emit ap->purchaseSucceeded(transaction);
        } else {
            qCCritical(Logging::store) << "Failed to map successful purchase to a product!";
        }
    });
    connect(this, &AbstractStoreBackend::purchaseRestored, [this](AbstractTransaction * transaction){
        qCDebug(Logging::store) << "purchaseRestored:" << transaction->orderId();

        AbstractProduct * ap = product(transaction->productId());
        if (ap) {
            emit ap->purchaseRestored(transaction);
        } else {
            qCCritical(Logging::store) << "Failed to map restored purchase to a product!";
        }
    });
    connect(this, &AbstractStoreBackend::purchaseFailed, [](int code){
        qCDebug(Logging::store) << "purchaseFailed:" << code;
    });
    connect(this, &AbstractStoreBackend::purchaseConsumed, [this](AbstractTransaction * transaction){
        qCDebug(Logging::store) << "purchaseConsumed:" << transaction->orderId();

        AbstractProduct * ap = product(transaction->productId());
        if (ap) {
            emit ap->purchaseConsumed(transaction);
        } else {
            qCCritical(Logging::store) << "Failed to map consumed purchase to a product!";
        }
    });
}

AbstractProduct * AbstractStoreBackend::product(const QString &identifier)
{
    for (AbstractProduct * ap : _products) {
        if (ap->identifier() == identifier)
            return ap;
    }
    return nullptr;
}
