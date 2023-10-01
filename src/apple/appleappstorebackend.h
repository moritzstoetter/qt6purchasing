#ifndef APPLEAPPSTOREBACKEND_H
#define APPLEAPPSTOREBACKEND_H

#include "../abstractstorebackend.h"

Q_FORWARD_DECLARE_OBJC_CLASS(QT_MANGLE_NAMESPACE(InAppPurchaseManager));

QT_BEGIN_NAMESPACE

class AppleAppStoreBackend : public AbstractStoreBackend
{
public:
    AppleAppStoreBackend(QObject * parent = nullptr);
    ~AppleAppStoreBackend();

    void startConnection() override;
    void registerProduct(AbstractProduct * product) override;
    void purchaseProduct(AbstractProduct * product) override;
    void consumePurchase(AbstractTransaction * transaction) override;
    void restorePurchases(AbstractProduct* product) override;

//    void emitProductRegistered(AbstractProduct * product);
private:
    QT_MANGLE_NAMESPACE(InAppPurchaseManager) * _iapManager = nullptr;

};

QT_END_NAMESPACE

#endif // APPLEAPPSTOREBACKEND_H
