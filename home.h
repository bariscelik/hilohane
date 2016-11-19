#ifndef HOME_H
#define HOME_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QIdentityProxyModel>
#include "addrecord.h"
#include "editrecord.h"
#include "students.h"

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
    void on_actionAdd_triggered();
    void on_actionDelete_triggered();
    void on_action_ogrenciler_triggered();
    void on_action_iade_triggered();
    void on_actionExport_triggered();
    void on_tabWidget_currentChanged(int index);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_iadeTableView_doubleClicked(const QModelIndex &index);

private:
    QString getLastExecutedQuery(const QSqlQuery& query);
    Ui::Home *ui;
    addRecord ar;
    editRecord er;
    students st;
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQueryModel *returnedModel = new QSqlQueryModel();
    MyModel *mymodel=new MyModel();
    bool iadetab = false;
};

#endif // HOME_H
