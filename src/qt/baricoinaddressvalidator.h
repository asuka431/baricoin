// Copyright (c) 2011-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef BARICOIN_QT_BARICOINADDRESSVALIDATOR_H
#define BARICOIN_QT_BARICOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class BaricoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BaricoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Baricoin address widget validator, checks for a valid baricoin address.
 */
class BaricoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit BaricoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // BARICOIN_QT_BARICOINADDRESSVALIDATOR_H
