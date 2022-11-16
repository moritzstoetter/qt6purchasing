#include "googleplaystoretransaction.h"

GooglePlayStoreTransaction::GooglePlayStoreTransaction(AbstractStoreBackend * store, QJsonObject json) : AbstractTransaction(store, json["orderId"].toString()),
    _json(json)
{}

QString GooglePlayStoreTransaction::productId() const
{
    return _json["productId"].toString();
}
