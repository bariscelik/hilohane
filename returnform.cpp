#include "returnform.h"
#include "ui_returnform.h"

#include <QSqlQuery>

ReturnForm::ReturnForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReturnForm)
{
    ui->setupUi(this);

    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("Vazgeç");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("İade Et");

    ui->returnDateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

void ReturnForm::setIds(QStringList idList)
{
    ids = idList;
}

ReturnForm::~ReturnForm()
{
    delete ui;
}

void ReturnForm::on_buttonBox_accepted()
{
    if(ids.size() > 0){
        QSqlQuery returnQuery;

        returnQuery.prepare("UPDATE books SET return_date = :rd WHERE id IN(" + ids.join(", ") + ")");
        returnQuery.bindValue(":rd", ui->returnDateTimeEdit->dateTime().toSecsSinceEpoch());
        returnQuery.exec();
    }
}
