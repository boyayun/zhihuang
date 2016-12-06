#include "controlalarm.h"
#include "ui_controlalarm.h"

ControlAlarm::ControlAlarm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlAlarm)
{
    ui->setupUi(this);
}


ControlAlarm::ControlAlarm(VarDesc v, QWidget *parent):
    QWidget(parent),
    ui(new Ui::ControlAlarm)
{
    ui->setupUi(this);
    this->varDescAlarm = v.pVarDescAlarm;
    qDebug()<<__FILE__<<__LINE__;
    qDebug()<<varDescAlarm->getAlarmString();
    qDebug()<<varDescAlarm->getAlarmCode();
    qDebug()<<varDescAlarm->getAlarmClass();
    qDebug()<<varDescAlarm->getBytePos();
    qDebug()<<varDescAlarm->getBitPos();
    this->ui->comboBox->addItems(varDescAlarm->getAlarmString());
    currentErrorCode = v.pVarDescAlarm->getAlarmCode().at(0);
    this->hasBytepos = (v.pVarDescAlarm->getAlarmCode().length() == v.pVarDescAlarm->getBytePos().length());
    this->selectPanel = NULL;

    connect(&alarmTimer,SIGNAL(timeout()),this,SLOT(slotAlarmCome()));


}

uint8_t ControlAlarm::getAlarm()
{
    return this->ui->comboBox->currentIndex();
}

void ControlAlarm::stopAlarm()
{
    this->ui->checkBox->setChecked(false);
    this->alarmTimer.stop();
    this->ui->spinBox->setEnabled(true);
}

ControlAlarm::~ControlAlarm()
{
    delete ui;
}

void ControlAlarm::on_checkBox_clicked(bool checked)
{

    QDEBUG << "点击报警"<<checked;
    if(checked)
    {
        alarmTimer.start(this->ui->spinBox->value()*1000);
        this->ui->spinBox->setEnabled(false);
    }
    else {
        alarmTimer.stop();
        this->ui->spinBox->setEnabled(true);
    }
}

void ControlAlarm::slotAlarmCome()
{
    QDEBUG << "Alarm Come";
    emit signalAlarms(currentErrorCode);
}

void ControlAlarm::on_comboBox_currentIndexChanged(int index)
{
    this->currentErrorCode = varDescAlarm->getAlarmCode().at(index);
    qDebug()<<this->currentErrorCode;
//    emit signalStatusChanged(VARTYPEALARM);
}

void ControlAlarm::on_spinBox_valueChanged()
{

}

void ControlAlarm::on_checkBox_clicked()
{

}
bool ControlAlarm::getHasBytepos() const
{
    return hasBytepos;
}

void ControlAlarm::setHasBytepos(const&value)
{
    this->hasBytepos = value;
}




QList<uint8_t> ControlAlarm::getFlagBitPos() const
{
    return flagBitPos;
}

void ControlAlarm::setFlagBitPos(const QList<uint8_t> &value)
{
    flagBitPos = value;
}

QList<uint8_t> ControlAlarm::getFlagBytePos() const
{
    return flagBytePos;
}

void ControlAlarm::setFlagBytePos(const QList<uint8_t> &value)
{
    flagBytePos = value;
}


void ControlAlarm::on_pushButton_clicked()
{

    if(this->selectPanel == NULL)
    {
        this->selectPanel = new SelectPanel(this->varDescAlarm->getAlarmString(),this->varDescAlarm->getAlarmCode());
        connect(selectPanel,SIGNAL(signalALarmStateChanged(QList<uint16_t>)),this,SLOT(slotAlarmStatusChanged(QList<uint16_t>)));
    }
    this->selectPanel->exec();
}
QList<uint16_t> ControlAlarm::getCurrentAlarmCodelist() const
{
    return currentAlarmCodelist;
}


void ControlAlarm::slotAlarmStatusChanged(QList<uint16_t> l)
{
    this->currentAlarmCodelist = l;
    emit signalStateChanged(VARTYPEALARM);
}
