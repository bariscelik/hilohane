#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QIdentityProxyModel>
#include <QSortFilterProxyModel>
#include "addrecord.h"
#include "editrecord.h"
#include "students.h"
#include "tools.h"
#include "multiplefiltermodel.h"

class MyModel : public QIdentityProxyModel
{
    QColor calculateColorForRow(int row) const {

    }

    QVariant data(const QModelIndex &index, int role)
    {
        qDebug("deneyaa");
        if (role == Qt::BackgroundRole) {
           return QVariant(QColor(Qt::blue));
        }

        return QIdentityProxyModel::data(index, role);
    }
};

namespace Ui {
class Home;
}

class Home : public QMainWindow
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = 0);
    ~Home();

public slots:
    void updateModel();
    void updateReturnedModel();
private slots:
    void on_action_add_triggered();
    void on_action_delete_triggered();
    void on_action_students_triggered();
    void on_action_return_triggered();
    void on_action_export_triggered();
    void on_tabWidget_currentChanged(int index);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_returnedBooksTableView_doubleClicked(const QModelIndex &index);

    void on_actionTools_triggered();

private:
    QString getLastExecutedQuery(const QSqlQuery& query);
    Ui::Home *ui;
    addRecord ar;
    editRecord er;
    students st;
    tools tl;
    QSqlQueryModel *model = new QSqlQueryModel();// model of on loan books
    QSqlQueryModel *returnedModel = new QSqlQueryModel();// model of returned books
    MyModel *mymodel=new MyModel();
    MultipleFilterModel *m = new MultipleFilterModel(this);  // proxy model of on loan books for sorting
    MultipleFilterModel *rm = new MultipleFilterModel(this); // proxy model of returned books for sorting
    bool iadetab = false;
};

#endif // HOME_H
