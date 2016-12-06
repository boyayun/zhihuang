#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <QObject>
#include<QtSerialPort/QSerialPort>
#include<QByteArray>
#include<QDebug>
#include<stdint.h>
#include<QQueue>
#include<QTime>
#include<QTimer>

class SerialPort : public QObject
{
    Q_OBJECT
public:
    explicit SerialPort(uint8_t *receivedFrame, uint8_t *sendFrame,QObject *parent = 0);
    SerialPort();
    void write(uint8_t *data,uint8_t lignth);
private:
    QSerialPort *serialPort;
    char *buf;
    QQueue <uint8_t> receiveBuf;
  uint8_t preWord;
    uint8_t thisWord;
    uint8_t frameLength;
    uint8_t constructionNum;
    bool isInConstruction;//是否处于组帧状态
    uint8_t *frame;
    uint8_t *sendframe;
    QString frameStr;
    QString frameStrreceive;
    QTime currentTime;

    ////////////////////////
    QTimer serialTimer;
    bool isTimeout;
    QByteArray receivedFrame;


signals:
     void receiveOver();
     void constrictFrameOver();
     void signalOpenState(bool);
     void signalLog(QString);


     void signalReceivedCommand(QByteArray );

public slots:
    void readData();
    void constructFrame();
    void sendFrame();
    void setComName(QString portname);
private slots:
    void slotreceiveTimeout();

};

#endif // SERIALPORT_H
