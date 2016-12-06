#include "controlenumberate.h"
#include "ui_controlenumberate.h"

ControlEnumberate::ControlEnumberate(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlEnumberate)
{
    ui->setupUi(this);
}

ControlEnumberate::ControlEnumberate(VarDesc v, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ControlEnumberate)
{
    ui->setupUi(this);
    qDebug()<<__FUNCTION__<<__LINE__;
    qDebug()<<v.pVarDescEnumberate->getDisItem();
    qDebug()<<v.pVarDescEnumberate->getItem();
    this->ui->comboBox->addItems(v.pVarDescEnumberate->getDisItem());

}

ControlEnumberate::~ControlEnumberate()
{
    delete ui;
}

uint8_t ControlEnumberate::getCurrentStatus()
{
    return this->ui->comboBox->currentIndex();
}

void ControlEnumberate::setCurrentStatus(uint8_t index)
{
   this->ui->comboBox->setCurrentIndex(index);
}

void ControlEnumberate::on_comboBox_currentIndexChanged()
{
   emit signalStateChanged(VARTYPEENUMBERATE);
}
