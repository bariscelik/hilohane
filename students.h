#ifndef STUDENTS_H
#define STUDENTS_H

#include <QDialog>

#include "ui_addstudent.h"
#include "ui_recordsbystudent.h"
#include "initdatabase.h"

namespace Ui {
class students;
}

class students : public QDialog
{
    Q_OBJECT

public:
    explicit students(QWidget *parent = 0);
    ~students();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void updateModel();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::students *ui;
    Ui::addstudent adstui;
    Ui::recordsbystudent rbsui;
    QSqlQueryModel *model = new QSqlQueryModel();
    QSqlQueryModel *recordsModel = new QSqlQueryModel();
    void showEvent(QShowEvent *e);
    void updateRecordsModel(int student_id, QString student_name);
};

#endif // STUDENTS_H