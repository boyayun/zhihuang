#include "mainwindow.h"
#include <QApplication>
#include "define.h"
#include <QDate>
#include "functionitemwidget.h"
#include"controlpanel.h"
#include "detectxml.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    uint16_t v = 0x0001;
    QDEBUG <<v;
    ToBigEndian16(&v);
    QDEBUG <<v;
    MainWindow w;
    w.show();
    //FunctionItem f;
   /* QList<FunctionItem *>functionList;
    for(int i=1;i<=6;i++)
    {
        FunctionItem *fi = new FunctionItem();
        if(i==1)
        {
            VarDesc v;
            VarDescCmd vc;

            QList<QString> q;
            q.append("开机");
            q.append("关机");
            QList<uint8_t> qi;
            qi.append(1);
            qi.append(0);
            vc.setDisItem(q);
            vc.setItem(qi);
            v.pVarDescCmd = &vc;
            fi->setVarDesc(*(v.pVarDescCmd));
        }
        if(i==VARTYPENUMBER)
        {
            VarDesc v;
            VarDescNumber vn;
            vn.setMax(1000);
            vn.setMin(0);
            vn.setScalling(10);
            v.pVarDescNumber = &vn;
            qDebug()<<vn.getMax();
            fi->setVarDesc(*(v.pVarDescNumber));
        }
        if(i == VARTYPEENUMBERATE)
        {
            VarDesc v;
            VarDescEnumberate ve;
            QList<QString> ql;
            ql.append("第一个选项");
            ql.append("第二个选项");
            ql.append("第三个选项");
            QList<uint8_t> qi;
            qi.append(1);
            qi.append(2);
            qi.append(3);
            ve.setDisItem(ql);
            ve.setItem(qi);
            v.pVarDescEnumberate = &ve;
            fi->setVarDesc(*(v.pVarDescEnumberate));
        }
        if(i == VARTYPEALARM)
        {
            VarDesc v;
            VarDescAlarm va;
            QList<QString> qString;
            qString.append("第一个报警");
            qString.append("第二个报警");
            qString.append("第三个报警");
            va.setAlarmString(qString);
            QList<uint8_t> qCode;
            qCode.append(1);
            qCode.append(2);
            qCode.append(3);
            va.setAlarmCode(qCode);
            QList<uint8_t> qClass;
            qClass.append(1);
            qClass.append(2);
            qClass.append(3);
            va.setAlarmClass(qClass);
            v.pVarDescAlarm = &va;
            fi->setVarDesc(*(v.pVarDescAlarm));
        }
        if( i == VARTYPEDATE)
        {
            VarDesc v;
            VarDescDate vd;
            v.pVarDescDate = &vd;
            QDate date(2016,3,2);
            QDate dateMax(2099,12,22);
            QDate dateMin(1970,1,1);
            vd.setDate(date);
            vd.setMaxDate(dateMax);
            vd.setMinDate(dateMin);
            fi->setVarDesc(*(v.pVarDescDate));
        }
        if( i == VARTYPETIME)
        {
            VarDesc v;
            VarDescTime vt;
            v.pVarDescTime = &vt;

            QTime time(12,12,12);
            QTime timeMax(23,59,59);
            QTime timeMin(0,0,0);

            vt.setMaxTime(timeMax);
            vt.setMinTime(timeMin);
            vt.setTime(time);
            fi->setVarDesc(*(v.pVarDescTime));

        }

        fi->setVarType(i);
        fi->setFunctionName("功能"+QString::number(i));
        functionList.append(fi);

    }

    ControlPanel c(&functionList);

    c.show();*/


    /*
     * 测试解释程序
    */
#ifdef TEST_MODE
    QList<FunctionItem *>l;

    DetectXml d("f:\\testxml\\test.xml",&l);

    ControlPanel c(&l);
    c.show();
    uint8_t cmd[]={0x8d,0x13,0x00};
    c.slotReceivedCommand(cmd,0);
    uint8_t cmdNumber[] = {0x92,0x13,0x02c,0x01};
    c.slotReceivedCommand(cmdNumber,0);
    uint8_t cmdEnumberate[] = {0x9f,0x13,0x02c};
    c.slotReceivedCommand(cmdEnumberate,0);
    uint8_t cmdDate[] = {0x86,0x16,0xe0,0x07,0x01,0x02};
    c.slotReceivedCommand(cmdDate,0);
    uint8_t cmdTime[] = {0x85,0x16,22,22,22};
    c.slotReceivedCommand(cmdTime,0);
#endif




    //w.show()*/;

    return a.exec();
}
