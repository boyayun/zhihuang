#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"controlpanel.h"
#include"define.h"
#include "detectxml.h"
#include <QFileDialog>
#include <QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "serialport.h"
#include"about.h"
#include<QMessageBox>
#include<QDesktopServices>

#define CORRECTINSTRUCT(l) "<font color=\"blue\">"+QString("帧含义：")+":"+l+"</font>"
#define INCORRECTINSTRUCT(l) "<font color=\"red\">"+QTime::currentTime().toString()+":"+l+"</font>"
#define RECEIVEDFRAME(l) "<font color=\"black\"><b>"+QString("收到帧：")+QTime::currentTime().toString()+":"+l+"</b></font>"
#define SENDFRAME(l) "<font color=\"blue\"><b>"+QString("发送帧：")+QTime::currentTime().toString()+":"+l+"</b></font>"
#define ALARMFRAME(l) "<font color=\"gray\"><b>"+QString("报警帧：")+QTime::currentTime().toString()+":"+l+"</b></font>"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QList<FunctionItem *> list;
    ControlPanel *controlPanel;
    SerialPort s;

private slots:
    void slotOpenXmlFile(bool);
    void slotUpdateSerialInfo();
//    void slotReceivedframe(QByteArray );
    void slotSendReport(void * add,uint8_t len);
//    void on_serialCombox_currentIndexChanged(const QString &arg1);
    void slotPrintReceiveLog(QString log);
//    void slotPrintSendLog(QString log);
    void slotPrintLog(QString log,uint8_t type);
    void on_pushButton_clicked();

    void on_spinBox_valueChanged(int arg1);

    void on_checkBox_clicked(bool checked);
    void slotAbout(bool);

    void on_pushButton_2_clicked();

    void on_zhihuangShortButton_clicked();

    void on_zhihuangLongButton_clicked();

    void slotSerialState(bool);

    void on_serialCombox_activated(int index);

    void on_checkBox_2_clicked(bool checked);

    void on_checkBoxZhihuang_clicked(bool checked);

    void slotHelp(bool);

    void on_ResetpushButton_clicked();

//    void slot1(bool checked);

signals:
    void signalLog(QString log);
    void signalReportTimeChanged(int);
    void signalIsNeedPrintRF(bool); //汇报帧
    void signalIsNeedPrintQueryState(bool); //查询状态帧
    void signalIsZhihuang(bool);
    void signalZhihuangShortClicked();
    void signalZhihuangLongClicked();
};

#endif // MAINWINDOW_H
