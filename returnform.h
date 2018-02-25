#ifndef RETURNFORM_H
#define RETURNFORM_H

#include <QDialog>
#include <QPushButton>

namespace Ui {
class ReturnForm;
}

class ReturnForm : public QDialog
{
    Q_OBJECT

public:
    explicit ReturnForm(QWidget *parent = 0);
    ~ReturnForm();

    void setIds(QStringList idList);
private slots:
    void on_buttonBox_accepted();

private:
    Ui::ReturnForm *ui;
    QStringList ids;

};

#endif // RETURNFORM_H
