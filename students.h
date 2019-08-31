#ifndef STUDENTS_H
#define STUDENTS_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QDir>
#include <QSortFilterProxyModel>
#include "ui_recordsbystudent.h"

namespace Ui {
class students;
}

class students : public QDialog
{
    Q_OBJECT

public:
    explicit students(QWidget *parent, int sc_id);
    ~students();
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void updateModel();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_3_clicked();
    void exportBtnClicked();

    void on_pushButton_studentDetail_clicked();

private:
    Ui::students *ui;
    Ui::recordsbystudent rbsui;
    QSqlQueryModel *model = new QSqlQueryModel();
    QSortFilterProxyModel *pModel = new QSortFilterProxyModel();
    QSqlQueryModel *recordsModel = new QSqlQueryModel();
    void showEvent(QShowEvent *e);
    void updateRecordsModel();

    QString studentName;
    int currentStudentID = 0;
    int schoolID;
};

#endif // STUDENTS_H
