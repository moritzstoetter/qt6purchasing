#include "googleplaystoreproduct.h"

GooglePlayStoreProduct::GooglePlayStoreProduct(QObject * parent) : AbstractProduct(parent)
{}

void GooglePlayStoreProduct::setJson(QJsonObject json)
{
    _json = json;
    emit jsonChanged();
}

