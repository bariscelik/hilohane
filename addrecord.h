#ifndef ADDRECORD_H
#define ADDRECORD_H

#include <QDialog>
#include "students.h"

namespace Ui {
class addRecord;
}

class addRecord : public QDialog
{
    Q_OBJECT

public:
    explicit addRecord(QWidget *parent = 0);
    ~addRecord();

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

    void updateModel();
private:
    Ui::addRecord *ui;
    students st;
};

#endif // ADDRECORD_H
