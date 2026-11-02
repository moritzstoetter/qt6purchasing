#ifndef GOOGLEPLAYSTOREPRODUCT_H
#define GOOGLEPLAYSTOREPRODUCT_H

#include <qt6purchasing/abstractproduct.h>

class GooglePlayStoreBackend;

class GooglePlayStoreProduct : public AbstractProduct
{
    Q_OBJECT
    QML_NAMED_ELEMENT(Product)

public:
    GooglePlayStoreProduct(QObject * parent = nullptr);
};

#endif // GOOGLEPLAYSTOREPRODUCT_H
