#ifndef GOOGLEPLAYSTOREPRODUCT_H
#define GOOGLEPLAYSTOREPRODUCT_H

#include <QJsonObject>
#include "../abstractproduct.h"

class GooglePlayStoreBackend;

class GooglePlayStoreProduct : public AbstractProduct
{
    Q_OBJECT

public:
    GooglePlayStoreProduct(QObject * parent = nullptr);

    QJsonObject json() const { return _json; }
    void setJson(QJsonObject json);

private:
    QJsonObject _json;

signals:
    void jsonChanged();
};

#endif // GOOGLEPLAYSTOREPRODUCT_H
