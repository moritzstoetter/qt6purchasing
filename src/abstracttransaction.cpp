#include "abstracttransaction.h"
#include "abstractstorebackend.h"
#include <QJsonObject>

AbstractTransaction::AbstractTransaction(AbstractStoreBackend * store, QString orderId) : QObject(),
    _store(store),
    _orderId(orderId)
{
    if (this->thread() != _store->thread())
        this->moveToThread(_store->thread());

    this->setParent(_store);
}

void AbstractTransaction::finalize()
{
    if (!_store)
        return;

    qDebug() << "Transaction: Finalizing purchase" << this->_orderId;
    _store->consumePurchase(this);
}
