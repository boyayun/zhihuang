#include "detectxml.h"

DetectXml::DetectXml(QString path, QList<FunctionItem> *l)
{
    this->list = l;
    qDebug()<<path;
    QFile f(path);
    //qDebug()<<f.open(QFile::ReadOnly);
    QDomDocument doc;
    qDebug()<<__FILE__<<__LINE__<<doc.setContent(&f);

    QDomNodeList domList = doc.childNodes();
    for(int i=0;i<domList.size();i++)
    {
        if(domList.at(i).nodeName() == ROOT)
        {
            detectDeviceDescribe(domList.at(i));
        }
    }
}

DetectXml::DetectXml(QString path, QList<FunctionItem *> *l)
{
    this->listItem = l;
    l->clear();
    qDebug()<<path;
    QFile f(path);
    //qDebug()<<f.open(QFile::ReadOnly);
    QDomDocument doc;
    qDebug()<<__FILE__<<__LINE__<<doc.setContent(&f);

    QDomNodeList domList = doc.childNodes();
    for(int i=0;i<domList.size();i++)
    {
        if(domList.at(i).nodeName() == ROOT)
        {
            detectDeviceDescribe(domList.at(i));
        }
    }
}

void DetectXml::detectDeviceDescribe(QDomNode n)
{
    QDomNodeList domList = n.childNodes();
    for(int i=0;i<domList.size();i++)
    {
        if(domList.at(i).nodeName() == HEADINFORMATION)
        {
            //头部信息解释
            detectHeadInformation(domList.at(i));
        }
        else if(domList.at(i).nodeName() == FUNCTIONLIST)
        {
            //FunctionItem
            detectFunctionList(domList.at(i));
        }
        else
        {
            //错误
        }
    }
}

void DetectXml::detectHeadInformation(QDomNode n)
{
    QDEBUG<< "解析头部" ;
    QDEBUG<<n.nodeName();
    QDomNodeList domList = n.childNodes();
    for(int i=0;i<domList.size();i++)
    {
        QDomNode node = domList.at(i);
        if(node.nodeName()==DEVICENAME)
        {
            this->deviceName = node.toElement().text();
        }
        else if(node.nodeName() == MANUFACTURERCODE)
        {
            this->manufactureId = node.toElement().text();
        }
        else if(node.nodeName()==DEVICETYPEID)
        {
            this->deviceTypeId = node.toElement().text();
        }
    }
}

void DetectXml::detectFunctionList(QDomNode n)
{
    QDEBUG<<"解析功能列表";
    QDomNodeList domList = n.childNodes();
    for(int i=0;i<domList.size();i++)
    {
        detectFunction(domList.at(i));
    }


}

void DetectXml::detectFunction(QDomNode n)
{
    FunctionItem *f = new FunctionItem();
    //this->list->append(*f);
    this->listItem->append(f);
    QDomNodeList domList = n.childNodes();
    for(int i=0; i<domList.size();i++)
    {
        QDomNode node = domList.at(i);
        if(node.nodeName()==FUNCTIONNUMBER)
        {
            f->setFunctionNumber(node.toElement().text().toInt());
            QDEBUG<<"FunctionNumber："<<f->getFunctionNumber();

        }
        else if(node.nodeName()==FUNCTIONNAME)
        {
            f->setFunctionName(node.toElement().text());
            QDEBUG<<f->getFunctionName();
        }
        else if(node.nodeName()==DATAPOS)
        {
            f->setDataPos(node.toElement().text().toInt());
            QDEBUG<<"数据位置:"<<f->getDataPos();
        }
        else if(node.nodeName()==VARTYPEDESC)
        {
            //变量描述
            detectVarTypeDesc(node,f);
        }
        else if(node.nodeName()==DISTYPEDESC)
        {
            //显示描述
            detectDisTypeDesc(node,f);

        }

    }
}

void DetectXml::detectVarTypeDesc(QDomNode n, FunctionItem *f)
{
    QDomNodeList domList = n.childNodes();
    for(int i =0;i<domList.size();i++)
    {
        QDomNode node = domList.at(i);
        if(node.nodeName()==VARTYPE)
        {
            f->setVarType(node.toElement().text().toInt());
        }
        else if(node.nodeName()==VARINDEX)
        {
            f->setVarIndex(node.toElement().text().toInt());
        }
        else if(node.nodeName()==VARLENGTH)
        {
            f->setDatalength(node.toElement().text().toInt());
        }
        else if(node.nodeName() == DATATYPE)
        {
            f->setDataType(node.toElement().text().toInt());
        }
        else if(node.nodeName() == VARPARAMETERS)
        {
            //解析参数
            detectVarParameters(node,f);
        }
        else
        {
            QDEBUG << "ERROR";
        }

    }
}

void DetectXml::detectVarParameters(QDomNode n, FunctionItem *f)
{
    uint8_t VarType = f->getVarType();
    QDomNodeList domList = n.childNodes();
    if(VarType == VARTYPECMD)
    {
        varDesc.pVarDescCmd = new VarDescCmd();
        QList<uint8_t> item;
        for(int i=0;i<domList.size();i++)
        {
            if(domList.at(i).nodeName()== ITEM)
            {
                item.append(domList.at(i).toElement().text().toInt());
            }
        }
        varDesc.pVarDescCmd->setItem(item);
        QDEBUG << varDesc.pVarDescCmd->getItem();
    }
    else if(VarType == VARTYPENUMBER)//数字型
    {
        varDesc.pVarDescNumber = new VarDescNumber();
        for(int i=0;i<domList.size();i++)
        {
            QDomNode node = domList.at(i);
            if(node.nodeName()==MAX)
            {
                varDesc.pVarDescNumber->setMax(node.toElement().text().toInt());

            }
            else if(node.nodeName()==MIN)
            {
                varDesc.pVarDescNumber->setMin(node.toElement().text().toInt());

            }
            else if(node.nodeName()==SCALLING)
            {
                varDesc.pVarDescNumber->setScalling(node.toElement().text().toInt());
            }
            else if(node.nodeName()==DEFAULTVALUE)
            {

            }
            else
            {
                QDEBUG << "ERROR:"<<node.nodeName();
            }
        }


    }
    else if(VarType == VARTYPEENUMBERATE)
    {
        varDesc.pVarDescEnumberate = new VarDescEnumberate();
        QList<uint8_t> item;
        for(int i=0;i<domList.size();i++)
        {
            if(domList.at(i).nodeName()==ITEM)
            {
                item.append(domList.at(i).toElement().text().toInt());
            }
        }
        varDesc.pVarDescEnumberate->setItem(item);
        QDEBUG << varDesc.pVarDescEnumberate->getItem();
    }
    else if(VarType == VARTYPEALARM)
    {
        varDesc.pVarDescAlarm = new VarDescAlarm();

        QList<QString>alarmString;
        QList<uint16_t>alarmCode;
        QList<uint8_t>alarmClass;
        QList<uint8_t>alarmBytepos;
        QList<uint8_t>alarmBitpos;
        for(int i=0;i<domList.size();i++)
        {
            QDomNode node = domList.at(i);
            if(node.nodeName()==ALARMITEM)
            {
                QDomNodeList l = node.childNodes();
                for(int j=0;j<l.size();j++)
                {
                    QDomNode childnode = l.at(j);
                    if(childnode.nodeName()==ALARMCODE)
                    {
                        alarmCode.append(childnode.toElement().text().toInt());
                    }
                    else if(childnode.nodeName()==ALARMSTRING)
                    {
                        alarmString.append(childnode.toElement().text());
                    }
                    else if(childnode.nodeName()==ALARMCLASS)
                    {
                        alarmClass.append(childnode.toElement().text().toInt());
                    }
                    else if(childnode.nodeName() == ALARMBYTEPOS)
                    {
                        alarmBytepos.append(childnode.toElement().text().toInt());

                    }
                    else if(childnode.nodeName() == ALARMBITPOS)
                    {
                       alarmBitpos.append(childnode.toElement().text().toInt());
                    }
                    else
                    {
                        QDEBUG <<"Error";
                    }
                }
            }
        }
        varDesc.pVarDescAlarm->setAlarmClass(alarmClass);
        varDesc.pVarDescAlarm->setAlarmCode(alarmCode);

        varDesc.pVarDescAlarm->setAlarmString(alarmString);
        varDesc.pVarDescAlarm->setBytePos(alarmBytepos);
        varDesc.pVarDescAlarm->setBitPos(alarmBitpos);


        f->setVarDesc(*(varDesc.pVarDescAlarm));




        QDEBUG << varDesc.pVarDescAlarm->getAlarmString();
        QDEBUG << varDesc.pVarDescAlarm->getAlarmCode();
        QDEBUG << varDesc.pVarDescAlarm->getAlarmClass();
        QDEBUG << varDesc.pVarDescAlarm->getBytePos();
        QDEBUG << varDesc.pVarDescAlarm->getBitPos();
    }
    else if(VarType == VARTYPEDATE)
    {
        varDesc.pVarDescDate = new VarDescDate();
        int year,yearmax,yearmin,day,daymax,daymin,month,monthmax,monthmin;
        for(int i=0;i<domList.size();i++)
        {
            QDomNode node = domList.at(i);
            if(node.nodeName()==DAY)
            {
                day = node.toElement().text().toUInt();
            }
            else if(node.nodeName()==DAYMAX)
            {
                daymax = node.toElement().text().toUInt();
            }
            else if(node.nodeName()==DAYMIN)
            {
                daymin = node.toElement().text().toUInt();
            }
            else if(node.nodeName()==MONTH)
            {
                month= node.toElement().text().toUInt();
            }
            else if(node.nodeName()==MONTHMAX)
            {
                monthmax= node.toElement().text().toUInt();
            }
            else if(node.nodeName()==MONTHMIN)
            {
                monthmin= node.toElement().text().toUInt();
            }
            else if(node.nodeName()==YEAR)
            {
                year= node.toElement().text().toUInt();
            }
            else if(node.nodeName()==YEARMAX)
            {
                yearmax = node.toElement().text().toUInt();
            }
            else if(node.nodeName()==YEARMIN)
            {
                yearmin = node.toElement().text().toInt();

            }
        }
        varDesc.pVarDescDate->setDate(QDate(year,month,day));
        varDesc.pVarDescDate->setMaxDate(QDate(yearmax,monthmax,daymax));
        varDesc.pVarDescDate->setMinDate(QDate(yearmin,monthmin,daymin));
        QDEBUG << varDesc.pVarDescDate->getDate();
    }
    else if(VarType == VARTYPETIME)
    {
        varDesc.pVarDescTime = new VarDescTime();
        uint8_t hour,hourmax,hourmin,minute,minutemax,minutemin,second,secondmax,secondmin;
        for(int i=0;i<domList.size();i++)
        {
            QDomNode node = domList.at(i);
            if(node.nodeName()==HOUR)
            {
                hour = node.toElement().text().toUInt();
            }
            else if(node.nodeName()==HOURMAX)
            {
                hourmax = node.toElement().text().toUInt();
            }
            else if(node.nodeName()==HOURMIN)
            {
                hourmin = node.toElement().text().toUInt();
            }
            else if(node.nodeName()==MINUTE)
            {
                minute= node.toElement().text().toUInt();
            }
            else if(node.nodeName()==MINUTEMAX)
            {
                minutemax= node.toElement().text().toUInt();
            }
            else if(node.nodeName()==MINUTEMIN)
            {
                minutemin= node.toElement().text().toUInt();
            }
            else if(node.nodeName()==SECOND)
            {
                second= node.toElement().text().toUInt();
            }
            else if(node.nodeName()==SECONDMAX)
            {
                secondmax = node.toElement().text().toUInt();
            }
            else if(node.nodeName()==SECONDMIN)
            {
                secondmin = node.toElement().text().toInt();
            }
        }
        varDesc.pVarDescTime->setTime(QTime(hour,minute,second));
        varDesc.pVarDescTime->setMaxTime(QTime(hourmax,minutemax,secondmax));
        varDesc.pVarDescTime->setMinTime(QTime(hourmin,minutemin,secondmin));
        QDEBUG << varDesc.pVarDescTime->getTime();
    }
    else if(VarType == VARTYPEDATETIME)
    {
    }
    else
    {
        QDEBUG<<"ERROR";
    }
}

void DetectXml::detectDisTypeDesc(QDomNode n, FunctionItem *f)
{
    QDEBUG<< "解析显示类型参数";
   // QDEBUG<< n.nodeName();
    QDomNodeList domListl = n.childNodes();

    for(int k=0;k<domListl.size();k++) // DisTypeDesc
    {
        if(domListl.at(k).nodeName()==DISTYPE) //Distype
        {

        }
        else if(domListl.at(k).nodeName()==DISPLAYTEXT) //DisplayText
        {

        }
        else if(domListl.at(k).nodeName()==DISPARAMETERS) //Disparameters
        {
            uint8_t VarType = f->getVarType();
        //    QDEBUG <<"参数类型为："<<VarType;
            if(VarType == VARTYPECMD)
            {
        //        QDEBUG<<"开关型";
                QList<QString> ldisString;
                QDomNodeList domList = domListl.at(k).childNodes();
                for(int i=0;i<domList.size();i++)
                {
                    QDomNode node = domList.at(i);
                    if(node.nodeName()==DISPLAYITEM)
                    {
                        QDomNodeList l = node.childNodes();
                        for(int j=0;j<l.size();j++)
                        {
                            if(l.at(j).nodeName()==DISPLAYITEMSTRING)
                            {
                                ldisString.append(l.at(j).toElement().text());
                            }
                            else
                            {
                                // DisplayPictrueIndex;
                            }
                        }
                    }
                }
                QDEBUG <<ldisString;
                this->varDesc.pVarDescCmd->setDisItem(ldisString);
                f->setVarDesc(*(varDesc.pVarDescCmd));
            }
            else if(VarType == VARTYPENUMBER)
            {
                f->setVarDesc(*(varDesc.pVarDescNumber));

            }
            else if(VarType == VARTYPEENUMBERATE)
            {
                QDEBUG<<"开关型";
                QList<QString> ldisString;
                QDomNodeList domList = domListl.at(k).childNodes();
                for(int i=0;i<domList.size();i++)
                {
                    QDomNode node = domList.at(i);
                    if(node.nodeName()==DISPLAYITEM)
                    {
                        QDomNodeList l = node.childNodes();
                        for(int j=0;j<l.size();j++)
                        {
                            if(l.at(j).nodeName()==DISPLAYITEMSTRING)
                            {
                                ldisString.append(l.at(j).toElement().text());
                            }
                            else
                            {
                                // DisplayPictrueIndex;
                            }
                        }
                    }
                }
                QDEBUG <<ldisString;
                varDesc.pVarDescEnumberate->setDisItem(ldisString);
                f->setVarDesc(*(varDesc.pVarDescEnumberate));
            }
            else if(VarType == VARTYPEALARM)
            {
                //该部分在解析参数出已经完成

            }
            else if(VarType == VARTYPEDATE)
            {
                f->setVarDesc(*(varDesc.pVarDescDate));

            }
            else if(VarType == VARTYPETIME)
            {
                f->setVarDesc(*varDesc.pVarDescTime);

            }
            else if(VarType == VARTYPEDATETIME)
            {
                //暂留
            }
        }
    }
}

QString DetectXml::getDeviceName()
{
    return this->deviceName;

}

QString DetectXml::getDeviceID()
{
   return this->manufactureId+this->deviceTypeId+"000000000000000000";
}

