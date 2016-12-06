#include "controlpanel.h"

ControlPanel::ControlPanel(QWidget *parent) : QWidget(parent)
{
    this->setLayout(&mainLayout);
    FunctionItem f1,f2,f3,f4,f5,f6;
    f1.setVarType(1);
    f2.setVarType(2);
    f3.setVarType(3);
    f4.setVarType(4);
    f5.setVarType(5);
    f6.setVarType(6);

    mainLayout.addWidget(new FunctionItemWidget(&f1));
    mainLayout.addWidget(new FunctionItemWidget(&f2));
    mainLayout.addWidget(new FunctionItemWidget(&f3));
    mainLayout.addWidget(new FunctionItemWidget(&f4));
    mainLayout.addWidget(new FunctionItemWidget(&f5));
    mainLayout.addWidget(new FunctionItemWidget(&f6));
}

ControlPanel::ControlPanel(QList<FunctionItem *> *l, QWidget *parent)
{
    this->setLayout(&mainLayout);
    this->list = l;
    this->isNeedPrintRF = false;
    this->isNeedPrintQueryState = false;

    constructComandList();
    for(int i=0;i<l->size();i++)
    {
        FunctionItemWidget *pw = new FunctionItemWidget(l->at(i),statemachine);
        //       connect(pw,SIGNAL(signalALarm(uint8_t)),this,SLOT(slotAlarm(uint8_t)));
        connect(pw,SIGNAL(signalALarm(uint16_t)),this,SLOT(slotAlarm(uint16_t)));
        connect(pw,SIGNAL(signalLog(QString,uint8_t)),this,SIGNAL(signallog(QString,uint8_t)));
        //       connect(pw,SIGNAL(signalStatusChanged()),this,SLOT(slotQueryState()));
        connect(pw,SIGNAL(signalStatusChanged()),this,SLOT(slotReportState()));
        connect(pw,SIGNAL(signalZhihuangStatusChanged(uint8_t)), this, SLOT(slotZhihuangStatusChanged(uint8_t)));
        connect(this, SIGNAL(signalZhihuang(bool)), pw, SLOT(slotZhihuang(bool)));

        controlList.append(pw);
        mainLayout.addWidget(pw,i/4,i%4);
    }

    mainLayout.setContentsMargins(0,0,0,0);
    connect(&reportTimer,SIGNAL(timeout()),this,SLOT(slotReportState()));
    connect(&zhihuangTimer, SIGNAL(timeout()), this, SLOT(slotZhihuangControl()));

    reportTimer.start(10000);

    this->zhihuangDir = 1;
}

void ControlPanel::slotReceivedCommand(const uint8_t *command, uint8_t pos)
{
    uint16_t cmd = *((uint16_t *)(command));
    uint8_t index;
    uint8_t type = getStructType(cmd,&index);
    if(type == 0xff)
    {
        //该设备不存在指令#
#ifdef USE_BIG_EDITION
        uint16_t cmdBig = cmd;
        ToBigEndian16(&cmdBig);
        emit signallog("指令("+QString::number(cmdBig)+"---0x"+QString::number(cmdBig,16)+")"+"不存在",3);
#else
        emit signallog("指令("+QString::number(cmd)+"---0x"+QString::number(cmd,16)+")"+"不存在",3);
#endif
    }
    else if(type == 0) //公共指令
    {
        //查询指令，上报数据
        //发送状态
        if(this->isNeedPrintQueryState)
        {
            emit signallog("查询指令",2);
        }
        slotQueryState(this->isNeedPrintQueryState);
        QDEBUG <<"收到一个查询指令";
    }
    else if(type == VARTYPEALARM) //报警型
    {
        //不可能出现
        QDEBUG << "指令错误";
    }
    else
    {
        bool isSuccess =  controlList.at(index)->dealWithCommand((uint8_t *)(command+2));
        if(isSuccess)
        {
            slotQueryState();
        }
    }
}

void ControlPanel::slotReceivedCommand(const uint8_t *command, QByteArray frame)
{
    uint16_t cmd = *((uint16_t *)(command));
    uint8_t index;
    uint8_t type = getStructType(cmd,&index);
    if(type == 0xff)
    {
        //该设备不存在指令#
#ifdef USE_BIG_EDITION
        uint16_t cmdBig = cmd;
        ToBigEndian16(&cmdBig);
        emit signallog("指令("+QString::number(cmdBig)+"---0x"+QString::number(cmdBig,16)+")"+"不存在",3);
#else
        emit signallog("指令("+QString::number(cmd)+"---0x"+QString::number(cmd,16)+")"+"不存在",3);
#endif
        slotInvalid();
    }
    else if(type == 0) //公共指令
    {
        //查询指令，上报数据
        //发送状态
        if(this->isNeedPrintQueryState)
        {
            emit signallog(frame.toHex(),1);
            emit signallog("查询指令",2);
        }
        slotQueryState(this->isNeedPrintQueryState);
        QDEBUG <<"收到一个查询指令";
    }
    else if(type == VARTYPEALARM) //报警型
    {
        //不可能出现
        QDEBUG << "指令错误";
    }
    else
    {
        emit signallog(frame.toHex().toUpper(),1);
        bool isSuccess =  controlList.at(index)->dealWithCommand((uint8_t *)(command+2));
        if(isSuccess)
        {
            slotQueryState();
        }
        else
        {
            slotInvalid();
        }
    }
}

void ControlPanel::slotReceivedFrame(QByteArray frame)
{
    qDebug()<<QString(frame)<<"xxx";
    //解析帧
    if(!zhihuang)
    {
        bool uartsuccess = UserUartLinkUnpack((unsigned char *)frame.data(),frame.size());
        if(uartsuccess)
        {
            uint8_t buf[frame.length()];
            unsigned char len = getUserUartLinkMsg(buf);
            //处理 帧类型+数据区域
            qDebug()<<__FILE__<<__LINE__<<QByteArray((char *)buf,len).toHex();
            uint8_t frameType = buf[0];
            if(frameType == 0x01) //控制帧
            {
                receivedControlFrame(buf+1,len-1);
            }
            else if(frameType ==0x10)
            {
                //查询设备ID
                emit signallog("查询设备ID",2);
                QDEBUG << deviceId.toLocal8Bit();
                uint8_t src[37];
                src[0] = 0x11;
                memcpy(src+1,deviceId.toLocal8Bit().data(),36);

                int len = UserUartLinkPack((unsigned char *)sendFrame,(unsigned char *)src,37,0);
                emit signalSendReport(sendFrame,len);
                emit signallog(QByteArray((char *)sendFrame,len).toHex().toUpper(),4);
            }
            else if(frameType == 0x05)
            {
                //确认帧
                if(isNeedPrintRF)
                {
                    emit signallog(QString(frame.toHex()),1);
                    emit signallog("确认帧",2);
                }
            }
            else if(frameType == 0x09)
            {
                emit signallog("停止报警帧",2);
                for(int i = 0 ;i< commandTypeList.size();i++ )
                {
                    if(commandTypeList.at(i) == 0x04)
                    {
                        this->controlList.at(i)->stopAlarm();
                    }
                }
            }
            else
            {
                //出错
                slotInvalid();
            }
        }
    }
    else
    {
        uint8_t data_buf[ZHIHUANG_BUF_LEN]={0};
        uint8_t data_len = 0;

        data_len = ZhihuangUnpack(data_buf, (unsigned char *)frame.data(),frame.size());
        if(data_len != 0)
        {
            qDebug()<<QByteArray((char *)data_buf, data_len).toHex();
            qDebug()<<QString::number(data_len);

            revCmdFrame((uint8_t *)&data_buf,data_len);
        }
    }
}

void ControlPanel::slotZhihuangControl()
{
    uint8_t value = 0;
    value = controlList.at(4)->ZhihuangControl(this->zhihuangControlPos);     //位置

    if(value == this->zhihuangControlPos)
    {
        zhihuangTimer.stop();
    }
}

void ControlPanel::slotZhihuangStatusChanged(uint8_t datatype)
{

}

void ControlPanel::slotReportState()
{
    uint8_t srcData[stateLength+1];
    uint8_t len = 0;

    if(this->zhihuang == false)
    {
        srcData[0] = 0x07;//汇报帧
        memcpy((uint8_t *)(srcData+1),statemachine,stateLength);

        len = UserUartLinkPack((unsigned char *)(sendFrame), (unsigned char *)(srcData),  stateLength+1,0);
        QDEBUG << stateLength;
        QDEBUG<< QByteArray((char *)sendFrame);
        //通知mainwindow将数据发送出去;
        emit signalSendReport(sendFrame,len);
        if(isNeedPrintRF)
        {
            emit signallog(QByteArray((char *)sendFrame,len).toHex().toUpper(),4);
        }
    }
//    else        // 智皇窗帘
//    {
//        uint8_t srclen = 0;

//        srcData[srclen++] = 0;
//        srcData[srclen++] = 0x03;
//        srcData[srclen++] = statemachine[5];       // 窗帘位置H
//        srcData[srclen++] = statemachine[6];       // 窗帘位置L
//        srcData[srclen++] = statemachine[4];       // 窗帘设置L

//        len = zhiHuangPack((uint8_t*)(sendFrame), srcData, srclen);
//        QDEBUG << srclen;
//    }

//    QDEBUG<< QByteArray((char *)sendFrame);
//    //通知mainwindow将数据发送出去;
//    emit signalSendReport(sendFrame,len);
//    if(isNeedPrintRF)
//    {
//        emit signallog(QByteArray((char *)sendFrame,len).toHex().toUpper(),4);
//    }
}

void ControlPanel::slotQueryState()
{
    uint8_t srcData[stateLength+1];
    uint8_t len = 0;
    if(this->zhihuang == false)
    {
        srcData[0] = 0x02;//状态帧
        memcpy((uint8_t *)(srcData+1),statemachine,stateLength);

        len = UserUartLinkPack((unsigned char *)(sendFrame),
                                       (unsigned char *)(srcData),
                                       stateLength+1,0);
        QDEBUG << stateLength;
    }
    else        // 智皇窗帘
    {
        uint8_t srclen = 0;

        srcData[srclen++] = 0;
        srcData[srclen++] = 0x03;
        srcData[srclen++] = statemachine[6];       // 窗帘位置H
        srcData[srclen++] = statemachine[5];       // 窗帘位置L
        srcData[srclen++] = statemachine[3];       // 窗帘设置L

        len = zhiHuangPack((uint8_t*)(sendFrame), srcData, srclen);
        QDEBUG << srclen;
    }

    QDEBUG<< QByteArray((char *)sendFrame);
    //通知mainwindow将数据发送出去;

//    emit signalSendReport(sendFrame,len);
    emit signallog(QByteArray((char *)sendFrame,len).toHex().toUpper(),4);
}

void ControlPanel::slotQueryDIR(bool b)
{
    uint8_t srcData[stateLength+1];
    uint8_t len = 0;
    uint8_t srclen = 0;

    srcData[srclen++] = 0;
    srcData[srclen++] = 0x05;
    srcData[srclen++] = 0x05;
    srcData[srclen++] = this->zhihuangDir;       // 电机方向固定为0
    srcData[srclen++] = 0;

    len = zhiHuangPack((uint8_t*)(sendFrame), srcData, srclen);
    QDEBUG << srclen;

    QDEBUG<< QByteArray((char *)sendFrame);
    //通知mainwindow将数据发送出去;

    emit signalSendReport(sendFrame,len);
    if(b)
    {
        emit signallog(QByteArray((char *)sendFrame,len).toHex().toUpper(),4);
    }
}

void ControlPanel::slotQueryState(bool b)
{
    uint8_t srcData[stateLength+1];
    uint8_t len = 0;

    if(false == this->zhihuang)
    {
        srcData[0] = 0x02;//状态帧
        memcpy((uint8_t *)(srcData+1),statemachine,stateLength);

        len = UserUartLinkPack((unsigned char *)(sendFrame),
                                       (unsigned char *)(srcData),
                                       stateLength+1,0);
        QDEBUG << stateLength;
    }
    else        // 智皇窗帘
    {
        uint8_t srclen = 0;

        srcData[srclen++] = 0;
        srcData[srclen++] = 0x03;
        srcData[srclen++] = 0;
        srcData[srclen++] = 0;
        srcData[srclen++] = statemachine[6];       // 窗帘位置L

        len = zhiHuangPack((uint8_t*)(sendFrame), srcData, srclen);
        QDEBUG << srclen;
    }
    QDEBUG<< QByteArray((char *)sendFrame);
    //通知mainwindow将数据发送出去;

    emit signalSendReport(sendFrame,len);
    if(b)
    {
        emit signallog(QByteArray((char *)sendFrame,len).toHex().toUpper(),4);
    }
}

void ControlPanel::slotReportSetPos(bool b)
{
    uint8_t srcData[stateLength+1];
    uint8_t len = 0;
    uint8_t srclen = 0;

    srcData[srclen++] = 0;
    srcData[srclen++] = 0x04;
    srcData[srclen++] = statemachine[4];       // 窗帘设置位置L

    len = zhiHuangPack((uint8_t*)(sendFrame), srcData, srclen);
    QDEBUG << srclen;

    QDEBUG<< QByteArray((char *)sendFrame);
    //通知mainwindow将数据发送出去;

    emit signalSendReport(sendFrame,len);
    if(b)
    {
        emit signallog(QByteArray((char *)sendFrame,len).toHex().toUpper(),4);
    }
}

void ControlPanel::slotInvalid()
{
    QDEBUG << "回复无效帧";
    char invalid[] = {0x03,0x00,0x00,0x00,0x01};

    uint8_t len =UserUartLinkPack((unsigned char*)sendFrame,(unsigned char*)invalid,5,0);
    emit signalSendReport(sendFrame,len);
}

void ControlPanel::slotAlarm(uint16_t alarmCode)
{
    char alarmData[]={0x04,0x00,0x00,0x00,0x00,0x00,0x00};
    ExChangeEdition(&alarmCode);
    *((uint16_t *)(alarmData+1)) = alarmCode;

    uint8_t len =UserUartLinkPack((unsigned char*)sendFrame,(unsigned char*)alarmData,7,0);
    QDEBUG << len;
    emit signalSendReport(sendFrame,len);
    emit signallog(QByteArray((char *)sendFrame,len).toHex().toUpper(),len);
}

void ControlPanel::slotIsNeedPrintRF(bool i)
{
    this->isNeedPrintRF = i;
}

void ControlPanel::slotIsNeedPrintQueryState(bool i)
{
    this->isNeedPrintQueryState = i;
}

void ControlPanel::slotIsZhihuang(bool i)
{
    this->zhihuang = i;
    emit signalZhihuang(i);
}

void ControlPanel::slotShortClicked()
{
    uint8_t srcData[stateLength+1];
    uint8_t len = 0;
    uint8_t srclen = 0;

    srcData[srclen++] = 0;
    srcData[srclen++] = 0x01;
    srcData[srclen++] = 0xF0;
    srcData[srclen++] = 0x01;
    srcData[srclen++] = 0x70;       // 窗帘位置L

    len = zhiHuangPack((uint8_t*)(sendFrame), srcData, srclen);
    QDEBUG << srclen;

    QDEBUG<< QByteArray((char *)sendFrame);
    //通知mainwindow将数据发送出去;

    emit signalSendReport(sendFrame,len);
    if(this->isNeedPrintQueryState)
    {
        emit signallog(QByteArray((char *)sendFrame,len).toHex().toUpper(),4);
    }
}
void ControlPanel::slotLongClicked()
{
    uint8_t srcData[stateLength+1];
    uint8_t len = 0;
    uint8_t srclen = 0;

    srcData[srclen++] = 0;
    srcData[srclen++] = 0x01;
    srcData[srclen++] = 0xE0;
    srcData[srclen++] = 0x01;
    srcData[srclen++] = 0x70;       // 窗帘位置L

    len = zhiHuangPack((uint8_t*)(sendFrame), srcData, srclen);
    QDEBUG << srclen;

    QDEBUG<< QByteArray((char *)sendFrame);
    //通知mainwindow将数据发送出去;

    emit signalSendReport(sendFrame,len);
    if(this->isNeedPrintQueryState)
    {
        emit signallog(QByteArray((char *)sendFrame,len).toHex().toUpper(),4);
    }
}

void ControlPanel::slotReportTimeChanged(int s)
{
    this->reportTimer.stop();
    this->reportTimer.start(s*1000);
}

#define ZHIHUANG_PROTOCOL_TYPE_CONTROL   0x11     // 控制
#define ZHIHUANG_PROTOCOL_TYPE_QUERY     0x15     // 查询

#define ZHIHUANG_PROTOCOL_CMD_OPEN      1       // 开
#define ZHIHUANG_PROTOCOL_CMD_CLOSE     2       // 关
#define ZHIHUANG_PROTOCOL_CMD_STOP      3       // 停
#define ZHIHUANG_PROTOCOL_CMD_POS       7       // 百分比位置
#define ZHIHUANG_PROTOCOL_CMD_QUERY_POS 1       // 查询位置
#define ZHIHUANG_PROTOCOL_CMD_QUERY_DIR 5       // 查询方向

//#define ZHIHUANG_CONTROL_STOP           0       //窗帘停
//#define ZHIHUANG_CONTROL_START          1       //窗帘启动

//#define ZHIHUANG_CONTROL_OPEN           1       // 窗帘开
//#define ZHIHUANG_CONTROL_CLOSE          2       // 窗帘关

void ControlPanel::revCmdFrame(uint8_t * data, uint8_t len)
{
    uint8_t index = 0;
    uint8_t buf[len] = {0};
    uint8_t open = 0;
    uint8_t close = 0;
    uint8_t stop = 0;

    if(ZHIHUANG_PROTOCOL_TYPE_CONTROL == data[1])
    {
        if(ZHIHUANG_PROTOCOL_CMD_OPEN == data[2])
        {
            index = 0;
            buf[0] = open;
            open++;
            open &= 0x01;
            slotZhihuangControl();
            zhihuangTimer.start(500);
            if(false == this->zhihuangDir)
            {
                this->zhihuangControlPos = 10;
            }
            else
            {
                this->zhihuangControlPos = 10;
            }
        }

        else if(ZHIHUANG_PROTOCOL_CMD_CLOSE == data[2])
        {
            index = 1;
            buf[0] = close;
            close++;
            close &= 0x01;
            slotZhihuangControl();
            zhihuangTimer.start(500);
            if(false == this->zhihuangDir)
            {
                this->zhihuangControlPos = 0;
            }
            else
            {
                this->zhihuangControlPos = 0;
            }
        }

        else if(ZHIHUANG_PROTOCOL_CMD_STOP == data[2])
        {
            index = 2;
            buf[0] = stop;
            stop++;
            stop &= 0x01;
            zhihuangTimer.stop();
        }

        else if(ZHIHUANG_PROTOCOL_CMD_POS == data[2])
        {
            index = 3;
            buf[0] = 0;
            buf[1] = data[3];
            slotZhihuangControl();
            zhihuangTimer.start(500);
            this->zhihuangControlPos = data[3];
//            slotReportSetPos(this->isNeedPrintQueryState);
        }
        else
        {
            // 错误
        }
        slotQueryState(this->isNeedPrintQueryState);
        emit signallog(QByteArray((char *)data,len).toHex().toUpper(),1);
        controlList.at(index)->dealWithCommand(buf);
    }
    else if(ZHIHUANG_PROTOCOL_TYPE_QUERY == data[1])
    {
        // 查询指令，上报数据
        // 发送状态
        if(ZHIHUANG_PROTOCOL_CMD_QUERY_POS == data[2])
        {
            if(this->isNeedPrintQueryState)
            {
                emit signallog(QByteArray((char *)data,len).toHex(),1);
                emit signallog("查询状态",2);
            }
            slotQueryState(this->isNeedPrintQueryState);
        }
        else if(ZHIHUANG_PROTOCOL_CMD_QUERY_DIR == data[2])
        {
            if(this->isNeedPrintQueryState)
            {
                emit signallog(QByteArray((char *)data,len).toHex(),1);
                emit signallog("查询方向",2);
            }
            slotQueryDIR(this->isNeedPrintQueryState);
        }
        else
        {
            //错误
        }
        QDEBUG <<"收到一个查询指令";
    }
}

void ControlPanel::receivedControlFrame(uint8_t * frame, int len)
{
    qDebug()<<QByteArray((char *)frame,len).toHex();
    uint16_t cmd = *((uint16_t *)(frame + 2));
    uint16_t functionNumber =*((uint16_t*)(frame));
    ToBigEndian16(&cmd);
    ToBigEndian16(&functionNumber);

    uint8_t index = getIndexFromFnandVarindex(functionNumber,cmd);
    QDEBUG << index;
    if(index == 0xff)
    {
        //该设备不存在指令#
#ifdef USE_BIG_EDITION
        uint16_t cmdBig = cmd;
        ToBigEndian16(&cmdBig);
        emit signallog("指令("+QString::number(cmdBig)+"---0x"+QString::number(cmdBig,16)+")"+"不存在",3);
#else
        emit signallog("指令("+QString::number(cmd)+"---0x"+QString::number(cmd,16)+")"+"不存在",3);
#endif
        slotInvalid();
    }
    else if(index == 0xfe) //公共指令
    {
        //查询指令，上报数据
        //发送状态
        if(this->isNeedPrintQueryState)
        {
            emit signallog(QByteArray((char *)frame,len).toHex(),1);
            emit signallog("查询指令",2);
        }
        slotQueryState(this->isNeedPrintQueryState);
        QDEBUG <<"收到一个查询指令";
    }
    else
    {
        emit signallog(QByteArray((char *)frame,len).toHex().toUpper(),1);
        bool isSuccess =  controlList.at(index)->dealWithCommand((uint8_t *)(frame+4));
        if(isSuccess)
        {
            slotQueryState();
        }
        else
        {
            slotInvalid();
        }
    }
}

uint8_t ControlPanel::getIndexFromFnandVarindex(uint16_t functionNumber, uint16_t varIndex)
{
    qDebug()<<functionNumberList;
    qDebug()<<commandList;
    qDebug()<<"functionNumber"<<functionNumber<<",VarIndex"<<varIndex;
    if(varIndex == 0x11)
    {
        return 0xfe; //查询指令
    }

    for(int i=0;i<commandList.size();i++)
    {
        if(functionNumber == functionNumberList.at(i) && varIndex == commandList.at(i))
        {
            return i;
        }
    }
    QDEBUG <<"不存在指令";
    return 0xff;
}
QString ControlPanel::getDeviceId() const
{
    return deviceId;
}

void ControlPanel::setDeviceId(const QString &value)
{
    deviceId = value;
    deviceId.toLatin1().data()[35]=0x00;
}


void ControlPanel::constructComandList()
{
    this->stateLength = 0;
    uint8_t lastPos = 0;
    for(int i=0;i<list->size();i++ )
    {
        FunctionItem *f = list->at(i);
        if(f->getVarType()!=VARTYPEALARM)
        {
            lastPos = f->getDataPos();
            stateLength = lastPos + f->getDatalength();
        }
        else
        {
            /////////////////////////////////////////////
            int alarmcount = f->getVarDesc().pVarDescAlarm->getAlarmCode().length();
            if(alarmcount % 8 == 0)
            {
                stateLength += alarmcount/8;
            }
            else
            {
                stateLength += alarmcount/8;
                stateLength +=1;
            }
        }
        commandList.append(f->getVarIndex());
        functionNumberList.append(f->getFunctionNumber());

        commandTypeList.append(f->getVarType());
        commandPos.append(f->getDataPos());
    }
    this->statemachine = new uint8_t [stateLength +1];
    memset(statemachine,0,stateLength+1);
    //    statemachine[stateLength] = 0;
    ///////////////////////////////////////
    //    this->statemachine[stateLength] = 0xff;
    this->sendFrame = new uint8_t [stateLength+30];
}

uint8_t ControlPanel::getStructType(uint16_t cmd, uint8_t *index)
{

#ifdef USE_BIG_EDITION  //如果需要使用打断
    ToBigEndian16(&cmd);//将指令进行大小端替换再进行比较
#endif
    if(cmd == 11)
    {

        return 0;
    }
    for(int i=0;i<commandList.size();i++)
    {
        if(cmd == commandList.at(i))
        {
            *index = i;
            return commandTypeList.at(i);
        }
    }
    QDEBUG <<"不存在指令";
    return 0xff;
}

uint8_t ControlPanel::getStructTypefromFN(uint16_t fn, uint8_t *index)
{
    for(int i=0;i<functionNumberList.size();i++)
    {
        if(fn == commandList.at(i))
        {
            *index = i;
            return commandTypeList.at(i);
        }
    }

    return 0xff;
}
