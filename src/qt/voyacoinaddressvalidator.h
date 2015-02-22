// Copyright (c) 2011-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef VOYACOIN_QT_VOYACOINADDRESSVALIDATOR_H
#define VOYACOIN_QT_VOYACOINADDRESSVALIDATOR_H

#include <QValidator>

/** Base58 entry widget validator, checks for valid characters and
 * removes some whitespace.
 */
class VoyacoinAddressEntryValidator : public QValidator
{
    Q_OBJECT

public:
    explicit VoyacoinAddressEntryValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

/** Voyacoin address widget validator, checks for a valid voyacoin address.
 */
class VoyacoinAddressCheckValidator : public QValidator
{
    Q_OBJECT

public:
    explicit VoyacoinAddressCheckValidator(QObject *parent);

    State validate(QString &input, int &pos) const;
};

#endif // VOYACOIN_QT_VOYACOINADDRESSVALIDATOR_H
