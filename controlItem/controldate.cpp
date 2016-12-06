#include "controldate.h"
#include "ui_controldate.h"

ControlDate::ControlDate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlDate)
{
    ui->setupUi(this);
}

ControlDate::ControlDate(VarDesc v, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ControlDate)
{
    ui->setupUi(this);
    this->varDescDate = v.pVarDescDate;
    this->ui->dateEdit->setDate(v.pVarDescDate->getDate());
    qDebug()<<v.pVarDescDate->getDate();

}

ControlDate::~ControlDate()
{
    delete ui;
}

QDate ControlDate::getDate()
{
   QDEBUG <<this->ui->dateEdit->date();
   return this->ui->dateEdit->date();
}

void ControlDate::setDate(QDate d)
{
    this->ui->dateEdit->setDate(d);
}

void ControlDate::on_dateEdit_dateChanged(const QDate &date)
{
    QDEBUG <<date;
   emit signalStateChanged(VARTYPEDATE);
}
