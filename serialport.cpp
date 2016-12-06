#include "serialport.h"

SerialPort::SerialPort(uint8_t *receivedFrame,uint8_t *sendFrame, QObject *parent) : QObject(parent)
{
    frameStr = "<font color =\"#ff0000\">";
    frameStrreceive = "<font color =\"#ff0000\">";
    this->frame = receivedFrame;
    this->sendframe = sendFrame;
    preWord = '0';
    isInConstruction = false;
    serialPort = new QSerialPort();
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
//    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->setFlowControl(QSerialPort::SoftwareControl);
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(readData()));
    connect(this,SIGNAL(receiveOver()),this,SLOT(constructFrame()));
    //emit signalOpenState( serialPort->open(QSerialPort::ReadWrite));
    qDebug()<<"COM初始化成功 "<<endl;


}

SerialPort::SerialPort()
{
    serialPort = new QSerialPort();
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(readData()));
    connect(&serialTimer,SIGNAL(timeout()),this,SLOT(slotreceiveTimeout()));
}

void SerialPort::write(uint8_t *data, uint8_t len)
{

  qDebug()<<len;
  qDebug()<<"ssssssssssssssss";
  this->serialPort->write((char *)data,len);
}

void SerialPort::readData()
{
    receivedFrame += serialPort->readAll();
   //emit signalReceivedCommand(serialPort->readAll());
    serialTimer.start(100);
}

void SerialPort::constructFrame()
{
   // qDebug()<<"aha";
    while(!receiveBuf.isEmpty())
    {
        if(isInConstruction == false)
        {
            qDebug()<<"没有进入创建模式";
            thisWord = receiveBuf.dequeue();
            qDebug()<<"this world:"<<thisWord<<"...preworld:"<<preWord;
            if(0xf5 ==thisWord )
            {
                qDebug()<<"Yes"<<endl;
            }
            if(0xf5== preWord&& 0xf5 == thisWord)
            {
                qDebug()<<"Start to construct the frame~";
                isInConstruction = true;
                constructionNum = 2; //组到第二个字节

                frame[0]=0xf5;
                frame[1]=0xf5;
            }
            preWord = thisWord;
        }
        else
        {
           qDebug()<<"开始组第   "<<constructionNum<<"个字节 ";
            preWord = thisWord;
            thisWord = receiveBuf.dequeue();

            frame[constructionNum]=thisWord;
            if(constructionNum == 3)
            {
                qDebug()<<"该包长度为  "<<frameLength;
                frameLength =thisWord;
            }

            constructionNum++;
            if(constructionNum>50)
            {
                isInConstruction =false;
                //防止帧错误无限组帧的情况发生
            }
            if(constructionNum == frameLength + 4)
            {
                qDebug()<<"Construction Over, we need to send it!";
                for (int i=0; i< frameLength +4;i++)
                {
                    qDebug("%x",frame[i]);
                }
               //组帧完毕，通知处理帧
                isInConstruction = false;


              //  emit constrictFrameOver();
                frameStrreceive = "<font color =\"#0099ff\">";
                for(uint8_t i = 0; i<frameLength+4; i++)
                {
                // frameStrreceive = frameStrreceive + QString::number((uint)frame[i],16),+" " ;
                 //if(i==4)
                    if (QString::number((uint)frame[i],16).length()==1)
                    {
                        frameStrreceive = frameStrreceive +"0"+QString::number((uint)frame[i],16);
                    }
                    else
                    {

                      frameStrreceive = frameStrreceive + QString::number((uint)frame[i],16) ;
                    }
                }
                frameStrreceive = frameStrreceive + "</font>";
                emit signalLog("");
                emit signalLog(QTime::currentTime().toString()+"<<<<<<<<收到一个帧");
                emit signalLog(frameStrreceive);

                emit constrictFrameOver();

            }
        }
    }
}

void SerialPort::sendFrame()
{
    uint8_t length = sendframe[3]+4;
    qint64 l= (qint64)length;
    serialPort->write((char *)this->sendframe,l);
    //frameStr = "\0";
    for(uint8_t i = 0; i<length; i++)
    {
        if (QString::number((uint)sendframe[i],16).length()==1)
        {
            frameStr = frameStr +"0"+QString::number((uint)sendframe[i],16);
        }
        else
        {

          frameStr = frameStr + QString::number((uint)sendframe[i],16) ;
        }
    }
    frameStr = frameStr + "</font>";

    qDebug()<<"帧："<<frameStr;
    qDebug()<<"WRITE TO SERIALPORT";
   // emit signalLog("");

    switch (sendframe[4])//获得帧类型
    {
        case 1:
            emit signalLog(QTime::currentTime().toString()+">>>>>>>>>>>发送一个控制帧     ");

        break;
    case 2:
        emit signalLog(QTime::currentTime().toString()+">>>>>>>>>>>发送一个状态帧     ");
        break;
    case 3:
        emit signalLog(QTime::currentTime().toString()+">>>>>>>>>>>发送一个无效帧");
        break;
    case 4:
        emit signalLog(QTime::currentTime().toString()+">>>>>>>>>>发送一个报警帧");
        break;
    case 5:
        emit signalLog(QTime::currentTime().toString()+">>>>>>>>>发送一个ACK");

    }
    emit signalLog(frameStr);
     frameStr = "<font color =\"#ff0000\">";
   // for (uint8_t i=0;i<32)
}

void SerialPort::setComName(QString portname)
{
    qDebug()<<"收到portname"<<portname;

    serialPort->close();
    if(portname !="请选择串口")
    {
    serialPort->setPortName(portname);
   //qDebug()<<serialPort->open(QSerialPort::ReadWrite);

      emit signalOpenState( serialPort->open(QSerialPort::ReadWrite));
    }
}

void SerialPort::slotreceiveTimeout()
{
    serialTimer.stop();

    emit signalReceivedCommand(receivedFrame);
    qDebug()<<QString(receivedFrame.toHex());
    receivedFrame.clear();
}


