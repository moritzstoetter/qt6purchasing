#ifndef ABSTRACTTRANSACTION_H
#define ABSTRACTTRANSACTION_H

#include <QJsonObject>
#include <QObject>

class AbstractStoreBackend;

class AbstractTransaction : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int status READ status NOTIFY statusChanged)
    Q_PROPERTY(QString orderId READ orderId CONSTANT)

public:
    enum Status {
        PurchaseApproved,
        PurchaseFailed,
        PurchaseRestored,
        PurchaseConsumed
    };
    Q_ENUM(Status)

    int status() const { return _status; }
    QString orderId() const { return _orderId; }
    virtual QString productId() const = 0;

    Q_INVOKABLE void finalize();

protected:
    explicit AbstractTransaction(AbstractStoreBackend * store, QString orderId);
    AbstractStoreBackend * _store = nullptr;
    int _status;
    QString _orderId;

signals:
    void statusChanged();

};

#endif // ABSTRACTTRANSACTION_H

