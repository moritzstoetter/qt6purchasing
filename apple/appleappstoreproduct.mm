#include "appleappstoreproduct.h"

AppleAppStoreProduct::AppleAppStoreProduct(QObject * parent) : AbstractProduct(parent)
{}

void AppleAppStoreProduct::setNativeProduct(SKProduct * np)
{
    _nativeProduct = np;
}
