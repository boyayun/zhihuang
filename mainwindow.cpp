#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->ui->spinBox->setValue(10);
    this->setWindowTitle("设备模拟器");
    controlPanel = NULL;
    slotUpdateSerialInfo();
    this->setGeometry(100,100,800,600);
    connect(this->ui->actionOpen,SIGNAL(triggered(bool)),this,SLOT(slotOpenXmlFile(bool)));
    connect(&s,SIGNAL(signalOpenState(bool)),this,SLOT(slotSerialState(bool)));
    connect(this->ui->actionVersion,SIGNAL(triggered(bool)),this,SLOT(slotAbout(bool)));
    connect(this->ui->actionHelp,SIGNAL(triggered(bool)),this,SLOT(slotHelp(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotOpenXmlFile(bool)
{
    QDEBUG << "打开一个文件";
    QString filePath = QFileDialog::getOpenFileName(this,"打开设备描述文件",QDir::currentPath(),"Xml(*.xml)");
    QDEBUG << filePath;
    if(!filePath.isEmpty())
    {
        DetectXml d(filePath,&list);
        this->setWindowTitle(d.getDeviceName());
        this->controlPanel = new ControlPanel(&list);
        controlPanel->setDeviceId(d.getDeviceID());
        connect(&s,SIGNAL(signalReceivedCommand(QByteArray)),controlPanel,SLOT(slotReceivedFrame(QByteArray)));
        connect(controlPanel,SIGNAL(signalSendReport(void*,uint8_t)),this,SLOT(slotSendReport(void*,uint8_t)));
        connect(controlPanel,SIGNAL(signallog(QString,uint8_t)),this,SLOT(slotPrintLog(QString,uint8_t)));
        connect(this,SIGNAL(signalReportTimeChanged(int)),controlPanel,SLOT(slotReportTimeChanged(int)));
        connect(this,SIGNAL(signalIsNeedPrintRF(bool)),controlPanel,SLOT(slotIsNeedPrintRF(bool)));
        connect(this,SIGNAL(signalIsNeedPrintQueryState(bool)),controlPanel,SLOT(slotIsNeedPrintQueryState(bool)));
        connect(this, SIGNAL(signalIsZhihuang(bool)), controlPanel, SLOT(slotIsZhihuang(bool)));
        connect(this, SIGNAL(signalZhihuangShortClicked()), controlPanel, SLOT(slotShortClicked()));
        connect(this, SIGNAL(signalZhihuangLongClicked()), controlPanel, SLOT(slotLongClicked()));

        this->ui->checkBoxZhihuang->setEnabled(true);
        this->ui->scrollArea->setWidget(controlPanel);
    }
}

void MainWindow::slotUpdateSerialInfo()
{
    this->ui->serialCombox->clear();
    this->ui->serialCombox->addItem("请选择串口");
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        if(!info.isBusy())
        {
            this->ui->serialCombox->addItem(info.portName());
        }
    }
}

void MainWindow::slotSendReport(void *add, uint8_t len)
{
   QDEBUG<<len;
   s.write((uint8_t *)add,len);
}

void MainWindow::slotPrintReceiveLog(QString log)
{
    this->ui->receiveDisplay->append(log);
}

void MainWindow::slotPrintLog(QString log, uint8_t type)
{
   this->ui->receiveDisplay->textCursor().movePosition(QTextCursor::End);
   if(type == 1)
   {
       this->ui->receiveDisplay->append(RECEIVEDFRAME(log));
   }
   else if(type ==2)
   {
       this->ui->receiveDisplay->append(CORRECTINSTRUCT(log));
   }
   else if(type == 3)
   {
       this->ui->receiveDisplay->append(INCORRECTINSTRUCT(log));
   }
   else if(type == 4)
   {
       this->ui->receiveDisplay->append(SENDFRAME(log));
   }
   else if(type == 5)
   {
       this->ui->receiveDisplay->append(ALARMFRAME(log));
   }
   else if(type == 6)
   {
       this->ui->receiveDisplay->append(QTime::currentTime().toString()+":"+log);
   }
}


void MainWindow::on_pushButton_clicked()
{
   this->ui->receiveDisplay->clear();
}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
   qDebug()<<arg1;
   emit signalReportTimeChanged(arg1);
}

void MainWindow::on_checkBox_clicked(bool checked)
{
    emit signalIsNeedPrintRF(checked);
}

void MainWindow::slotAbout(bool)
{
    About a;
    a.setEnabled(true);
    a.exec();

}

void MainWindow::on_pushButton_2_clicked()
{
    slotUpdateSerialInfo();
}

void MainWindow::on_zhihuangShortButton_clicked()
{
    emit signalZhihuangShortClicked();
}

void MainWindow::on_zhihuangLongButton_clicked()
{
    emit signalZhihuangLongClicked();
}

void MainWindow::slotSerialState(bool b)
{
   QDEBUG << b;
   if(!b)
   {
       QMessageBox::information(this,"串口","打开失败",0);
       this->ui->serialCombox->setCurrentIndex(0);
   }
}

void MainWindow::on_serialCombox_activated(int index)
{
    s.setComName(this->ui->serialCombox->itemText(index));
}

void MainWindow::on_checkBox_2_clicked(bool checked)
{
    emit signalIsNeedPrintQueryState(checked);
}

void MainWindow::on_checkBoxZhihuang_clicked(bool checked)
{
    emit signalIsZhihuang(checked);

    if(checked)
    {
        this->ui->zhihuangLongButton->setEnabled(true);
        this->ui->zhihuangShortButton->setEnabled(true);
    }
    else
    {
        this->ui->zhihuangLongButton->setEnabled(false);
        this->ui->zhihuangShortButton->setEnabled(false);
    }
}

void MainWindow::slotHelp(bool)
{
    qDebug()<<"xxx";
    QDesktopServices::openUrl(QUrl("help.html"));
}

void MainWindow::on_ResetpushButton_clicked()
{
    int ret = QMessageBox::warning(this,"复位","该操作会复位WIFI模块，需要重新进行配网",
                                   QMessageBox::Yes|QMessageBox::Cancel);
    if(ret == QMessageBox::Yes)
    {

        uint8_t  resetCommand[] = {0xf5,0xf5,0x00,0x03,0xac,0x2a,0xf7};
        s.write(resetCommand,7);
    }
}
