#include "controlnumber.h"
#include "ui_controlnumber.h"

ControlNumber::ControlNumber(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlNumber)
{
    ui->setupUi(this);
}

ControlNumber::ControlNumber(VarDesc v, QWidget *parent):
    QWidget(parent),ui(new Ui::ControlNumber)
{

    ui->setupUi(this);
    this->varDescNumber = v.pVarDescNumber;
    this->ui->dial->setMaximum(varDescNumber->getMax());
    this->ui->dial->setMinimum(varDescNumber->getMin());

    qDebug()<<"This is the parameter of number:";
    qDebug()<<__FUNCTION__<<"max"<<varDescNumber->getMax();
    qDebug()<<varDescNumber->getMin();
    qDebug()<<varDescNumber->getScalling();
    qDebug()<<varDescNumber->getUnit();

}

ControlNumber::~ControlNumber()
{
    delete ui;
}

int16_t ControlNumber::getNumber()
{
    return this->ui->dial->value();
}

void ControlNumber::setNumber(int16_t number)
{
   this->ui->dial->setValue(number);
    this->ui->label->setText(QString::number(number));
}

void ControlNumber::on_dial_sliderReleased()
{
   emit signalStateChanged(VARTYPENUMBER);
}
