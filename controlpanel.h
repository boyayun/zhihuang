#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QObject>
#include <QWidget>
#include"define.h"
#include"functionitemwidget.h"
#include"UserUartLink.h"
#include<QTimer>


typedef struct _frame_struct
{
    uint16_t frameHead;
    uint8_t frameControl;
    uint8_t frameLength;
    uint8_t frameType;
}*pFrameStruct;

class ControlPanel : public QWidget
{
    Q_OBJECT
public:
    explicit ControlPanel(QWidget *parent = 0);
    explicit ControlPanel(QList<FunctionItem *>*l,QWidget *parent = 0);

    QString getDeviceId() const;
    void setDeviceId(const QString &value);

signals:
    void signalSendReport(void *,uint8_t length);
    void signallog(QString log,uint8_t type);
    void signalZhihuang(bool);

public slots:
    void slotReceivedCommand(const uint8_t *command,uint8_t pos);
    void slotReceivedCommand(const uint8_t *command,QByteArray frame);
    void slotReceivedFrame(QByteArray);

    void slotReportState(); //汇报帧
    void slotQueryState();  //状态帧
    void slotQueryDIR(bool b);
    void slotQueryState(bool);
    void slotZhihuangStatusChanged(uint8_t);

    void slotInvalid();

    void slotAlarm(uint16_t alarmCode);       //报警帧
    void slotIsNeedPrintRF(bool i);
    void slotIsNeedPrintQueryState(bool i);
    void slotIsZhihuang(bool i);
    void slotShortClicked();
    void slotLongClicked();
    void slotReportSetPos(bool);

    void slotReportTimeChanged(int s);
    void slotZhihuangControl();
private:
    QList<FunctionItemWidget *> controlList;
    QGridLayout mainLayout;
    QList<uint16_t>commandList;
    QList<uint16_t>functionNumberList;
    QList<uint8_t>commandTypeList;
    QList<uint8_t>commandPos;
    QList<FunctionItem *>*list;
    uint8_t stateLength;
    QTimer reportTimer;
    QTimer zhihuangTimer;
    QString deviceId;
    
    void revCmdFrame(uint8_t *, uint8_t len);
    void receivedControlFrame(uint8_t *,int len);
    uint8_t getIndexFromFnandVarindex(uint16_t functionNumber,uint16_t varIndex);

   // uint8_t

    void constructComandList();
    uint8_t getStructType(uint16_t cmd, uint8_t *index);
    uint8_t getStructTypefromFN(uint16_t fn,uint8_t *index);//从functionNumber得到功能类型;
    uint8_t *statemachine;
    uint8_t *sendFrame;
    bool isNeedPrintRF;//是否需要打印汇报帧
    bool isNeedPrintQueryState;
    bool zhihuang;   //智皇
    uint8_t zhihuangControlPos = 0;
    bool zhihuangDir;      //电机方向
};

#endif // CONTROLPANEL_H
