
#include<define.h>


VarDescCmd::VarDescCmd()
{

}

VarDescCmd::VarDescCmd(const VarDescCmd &v)
{
   this->Item = v.getItem();
    this->DisItem = v.getDisItem();
}

QList<uint8_t> VarDescCmd::getItem() const
{
    return Item;
}

void VarDescCmd::setItem(const QList<uint8_t> &value)
{
    Item = value;
}


QList<QString> VarDescCmd::getDisItem() const
{
    return DisItem;
}

void VarDescCmd::setDisItem(const QList<QString> &value)
{
    DisItem = value;
}

VarDescNumber::VarDescNumber()
{
   this->Max = 100;
   this->Min = 0;
    this->Scalling = 1;
}

VarDescNumber::VarDescNumber(const VarDescNumber &v)
{
   this->Max = v.getMax();
    this->Min = v.getMin();
    this->Scalling = v.getScalling();
    this->Unit = v.getUnit();
}

int16_t VarDescNumber::getMax() const
{
    return Max;
}

void VarDescNumber::setMax(const int16_t &value)
{
    Max = value;
}

int16_t VarDescNumber::getMin() const
{
    return Min;
}

void VarDescNumber::setMin(const int16_t &value)
{
    Min = value;
}

int16_t VarDescNumber::getScalling() const
{
    return Scalling;
}

void VarDescNumber::setScalling(const int16_t &value)
{
    Scalling = value;
}

uint8_t VarDescNumber::getUnit() const
{
    return Unit;
}

void VarDescNumber::setUnit(const uint8_t &value)
{
    Unit = value;
}

VarDescEnumberate::VarDescEnumberate()
{

}

VarDescEnumberate::VarDescEnumberate(const VarDescEnumberate &v)
{
   this->Item = v.getItem();
   this->DisItem = v.getDisItem();
}

QList<uint8_t> VarDescEnumberate::getItem() const
{
    return Item;
}

void VarDescEnumberate::setItem(const QList<uint8_t> &value)
{
    Item = value;
}

QList<QString> VarDescEnumberate::getDisItem() const
{
    return DisItem;
}

void VarDescEnumberate::setDisItem(const QList<QString> &value)
{
    DisItem = value;
}

VarDescAlarm::VarDescAlarm()
{

}

VarDescAlarm::VarDescAlarm(const VarDescAlarm &v)
{

    this->AlarmString = v.getAlarmString();
    this->AlarmCode = v.getAlarmCode();
    this->AlarmClass = v.getAlarmClass();
    this->BytePos = v.getBytePos();
    this->BitPos = v.getBitPos();
}
QList<QString> VarDescAlarm::getAlarmString() const
{
    return AlarmString;
}

void VarDescAlarm::setAlarmString(const QList<QString> &value)
{
    AlarmString = value;
}
QList<uint16_t> VarDescAlarm::getAlarmCode() const
{
    return AlarmCode;
}

void VarDescAlarm::setAlarmCode(const QList<uint16_t> &value)
{
    AlarmCode = value;
}
QList<uint8_t> VarDescAlarm::getAlarmClass() const
{
    return AlarmClass;
}

void VarDescAlarm::setAlarmClass(const QList<uint8_t> &value)
{
    AlarmClass = value;
}

QList<uint8_t> VarDescAlarm::getBytePos() const
{
    return BytePos;
}

void VarDescAlarm::setBytePos(const QList<uint8_t> &value)
{
    BytePos = value;
}

QList<uint8_t> VarDescAlarm::getBitPos() const
{
    return BitPos;
}

void VarDescAlarm::setBitPos(const QList<uint8_t> &value)
{
    BitPos = value;
}







VarDescDate::VarDescDate()
{
    
}

VarDescDate::VarDescDate(const VarDescDate &v)
{
    this->Date = v.getDate();
    this->MaxDate = v.getMaxDate();
    this->MinDate = v.getMinDate();
}

QDate VarDescDate::getDate() const
{
    return Date;
}

void VarDescDate::setDate(const QDate &value)
{
    Date = value;
}

QDate VarDescDate::getMaxDate() const
{
    return MaxDate;
}

void VarDescDate::setMaxDate(const QDate &value)
{
    MaxDate = value;
}

QDate VarDescDate::getMinDate() const
{
    return MinDate;
}

void VarDescDate::setMinDate(const QDate &value)
{
    MinDate = value;
}

VarDescTime::VarDescTime(const VarDescTime &v)
{
   this->Time  = v.getTime();
    this->MinTime = v.getMinTime();
    this->MaxTime = v.getMaxTime();
}

VarDescTime::VarDescTime()
{

}

QTime VarDescTime::getTime() const
{
    return Time;
}

void VarDescTime::setTime(const QTime &value)
{
    Time = value;
}

QTime VarDescTime::getMaxTime() const
{
    return MaxTime;
}

void VarDescTime::setMaxTime(const QTime &value)
{
    MaxTime = value;
}

QTime VarDescTime::getMinTime() const
{
    return MinTime;
}

void VarDescTime::setMinTime(const QTime &value)
{
    MinTime = value;
}


QString FunctionItem::getFunctionName() const
{
    return functionName;
}

void FunctionItem::setFunctionName(const QString &value)
{
    functionName = value;
}


uint8_t FunctionItem::getDirection() const
{
    return direction;
}

void FunctionItem::setDirection(const uint8_t &value)
{
    direction = value;
}

uint8_t FunctionItem::getDataPos() const
{
    return dataPos;
}

void FunctionItem::setDataPos(const uint8_t &value)
{
    dataPos = value;
}

void FunctionItem::setVarDesc(const VarDescAlarm v)
{
    this->vardesc.pVarDescAlarm = new VarDescAlarm(v);
}

void FunctionItem::setVarDesc(VarDescDate v)
{
    this->vardesc.pVarDescDate = new VarDescDate(v);

}

void FunctionItem::setVarDesc(const VarDescTime v)
{
    this->vardesc.pVarDescTime = new VarDescTime(v);
}

VarDesc FunctionItem::getVarDesc()
{
   return this->vardesc;
}

void FunctionItem::setVarDesc(VarDescCmd v)
{
    this->vardesc.pVarDescCmd = new VarDescCmd(v);

}

void FunctionItem::setVarDesc(VarDescNumber v)
{
    this->vardesc.pVarDescNumber  =new VarDescNumber(v);
    qDebug()<<__FUNCTION__<<vardesc.pVarDescNumber->getMax();
}

void FunctionItem::setVarDesc(const VarDescEnumberate v)
{
    this->vardesc.pVarDescEnumberate = new VarDescEnumberate(v);
}

/*void FunctionItem::setVarDesc(void *p)
{
    switch (varType) {
    case VARTYPECMD:
        this->pVarDesc = new VarDescCmd();


        break;
    case VARTYPENUMBER:
        this->pVarDesc = new VarDescNumber();
        break;
    case VARTYPEENUMBERATE:
        this->pVarDesc = new VarDescEnumberate();
        break;
    case VARTYPEALARM:
        this->pVarDesc = new VarDescAlarm();
        break;
    case VARTYPEDATE:
        this->pVarDesc = new VarDescDate();
    case VARTYPETIME:
        this->pVarDesc = new VarDescTime();
        break;
    default:
        break;
    }
}*/


uint8_t FunctionItem::getVarType() const
{
    return varType;
}

void FunctionItem::setVarType(const uint8_t &value)
{
    varType = value;
}

uint16_t FunctionItem::getVarIndex() const
{
    return varIndex;
}

void FunctionItem::setVarIndex(const uint16_t &value)
{
    varIndex = value;
}

uint8_t FunctionItem::getDatalength() const
{
    return datalength;
}

void FunctionItem::setDatalength(const uint8_t &value)
{
    datalength = value;
}

uint8_t FunctionItem::getDataType() const
{
    return dataType;
}

void FunctionItem::setDataType(const uint8_t &value)
{
    dataType = value;
}
FunctionItem::FunctionItem(VarDesc v)
{
    this->vardesc = v;
}

FunctionItem::FunctionItem()
{
    this->datalength = 0; //报警变量
    this->dataPos = 255;
}

uint16_t FunctionItem::getFunctionNumber() const
{
    return functionNumber;
}

void FunctionItem::setFunctionNumber(const uint16_t &value)
{
    functionNumber = value;
}

int checkSystem()
{
    union check
    {
        int i;
        char ch;
    }c;
    c.i=1;
    return (c.ch==1);
}

void ToBigEndian16(uint16_t *v)
{
    if(checkSystem())
    {
        uint16_t v1 = *v>>8;
        uint16_t v2 = *v<<8;
        *v = v1 | v2;
    }
}

void ExChangeEdition(uint16_t *v)
{
    uint16_t v1 = *v>>8;
    uint16_t v2 = *v<<8;
    *v = v1 | v2;
}
