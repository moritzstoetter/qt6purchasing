#ifndef ABSTRACTSTOREBACKEND_H
#define ABSTRACTSTOREBACKEND_H

#include <QJsonDocument>
#include <QObject>
#include <QQmlListProperty>

class AbstractProduct;
class AbstractTransaction;

class AbstractStoreBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<AbstractProduct> productsQml READ productsQml)
    Q_CLASSINFO("DefaultProperty", "productsQml")

public:
    static AbstractStoreBackend * instance() { return _instance; }
    QQmlListProperty<AbstractProduct> productsQml() { return {this, &_products}; }
    QList<AbstractProduct *> products() { return _products; }
    AbstractProduct * product(const QString &identifier);
    bool isConnected() const { return _connected; }

    virtual void startConnection() = 0;
    virtual void registerProduct(AbstractProduct * product) = 0;
    virtual void purchaseProduct(AbstractProduct * product) = 0;
    virtual void consumePurchase(AbstractTransaction * transaction) = 0;

protected:
    explicit AbstractStoreBackend(QObject * parent = nullptr);
    static AbstractStoreBackend * _instance;
    QList<AbstractProduct *> _products;
    bool _connected = false;

signals:
    void connectedChanged(bool connected);
    void productRegistered(AbstractProduct * product);
    void purchaseSucceeded(AbstractTransaction * transaction);
    void purchaseRestored(AbstractTransaction * transaction);
    void purchaseFailed(int code);
    void purchaseConsumed(AbstractTransaction * transaction);
};

#endif // ABSTRACTSTOREBACKEND_H
