// Copyright (c) 2011-2013 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "voyacoinunits.h"

#include "primitives/transaction.h"

#include <QStringList>

VoyacoinUnits::VoyacoinUnits(QObject *parent):
        QAbstractListModel(parent),
        unitlist(availableUnits())
{
}

QList<VoyacoinUnits::Unit> VoyacoinUnits::availableUnits()
{
    QList<VoyacoinUnits::Unit> unitlist;
    unitlist.append(VOYA);
    unitlist.append(mVOYA);
    unitlist.append(uVOYA);
    return unitlist;
}

bool VoyacoinUnits::valid(int unit)
{
    switch(unit)
    {
    case VOYA:
    case mVOYA:
    case uVOYA:
        return true;
    default:
        return false;
    }
}

QString VoyacoinUnits::id(int unit)
{
    switch(unit)
    {
    case VOYA: return QString("btc");
    case mVOYA: return QString("mbtc");
    case uVOYA: return QString("ubtc");
    default: return QString("???");
    }
}

QString VoyacoinUnits::name(int unit)
{
    switch(unit)
    {
    case VOYA: return QString("VOYA");
    case mVOYA: return QString("mVOYA");
    case uVOYA: return QString::fromUtf8("μVOYA");
    default: return QString("???");
    }
}

QString VoyacoinUnits::description(int unit)
{
    switch(unit)
    {
    case VOYA: return QString("Voyacoins");
    case mVOYA: return QString("Milli-Voyacoins (1 / 1" THIN_SP_UTF8 "000)");
    case uVOYA: return QString("Micro-Voyacoins (1 / 1" THIN_SP_UTF8 "000" THIN_SP_UTF8 "000)");
    default: return QString("???");
    }
}

qint64 VoyacoinUnits::factor(int unit)
{
    switch(unit)
    {
    case VOYA:  return 100000000;
    case mVOYA: return 100000;
    case uVOYA: return 100;
    default:   return 100000000;
    }
}

int VoyacoinUnits::decimals(int unit)
{
    switch(unit)
    {
    case VOYA: return 8;
    case mVOYA: return 5;
    case uVOYA: return 2;
    default: return 0;
    }
}

QString VoyacoinUnits::format(int unit, const CAmount& nIn, bool fPlus, SeparatorStyle separators)
{
    // Note: not using straight sprintf here because we do NOT want
    // localized number formatting.
    if(!valid(unit))
        return QString(); // Refuse to format invalid unit
    qint64 n = (qint64)nIn;
    qint64 coin = factor(unit);
    int num_decimals = decimals(unit);
    qint64 n_abs = (n > 0 ? n : -n);
    qint64 quotient = n_abs / coin;
    qint64 remainder = n_abs % coin;
    QString quotient_str = QString::number(quotient);
    QString remainder_str = QString::number(remainder).rightJustified(num_decimals, '0');

    // Use SI-style thin space separators as these are locale independent and can't be
    // confused with the decimal marker.
    QChar thin_sp(THIN_SP_CP);
    int q_size = quotient_str.size();
    if (separators == separatorAlways || (separators == separatorStandard && q_size > 4))
        for (int i = 3; i < q_size; i += 3)
            quotient_str.insert(q_size - i, thin_sp);

    if (n < 0)
        quotient_str.insert(0, '-');
    else if (fPlus && n > 0)
        quotient_str.insert(0, '+');
    return quotient_str + QString(".") + remainder_str;
}


// TODO: Review all remaining calls to VoyacoinUnits::formatWithUnit to
// TODO: determine whether the output is used in a plain text context
// TODO: or an HTML context (and replace with
// TODO: BtcoinUnits::formatHtmlWithUnit in the latter case). Hopefully
// TODO: there aren't instances where the result could be used in
// TODO: either context.

// NOTE: Using formatWithUnit in an HTML context risks wrapping
// quantities at the thousands separator. More subtly, it also results
// in a standard space rather than a thin space, due to a bug in Qt's
// XML whitespace canonicalisation
//
// Please take care to use formatHtmlWithUnit instead, when
// appropriate.

QString VoyacoinUnits::formatWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    return format(unit, amount, plussign, separators) + QString(" ") + name(unit);
}

QString VoyacoinUnits::formatHtmlWithUnit(int unit, const CAmount& amount, bool plussign, SeparatorStyle separators)
{
    QString str(formatWithUnit(unit, amount, plussign, separators));
    str.replace(QChar(THIN_SP_CP), QString(THIN_SP_HTML));
    return QString("<span style='white-space: nowrap;'>%1</span>").arg(str);
}


bool VoyacoinUnits::parse(int unit, const QString &value, CAmount *val_out)
{
    if(!valid(unit) || value.isEmpty())
        return false; // Refuse to parse invalid unit or empty string
    int num_decimals = decimals(unit);

    // Ignore spaces and thin spaces when parsing
    QStringList parts = removeSpaces(value).split(".");

    if(parts.size() > 2)
    {
        return false; // More than one dot
    }
    QString whole = parts[0];
    QString decimals;

    if(parts.size() > 1)
    {
        decimals = parts[1];
    }
    if(decimals.size() > num_decimals)
    {
        return false; // Exceeds max precision
    }
    bool ok = false;
    QString str = whole + decimals.leftJustified(num_decimals, '0');

    if(str.size() > 18)
    {
        return false; // Longer numbers will exceed 63 bits
    }
    CAmount retvalue(str.toLongLong(&ok));
    if(val_out)
    {
        *val_out = retvalue;
    }
    return ok;
}

QString VoyacoinUnits::getAmountColumnTitle(int unit)
{
    QString amountTitle = QObject::tr("Amount");
    if (VoyacoinUnits::valid(unit))
    {
        amountTitle += " ("+VoyacoinUnits::name(unit) + ")";
    }
    return amountTitle;
}

int VoyacoinUnits::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return unitlist.size();
}

QVariant VoyacoinUnits::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row >= 0 && row < unitlist.size())
    {
        Unit unit = unitlist.at(row);
        switch(role)
        {
        case Qt::EditRole:
        case Qt::DisplayRole:
            return QVariant(name(unit));
        case Qt::ToolTipRole:
            return QVariant(description(unit));
        case UnitRole:
            return QVariant(static_cast<int>(unit));
        }
    }
    return QVariant();
}

CAmount VoyacoinUnits::maxMoney()
{
    return MAX_MONEY;
}
