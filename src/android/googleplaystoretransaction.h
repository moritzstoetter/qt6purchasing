#ifndef GOOGLEPLAYSTORETRANSACTION_H
#define GOOGLEPLAYSTORETRANSACTION_H

#include "../abstracttransaction.h"

class GooglePlayStoreTransaction : public AbstractTransaction
{
    Q_OBJECT
public:
    GooglePlayStoreTransaction(AbstractStoreBackend * store, QJsonObject json);

    QString productId() const override;
    QJsonObject json() const { return _json; }

private:
    QJsonObject _json;

};

#endif // GOOGLEPLAYSTORETRANSACTION_H
