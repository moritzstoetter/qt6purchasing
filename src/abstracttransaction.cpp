#include "abstracttransaction.h"
#include "globaldef.h"
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

    qCDebug(Logging::store) << "Transaction: Finalizing purchase" << this->_orderId;
    _store->consumePurchase(this);
}
