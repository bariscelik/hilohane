#include "multiplefiltermodel.h"
#include <QDebug>

MultipleFilterModel::MultipleFilterModel(QObject *parent){}

void MultipleFilterModel::setDateRangeFilter(qint32 col, const QDate& minDate, const QDate& maxDate, qint32 role)
{
    if (minDate.isNull() && maxDate.isNull())
        return removeFilterFromColumn(col,role);
    m_regExpFilter.remove((static_cast<qint64>(col) << 32) | static_cast<qint64>(role));
    QPair<QDate, QDate> result(minDate, maxDate);
    m_dateRangeFilter[(static_cast<qint64>(col) << 32) | static_cast<qint64>(role)] = result;
    invalidateFilter();
}

void MultipleFilterModel::setNumberFilter(qint32 col, const QString& exp, qint32 role)
{
    if (exp.isNull() || exp.isEmpty())
        return removeFilterFromColumn(col, role);
    m_numberFilter[(static_cast<qint64>(col) << 32) | static_cast<qint64>(role)] = exp;
    invalidateFilter();
}

void MultipleFilterModel::setRegExpFilter(qint32 col, const QRegExp& matcher, qint32 role)
{
    if(matcher.isEmpty() || !matcher.isValid())
        return removeFilterFromColumn(col, role);
    m_dateRangeFilter.remove((static_cast<qint64>(col) << 32) | static_cast<qint64>(role));
    m_regExpFilter[(static_cast<qint64>(col) << 32) | static_cast<qint64>(role)] = matcher;
    invalidateFilter();
}

void MultipleFilterModel::clearFilters()
{
    m_dateRangeFilter.clear();
    m_regExpFilter.clear();
    invalidateFilter();
}

void MultipleFilterModel::removeFilterFromColumn(qint32 col)
{
    for (auto i = m_dateRangeFilter.begin(); i != m_dateRangeFilter.end();){
        if (static_cast<qint32>(i.key() >> 32) == col)
            i = m_dateRangeFilter.erase(i);
        else
            ++i;
    }

    for (auto i = m_regExpFilter.begin(); i != m_regExpFilter.end();) {
        if (static_cast<qint32>(i.key() >> 32) == col)
            i = m_regExpFilter.erase(i);
        else
            ++i;
    }

    for (auto i = m_numberFilter.begin(); i != m_numberFilter.end();){
        if (static_cast<qint32>(i.key() >> 32) == col)
            i = m_numberFilter.erase(i);
        else
            ++i;
    }

    invalidateFilter();

}
void MultipleFilterModel::removeFilterFromColumn(qint32 col, qint32 role)
{
    m_dateRangeFilter.remove((static_cast<qint64>(col) << 32) | static_cast<qint64>(role));
    m_regExpFilter.remove((static_cast<qint64>(col) << 32) | static_cast<qint64>(role));
    m_numberFilter.remove((static_cast<qint64>(col) << 32) | static_cast<qint64>(role));
    invalidateFilter();
}

bool MultipleFilterModel::filterAcceptsRow(int source_row, const QModelIndex & source_parent) const
{

    for (int i = 0; i < sourceModel()->columnCount(source_parent); ++i) {

        const QModelIndex currntIndex = sourceModel()->index(source_row, i, source_parent);

        for (auto dateRngIter = m_dateRangeFilter.constBegin(); dateRngIter != m_dateRangeFilter.constEnd(); ++dateRngIter) {
            if (static_cast<qint32>(dateRngIter.key() >> 32) == i) {
                const QDate testDate = QDateTime::fromString(currntIndex.data(static_cast<qint32>(dateRngIter.key() & ((static_cast<qint64>(1) << 32) - 1))).toString()).date();
                if (!((testDate >= dateRngIter.value().first || dateRngIter.value().first.isNull()) && (testDate <= dateRngIter.value().second || dateRngIter.value().second.isNull())))
                    return false;
            }
        }

        for (auto regExpIter = m_regExpFilter.constBegin(); regExpIter != m_regExpFilter.constEnd(); ++regExpIter) {
            if (static_cast<qint32>(regExpIter.key() >> 32) == i) {
                if (regExpIter.value().indexIn(currntIndex.data(static_cast<qint32>(regExpIter.key() & ((static_cast<qint64>(1) << 32) - 1))).toString().trimmed()) < 0)
                    return false;
            }
        }

        for (auto numFiltIter = m_numberFilter.constBegin(); numFiltIter != m_numberFilter.constEnd(); ++numFiltIter) {
            if (static_cast<qint32>(numFiltIter.key() >> 32) == i) {
                QString expStr = numFiltIter.value().simplified().replace(" ", "");

                int pageNumber = currntIndex.data(static_cast<qint32>(numFiltIter.key() & ((static_cast<qint64>(1) << 32) - 1))).toInt();

                if(expStr.at(0) == '>' && expStr.length() > 1)
                {
                    expStr = expStr.remove(0,1);

                    if(pageNumber<=expStr.toInt())
                        return false;

                }else if(expStr.at(0) == '<' && expStr.length() > 1)
                {
                    expStr = expStr.remove(0,1);


                    if(pageNumber>=expStr.toInt())
                        return false;
                }else{
                    if(pageNumber != expStr.toInt())
                        return false;
                }

            }
        }
    }
    return true;
}
