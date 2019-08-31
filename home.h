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
#include "students.h"
#include "tools.h"
#include "multiplefiltermodel.h"
#include "coloredsqlquerymodel.h"
#include "returnform.h"
#include "books.h"

namespace Ui {
class Home;
}

class Home : public QMainWindow
{
    Q_OBJECT

public:
    explicit Home(QWidget *parent = 0, int scid = 0);
    ~Home();

public slots:
    void updateModel();
    void updateReturnedModel();
private slots:
    void on_action_to_lend_triggered();
    void on_action_delete_triggered();
    void on_action_students_triggered();
    void on_action_return_triggered();
    void on_action_export_triggered();
    void on_tabWidget_currentChanged(int index);

    void on_activeBooksTableView_doubleClicked(const QModelIndex &index);
    void on_returnedBooksTableView_doubleClicked(const QModelIndex &index);

    void on_action_settings_triggered();
    void updateStatusBar();

    void on_action_return_with_date_triggered();

    void on_action_books_triggered();

    void on_action_print_labels_triggered();

    void on_action_about_triggered();

private:
    QString getLastExecutedQuery(const QSqlQuery& query);
    QLabel *sBarTableInfo; // status bar table info
    QLabel *sBarSelected; // status bar selected items info
    Ui::Home *ui;
    ReturnForm rf;
    tools tl;
    ColoredSqlQueryModel *model = new ColoredSqlQueryModel();// model of on loan books
    ColoredSqlQueryModel *returnedModel = new ColoredSqlQueryModel();// model of returned books
    MultipleFilterModel *m = new MultipleFilterModel(this);  // proxy model of on loan books for sorting
    MultipleFilterModel *rm = new MultipleFilterModel(this); // proxy model of returned books for sorting
    bool iadetab = false;
    void updateHighlights();
    int schoolID;
};

#endif // HOME_H
