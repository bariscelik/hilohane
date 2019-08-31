#ifndef LOANDIALOG_H
#define LOANDIALOG_H

#include <QDialog>

namespace Ui {
class LoanDialog;
}


class LoanDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoanDialog(QWidget *parent, int sc_id);
    explicit LoanDialog(QWidget *parent, int l, int sc_id);
    ~LoanDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::LoanDialog *ui;
    int loanID = 0;
    int schoolID;
    void loadValues();
};

#endif // LOANDIALOG_H
