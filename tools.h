#ifndef TOOLS_H
#define TOOLS_H

#include <QDialog>

namespace Ui {
class tools;
}

class tools : public QDialog
{
    Q_OBJECT

public:
    explicit tools(QWidget *parent = 0);
    ~tools();
Q_SIGNALS:
    void updateModel();
private slots:
    void on_deleteAllBookDataBtn_clicked();

    void on_increaseClassNumberBtn_clicked();

    //void on_migrateDbBtn_clicked();

private:
    Ui::tools *ui;
};

#endif // TOOLS_H
