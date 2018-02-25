#ifndef EDITRECORD_H
#define EDITRECORD_H

#include <QDialog>
#include "students.h"

namespace Ui {
class editRecord;
}

class editRecord : public QDialog
{
    Q_OBJECT

public:
    explicit editRecord(QWidget *parent = 0);
    ~editRecord();

    void setRecordId(int id);
private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

public slots:
    void updateModel();

protected:
    void showEvent(QShowEvent *event);
private:
    Ui::editRecord *ui;
    students st;
    int recordId = 0;
    bool isActive = false;
};


#endif // EDITRECORD_H
