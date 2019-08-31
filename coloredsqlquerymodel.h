#ifndef COLOREDSQLQUERYMODEL_H
#define COLOREDSQLQUERYMODEL_H

#include <QSqlQueryModel>

class ColoredSqlQueryModel : public QSqlQueryModel
{
public:
    ColoredSqlQueryModel(){}

    QVariant data(const QModelIndex &index, int role) const override
    {
        if (mData.contains(index))
            if (role == Qt::BackgroundRole || role == Qt::ForegroundRole)
                return mData[index][role];

        return QSqlQueryModel::data(index,role);
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override
    {
        mData[index][role] = value;
        return QSqlQueryModel::setData(index, value, role);
    }

    void resetData(const QModelIndex &index)
    {
        mData.remove(index);
    }

private:
    QMap<QModelIndex,QMap<int,QVariant>> mData;
};

#endif // COLOREDSQLQUERYMODEL_H
