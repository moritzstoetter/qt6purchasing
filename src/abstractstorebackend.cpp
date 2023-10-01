#include "abstractstorebackend.h"
#include "abstractproduct.h"
#include "abstracttransaction.h"

AbstractStoreBackend * AbstractStoreBackend::_instance = nullptr;

AbstractStoreBackend::AbstractStoreBackend(QObject * parent) : QObject(parent)
{
    _instance = this;

    qDebug() << "Creating store backend";

    connect(this, &AbstractStoreBackend::connectedChanged, [this](bool connected) {
        _connected = connected;
        if (connected) {
            qDebug() << "Connected to store";
            qDebug() << "Found" << _products.size() << "product(s) awaiting registration";
            for (AbstractProduct * product : _products) {
                product->registerInStore();
            }
        } else {
            qDebug() << "Disconnected from store";
        }
    });
    connect(this, &AbstractStoreBackend::productRegistered, [](AbstractProduct * product){
        qDebug() << "Product registered:" << product->identifier();
    });
    connect(this, &AbstractStoreBackend::purchaseSucceeded, [this](AbstractTransaction * transaction){
        qDebug() << "purchaseSucceeded:" << transaction->orderId();

        AbstractProduct * ap = product(transaction->productId());
        if (ap) {
            emit ap->purchaseSucceeded(transaction);
        } else {
            qCritical() << "Failed to map successful purchase to a product!";
        }
    });
    connect(this, &AbstractStoreBackend::purchaseRestored, [this](AbstractTransaction * transaction){
        qDebug() << "purchaseRestored:" << transaction->orderId();

        AbstractProduct * ap = product(transaction->productId());
        if (ap) {
            emit ap->purchaseRestored(transaction);
        } else {
            qCritical() << "Failed to map restored purchase to a product!";
        }
    });
    connect(this, &AbstractStoreBackend::purchaseFailed, [](int code){
        qDebug() << "purchaseFailed:" << code;
    });
    connect(this, &AbstractStoreBackend::purchaseConsumed, [this](AbstractTransaction * transaction){
        qDebug() << "purchaseConsumed:" << transaction->orderId();

        AbstractProduct * ap = product(transaction->productId());
        if (ap) {
            emit ap->purchaseConsumed(transaction);
        } else {
            qCritical() << "Failed to map consumed purchase to a product!";
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
