#include "controltime.h"
#include "ui_controltime.h"

ControlTime::ControlTime(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlTime)
{
    ui->setupUi(this);
}

ControlTime::ControlTime(VarDesc v, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ControlTime)
{
    ui->setupUi(this);
    varDescTime = v.pVarDescTime;
    qDebug()<<v.pVarDescTime->getTime();
    this->ui->timeEdit->setTime(varDescTime->getTime());


}

ControlTime::~ControlTime()
{
    delete ui;
}

QTime ControlTime::getTime()
{
    return this->ui->timeEdit->time();
}

void ControlTime::setTime(QTime t)
{
   this->ui->timeEdit->setTime(t);
}


void ControlTime::on_timeEdit_timeChanged()
{
   emit signalStateChanged(VARTYPETIME);
}
