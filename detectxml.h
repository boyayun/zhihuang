#ifndef DETECTXML_H
#define DETECTXML_H
#include"define.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomElement>



class DetectXml
{
public:
    DetectXml(QString path,QList<FunctionItem> *l);
    DetectXml(QString path, QList<FunctionItem*> *l);
    void detectDeviceDescribe(QDomNode n);
    void detectHeadInformation(QDomNode n);
    void detectFunctionList(QDomNode n);
    void detectFunction(QDomNode n);
    void detectVarTypeDesc(QDomNode n,FunctionItem *f);
    void detectVarParameters(QDomNode n,FunctionItem *f);
    void detectDisTypeDesc(QDomNode n,FunctionItem *f);
    QString getDeviceName();
    QString getDeviceID();
private:
    QList<FunctionItem> *list;
    QList<FunctionItem*>*listItem;
    QString deviceName;
    QString manufactureId;
    QString deviceTypeId;


    VarDesc varDesc;


};

#endif // DETECTXML_H
