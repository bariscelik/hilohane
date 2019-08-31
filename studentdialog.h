#ifndef STUDENTDIALOG_H
#define STUDENTDIALOG_H

#include <QDialog>

namespace Ui {
class StudentDialog;
}

typedef struct {
    int studentID;
    QString name;
    int number;
    QString cl;
}StudentForm;

class StudentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StudentDialog(QWidget *parent, int sc_id);
    explicit StudentDialog(QWidget *parent, StudentForm &f, int sc_id);
    ~StudentDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::StudentDialog *ui;
    int studentID = 0;
    int schoolID;
};

#endif // STUDENTDIALOG_H
