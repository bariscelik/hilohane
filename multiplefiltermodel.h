#ifndef MULTIPLEFILTERMODEL_H
#define MULTIPLEFILTERMODEL_H

#include <QSortFilterProxyModel>
#include <QPair>
#include <QHash>
#include <QDate>
#include <QRegExp>

class MultipleFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    MultipleFilterModel(QObject *parent);
    virtual void setDateRangeFilter(qint32 col, const QDate& minDate, const QDate& maxDate, qint32 role = Qt::DisplayRole);
    virtual void setRegExpFilter(qint32 col, const QRegExp& matcher, qint32 role=Qt::DisplayRole);
    void setNumberFilter(qint32 col, const QString &exp, qint32 role=Qt::DisplayRole);
    virtual void clearFilters();
    virtual void removeFilterFromColumn(qint32 col, qint32 role);
    virtual void removeFilterFromColumn(qint32 col);
protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex & source_parent) const override;
private:
    QHash<qint64, QPair<QDate, QDate> >  m_dateRangeFilter;
    QHash<qint64, QRegExp > m_regExpFilter;
    QHash<qint64, QString > m_numberFilter;

};

#endif // MULTIPLEFILTERMODEL_H
