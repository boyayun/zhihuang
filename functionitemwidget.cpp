#include "functionitemwidget.h"

FunctionItemWidget::FunctionItemWidget(FunctionItem *f, QWidget *parent) : QWidget(parent)
{
    this->functionItem = f;

    init();
    functionNameLabel.setAlignment(Qt::AlignCenter);
    mainLayout.addWidget(&functionNameLabel);

    this->functionNameLabel.setText(f->getFunctionName());
    this->setLayout(&mainLayout);
}

FunctionItemWidget::FunctionItemWidget(FunctionItem *f, void *baseAddr, QWidget *parent) : QWidget(parent)
{
    this->functionItem = f;
    this->baseAddress = baseAddr;
    init();
    functionNameLabel.setAlignment(Qt::AlignCenter);
    mainLayout.addWidget(&functionNameLabel);
    functionNameLabel.setStyleSheet("font: 45 16pt");

    this->functionNameLabel.setText(f->getFunctionName());
    this->setLayout(&mainLayout);
    QDEBUG <<"初始化状态";
    slotStateChanged(f->getVarType());
}

bool FunctionItemWidget::dealWithCommand(void * parameter)
{
    //parameter
    uint8_t varType = functionItem->getVarType();
    if(varType ==VARTYPECMD)
    {
        uint8_t *cmd = (uint8_t *)parameter;
        QList<uint8_t>comdlist = functionItem->getVarDesc().pVarDescCmd->getItem();
        QList<QString>itemList = functionItem->getVarDesc().pVarDescCmd->getDisItem();
        if(comdlist.contains(*cmd))
        {
            *((uint8_t *)baseAddress + functionItem->getDataPos()) = *cmd;
            this->controlWidget.controlCmd->setStatus(*cmd);
            emit signalLog(functionItem->getFunctionName()+":"+itemList.at(comdlist.indexOf(*cmd)),2);
        }
        else
        {
            QDEBUG << "指令错误";
            emit signalLog(functionItem->getFunctionName()+
                           ":"+QString::number(*cmd)+" "+"("+QString::number(comdlist.at(0))+","+QString::number(comdlist.at(1))+") 参数错误",3);
            return false;
            //发送错误帧
        }
    }
    else if(varType == VARTYPENUMBER) //数字型
    {
        int16_t *cmd = (int16_t *)parameter; //从网络中来的数据

        int16_t max = functionItem->getVarDesc().pVarDescNumber->getMax();
        int16_t min = functionItem->getVarDesc().pVarDescNumber->getMin();
#ifdef USE_BIG_EDITION
        int16_t cmdBig = *cmd;//小端
        ExChangeEdition((uint16_t *)cmd);//将大端转换成小段数据比较
#endif
        if((*cmd) >= min && (*cmd <= max))
        {

            QDEBUG<< *cmd;
            QDEBUG<< functionItem->getDataPos();
#ifdef USE_BIG_EDITION
            *((int16_t *)((uint8_t *)baseAddress + functionItem->getDataPos())) = cmdBig;//写入状态机
#else
            *((int16_t *)((uint8_t *)(baseAddress + functionItem->getDataPos()))) = *cmd;//写入状态机
#endif

            QDEBUG <<*((int16_t *)((uint8_t *)baseAddress + functionItem->getDataPos()));
            this->controlWidget.controlNumber->setNumber(*cmd);
            emit signalLog(functionItem->getFunctionName()+":"+
                           QString::number(*cmd),2);
        }
        else
        {
            QDEBUG <<"数字型参数错误！";
            emit signalLog(functionItem->getFunctionName()+":"+QString::number(*cmd)+" "+
                           "["+QString::number(max)+"-"+QString::number(min)+"] 参数错误"
                           ,3);
            return false;
        }
    }
    else if(varType ==VARTYPEENUMBERATE) //枚举型
    {
        uint8_t *cmd = (uint8_t *)parameter;
        QList<uint8_t> itemlist = functionItem->getVarDesc().pVarDescEnumberate->getItem();
        QList<QString> disItemsList = functionItem->getVarDesc().pVarDescEnumberate->getDisItem();
        if(itemlist.contains(*cmd))
        {
            uint8_t index = itemlist.indexOf(*cmd);
            QDEBUG << "第"<<index<<"个";
            *((uint8_t *)baseAddress + functionItem->getDataPos()) = *cmd;
            this->controlWidget.controlEnumberate->setCurrentStatus(index);
           emit signalLog(functionItem->getFunctionName()+":"+disItemsList.at(index),2);
        }
        else
        {
            QString log;
            log = functionItem->getFunctionName()+":"+QString::number(*cmd)+" "+"[";

            for(int i=0;i<itemlist.size();i++)
            {
                log += QString::number(itemlist.at(i))+"," ;
            }
            log +="] 参数错误";
            emit signalLog(log,3);
            QDEBUG <<"枚举型参数不在范围之内";
            return false;
        }



    }
    else if(varType == VARTYPEDATE) //日期型
    {
        pDate pdate = (pDate)parameter;

        //       pDate pStateMachine = (pDate)((uint8_t *)(baseAddress + functionItem->getDataPos()));
        //       *pStateMachine =*pdate;


        //写入状态机
        QDEBUG <<"日期型指令";
        QDEBUG << pdate->year << pdate->month << pdate->day;
        QDate dateMax = functionItem->getVarDesc().pVarDescDate->getMaxDate();
        QDate dateMin = functionItem->getVarDesc().pVarDescDate->getMinDate();


        //从网络端得到的year为大端，需要转换成小端
#ifdef USE_BIG_EDITION
        uint16_t yearLittle = pdate->year;  //大端
        ExChangeEdition(&yearLittle); //小端年
        QDate date(yearLittle,pdate->month,pdate->day);
        QDEBUG << date.toString();
        QDEBUG << QDate(2016,12,2).toString();
#else
        QDate date(pdate->year,pdate->month,pdate->month); //无需转换年
#endif

        if(date > dateMin && date< dateMax)
        {
            pDate pStateMachine = (pDate)((uint8_t *)baseAddress + functionItem->getDataPos());
            *pStateMachine =*pdate;
            this->controlWidget.controlDate->setDate(QDate(yearLittle,pdate->month,pdate->day));

            emit signalLog(functionItem->getFunctionName()+":"+date.toString(),2);

        }
        else
        {
            emit signalLog(functionItem->getFunctionName()+":"+date.toString()+" ["+
                           dateMin.toString()+"-"+dateMax.toString()+"] 参数错误",3);
            QDEBUG <<"不在范围之内";
            return false;
        }


    }
    else if(varType == VARTYPETIME) //时间型
    {
        pTime ptime = (pTime)parameter;
        QTime timeMax = functionItem->getVarDesc().pVarDescTime->getMaxTime();
        QTime timeMin = functionItem->getVarDesc().pVarDescTime->getMinTime();
        QTime time(ptime->hour,ptime->minute,ptime->second);
        pTime pStateMachine = (pTime)((uint8_t *)baseAddress + functionItem->getDataPos());
        *pStateMachine = *ptime;
        this->controlWidget.controlTime->setTime(time);
        emit signalLog(functionItem->getFunctionName()+":"+time.toString(),2);
    }
    else if(varType == VARTYPEDATETIME) //时间日期型
    {
    }
    else
    {
        QDEBUG << "无效命令";

        return false;

    }
    return true;


}

void FunctionItemWidget::stopAlarm()
{
    this->controlWidget.controlAlarm->stopAlarm();


}

uint8_t FunctionItemWidget::ZhihuangControl(uint8_t ctrlpos)
{
    int16_t max = functionItem->getVarDesc().pVarDescNumber->getMax();
    int16_t min = functionItem->getVarDesc().pVarDescNumber->getMin();
    int16_t value = 0;
    int16_t tmp = 0;

//    value = *((int16_t *)((uint8_t *)baseAddress + functionItem->getDataPos()));
    value = this->controlWidget.controlNumber->getNumber();

    if(((value >= min) && (value <= max)) && ((ctrlpos >= min) && (ctrlpos <= max)))
    {
        QDEBUG<< value;
        QDEBUG<< ctrlpos;

        QDEBUG<< functionItem->getDataPos();

        if(value < ctrlpos)
        {
            value++;
            tmp = value;
            ExChangeEdition((uint16_t*)&tmp);

            *((int16_t *)((uint8_t *)baseAddress + functionItem->getDataPos())) = tmp;//写入状态机

            QDEBUG <<*((int16_t *)((uint8_t *)baseAddress + functionItem->getDataPos()));
            this->controlWidget.controlNumber->setNumber(value);
            emit signalLog(functionItem->getFunctionName()+":"+
                           QString::number(value),2);
        }

        else if(value > ctrlpos)
        {
            value--;
            tmp = value;
            ExChangeEdition((uint16_t*)&tmp);
            *((int16_t *)((uint8_t *)baseAddress + functionItem->getDataPos())) = tmp;//写入状态机

            QDEBUG <<*((int16_t *)((uint8_t *)baseAddress + functionItem->getDataPos()));
            this->controlWidget.controlNumber->setNumber(value);
            emit signalLog(functionItem->getFunctionName()+":"+
                           QString::number(value),2);
        }
    }
    return value;
}

void FunctionItemWidget::slotStateChanged(uint8_t dataType)
{
    void *address = (uint8_t *)baseAddress + functionItem->getDataPos();
    QDEBUG <<"数据偏移："<<functionItem->getDataPos();
    if(dataType == VARTYPECMD)
    {
        uint8_t *v = (uint8_t *)address;

        uint8_t status = this->controlWidget.controlCmd->getStatus();
        qDebug()<<"**********"<<status;
        if(status == 0) //左边
        {
            *v =  ((this->functionItem)->getVarDesc().pVarDescCmd)->getItem().at(0);
        }
        else if(status == 1)//右边
        {
            *v =  ((this->functionItem)->getVarDesc().pVarDescCmd)->getItem().at(1);
        }
        qDebug()<<*v;
    }
    else if(dataType == VARTYPENUMBER)
    {
        qDebug()<<"数字改变"<<controlWidget.controlNumber->getNumber();
        int16_t *v = (int16_t *)(address);
#ifdef USE_BIG_EDITION
        int16_t vBig = controlWidget.controlNumber->getNumber();
        ExChangeEdition((uint16_t *)(&vBig));
        *v = vBig;
#else
        *v = controlWidget.controlNumber->getNumber();
#endif
        qDebug()<<*v;
    }
    else if(dataType == VARTYPEENUMBERATE)
    {
        qDebug()<<"枚举改变"<<functionItem->getVarDesc().pVarDescEnumberate->getDisItem().at(controlWidget.controlEnumberate->getCurrentStatus());
        uint8_t *v = (uint8_t *)(address);
        *v = functionItem->getVarDesc().pVarDescEnumberate->getItem().at(controlWidget.controlEnumberate->getCurrentStatus());
        qDebug()<<*v;
    }
    else if(dataType == VARTYPEALARM)
    {
        qDebug()<<"报警变量改变"<<controlWidget.controlAlarm->getHasBytepos();

        if(this->controlWidget.controlAlarm->getHasBytepos())
        {
            qDebug()<<"报警变量改变2";

            QList<uint8_t> allBytepos = functionItem->getVarDesc().pVarDescAlarm->getBytePos();
            QList<uint8_t> allBitpos = functionItem->getVarDesc().pVarDescAlarm->getBitPos();
            QList<uint16_t> alarmCodeList = functionItem->getVarDesc().pVarDescAlarm->getAlarmCode();
            QList<uint16_t> alarmCurrentCodeList = this->controlWidget.controlAlarm->getCurrentAlarmCodelist();
            qDebug()<<alarmCurrentCodeList;

            for(int i=0; i < allBytepos.size(); i++)
            {
                uint8_t *p = (uint8_t *)baseAddress+allBytepos.at(i);
               // *p = 0x00;
               // if(allBytepos.contains(flagBytePos.at(i)))
                if(alarmCurrentCodeList.contains(alarmCodeList.at(i)))
                {

                    (*p) |= (0x01 << allBitpos.at(i));
                }
                else
                {
                    (*p) &= (~(0x01<<allBitpos.at(i)));

                }
            }
        }
    }
    else if(dataType == VARTYPETIME)
    {
        qDebug()<<"时间改变";
        QTime time = controlWidget.controlTime->getTime();
        uint8_t *hour = (uint8_t *)address;
        uint8_t *minute = (uint8_t *)address + 1;
        uint8_t *second = (uint8_t *)address + 2;

        *hour = time.hour();
        *minute = time.minute();
        *second = time.second();

    }
    else if(dataType == VARTYPEDATE)
    {
        QDate date = controlWidget.controlDate->getDate();
        uint16_t *year = (uint16_t *)address; //定义一个指正指向地址
        uint8_t *month = (uint8_t *)address + 2;
        uint8_t *day = (uint8_t *)address + 3;
#ifdef USE_BIG_EDITION
        uint16_t yearBig = date.year();
        ExChangeEdition(&yearBig);
        *year  = yearBig;
#else
        *year = date.year();
#endif
        *month = date.month();
        *day = date.day();
    }
    else
    {
        QDEBUG<< "错误";
    }
    emit signalStatusChanged(); //通知发送状态改变帧
    if(false == this->zhihuang)         //智皇
    {
        emit signalZhihuangStatusChanged(dataType);
    }
}

void FunctionItemWidget::slotAlarm(uint16_t AlarmCode)
{
   QDEBUG << AlarmCode;
   QDEBUG << this->functionItem->getVarDesc().pVarDescAlarm->getAlarmString().at(controlWidget.controlAlarm->getAlarm());
   QDEBUG << this->controlWidget.controlAlarm->getAlarm();
   emit signalALarm(AlarmCode);
   emit signalLog(functionItem->getVarDesc().pVarDescAlarm->getAlarmString().at(controlWidget.controlAlarm->getAlarm()),5);
}

void FunctionItemWidget::slotZhihuang(bool i)
{
    this->zhihuang = i;
}

void FunctionItemWidget::init()
{
    switch (functionItem->getVarType())
    {
    case VARTYPECMD:
        //  this->controlWidget.controlCmd = new ControlCmd();
        qDebug()<<functionItem->getVarDesc().pVarDescCmd->getDisItem();
        this->controlWidget.controlCmd = new ControlCmd(functionItem->getVarDesc());
        connect(this->controlWidget.controlCmd,SIGNAL(signalStateChanged(uint8_t)),this,SLOT(slotStateChanged(uint8_t)));

        mainLayout.addWidget(controlWidget.controlCmd);
        break;
    case VARTYPENUMBER://数字型//
        //        this->controlWidget.controlNumber = new ControlNumber();
        qDebug()<<__FUNCTION__<<functionItem->getVarDesc().pVarDescNumber->getMax();
        this->controlWidget.controlNumber = new ControlNumber(functionItem->getVarDesc());
        connect(controlWidget.controlNumber,SIGNAL(signalStateChanged(uint8_t)),this,SLOT(slotStateChanged(uint8_t)));

        mainLayout.addWidget(controlWidget.controlNumber);

        break;
    case VARTYPEENUMBERATE : //枚举型
        this->controlWidget.controlEnumberate = new ControlEnumberate(functionItem->getVarDesc());
        connect(controlWidget.controlEnumberate,SIGNAL(signalStateChanged(uint8_t)),this,SLOT(slotStateChanged(uint8_t)));
        mainLayout.addWidget(controlWidget.controlEnumberate);

        break;
    case VARTYPEALARM:
        //        this->controlWidget.controlAlarm = new ControlAlarm();

        this->controlWidget.controlAlarm = new ControlAlarm(functionItem->getVarDesc());
        connect(controlWidget.controlAlarm,SIGNAL(signalAlarms(uint16_t)),this,SLOT(slotAlarm(uint16_t)));
        connect(controlWidget.controlAlarm,SIGNAL(signalStateChanged(uint8_t)),this,SLOT(slotStateChanged(uint8_t)));


        mainLayout.addWidget(controlWidget.controlAlarm);
        break;
    case VARTYPEDATE:  //日期型
        //        this->controlWidget.controlDate = new ControlDate();
        this->controlWidget.controlDate = new ControlDate(functionItem->getVarDesc());
        connect(controlWidget.controlDate,SIGNAL(signalStateChanged(uint8_t)),this,SLOT(slotStateChanged(uint8_t)));

        mainLayout.addWidget(controlWidget.controlDate);
        break;
    case  VARTYPETIME: //时间型
        // this->controlWidget.controlTime = new ControlTime();
        this->controlWidget.controlTime = new ControlTime(functionItem->getVarDesc());
        connect(controlWidget.controlTime,SIGNAL(signalStateChanged(uint8_t)),this,SLOT(slotStateChanged(uint8_t)));
        mainLayout.addWidget(controlWidget.controlTime);
        break;
    default:
        break;
    }
}

