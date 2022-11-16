#include "appleappstoretransaction.h"

#import <StoreKit/StoreKit.h>

AppleAppStoreTransaction::AppleAppStoreTransaction(AbstractStoreBackend * store, SKPaymentTransaction * transaction) : AbstractTransaction(store, QString::fromNSString(transaction.transactionIdentifier)),
    _nativeTransaction(transaction)
{}

QString AppleAppStoreTransaction::productId() const
{
    return QString::fromNSString(_nativeTransaction.payment.productIdentifier);
}
