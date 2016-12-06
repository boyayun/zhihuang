#include "controlcmd.h"
#include "ui_controlcmd.h"

ControlCmd::ControlCmd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlCmd)
{
    ui->setupUi(this);
}

ControlCmd::ControlCmd(VarDesc v,QWidget *parent):
    QWidget(parent),
    ui(new Ui::ControlCmd)
{
    ui->setupUi(this);
    varDescCmd = v.pVarDescCmd;

    this->ui->labelleft->setText(v.pVarDescCmd->getDisItem().at(0));
    this->ui->labelright->setText(v.pVarDescCmd->getDisItem().at(1));
    qDebug()<<v.pVarDescCmd->getItem();

}

uint8_t ControlCmd::getStatus()
{
    return this->ui->horizontalSlider->value();
}

void ControlCmd::setStatus(uint8_t v)
{
   if(v == varDescCmd->getItem().first())
   {
      this->ui->horizontalSlider->setValue(0);
      QDEBUG << varDescCmd->getDisItem().at(0);
   }
   else
   {
      this->ui->horizontalSlider->setValue(1);
       QDEBUG << varDescCmd->getDisItem().at(1);
   }
}

ControlCmd::~ControlCmd()
{
    delete ui;
}

void ControlCmd::on_horizontalSlider_valueChanged()
{
    emit signalStateChanged(VARTYPECMD);
}
