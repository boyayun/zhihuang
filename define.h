#ifndef DEFINE
#define DEFINE
#include<QString>
#include<QDebug>
#include<QtSerialPort/QSerialPort>
#include<QQueue>
#include<stdint.h>
#include<QDate>
#include<QTime>

#define USE_BIG_EDITION


#define VERDION "V1.0"
#define AUTHOR "JiaQi.chen"




#define QDEBUG qDebug()<<__FILE__<<__LINE__

#define VARTYPECMD 1            // 开关量
#define VARTYPENUMBER 2         // 数字量
#define VARTYPEENUMBERATE 3     // 枚举量
#define VARTYPEALARM 4          // 报警量
#define VARTYPEDATE 5           // 日期量
#define VARTYPETIME 6           // 时间量
#define VARTYPEDATETIME 7       // 日期时间量

#define ROOT "DeviceDescription"    //根节点
#define HEADINFORMATION "DeviceInformation"         //设备头标记
#define DDVERSION "DDVersion"             //描述文件标准本号
#define DDFILEVER "DDFileVer"             //描述文件版本号
#define DDDEVICETYPE "DDDeviceType"       //文件使用者类型
#define DDURLLENGTH   "DDURLLength"       //URL长度
#define DDURL   "DDURL"                   //描述文件地址
#define MANUFACTURERCODE "ManufacturerCode" // 厂商编码
#define DEVICENAME "DeviceName"             //设备名称
#define DEVICETYPEID "DeviceTypeID"         //设备大小类
#define MODELID      "ModelID"              // 设备型号代码
#define UPDATE       "Update"               //具备在线升级能力
#define SYNCHRONOUS  "Synchronous"          //具备时间同步能力

#define FUNCTIONLIST "FunctionList"         //功能列表标识
#define FUNCTION     "Function"             //功能描述标识
#define FUNCTIONNUMBER "FunctionNumber"     //功能编号
#define FUNCTIONNAME   "FunctionName"       //功能名称
#define DISPLAY        "Display"            //是否显示
#define DIRECTION      "Direction"          //权限
#define VARTYPEDESC    "VarTypeDesc"        //类型描述标识
#define VARTYPE        "VarType"            //功能类型
#define VARINDEX       "VarIndex"           //功能控制码
#define VARLENGTH      "VarLength"          //参数长度
#define VARPARAMETERS  "VarParameters"      //类型参数标识

#define ITEM           "Item"               //
#define DEFAULTVALUE   "DefaultValue"       //

#define MAX             "Max"               //最大值
#define MIN             "Min"               //最小值
#define SCALLING        "Scalling"          //分辨率
#define UNIT            "Unit"              //单位
#define ITEMNUM         "ItemNum"           //子项数量
#define ALARMITEM        "AlarmItem"
#define ALARMCLASS      "AlarmClass"        //报警级别
#define ALARMCODE        "AlarmCode"
#define ALARMSTRING      "AlarmString"
#define ALARMBYTEPOS     "BytePos"
#define ALARMBITPOS      "BitPos"

#define YEAR            "Year"              //年
#define YEARMAX         "YearMax"           //年最大值
#define YEARMIN         "YearMin"           //年最小值
#define YEARSTEP        "YearStep"          //年步长
#define MONTH           "Month"             //y月
#define MONTHMAX        "MonthMax"          //月最大值
#define MONTHMIN        "MonthMin"          //月最小值
#define MONTHSTEP       "MonthStep"         //月步长
#define DAY             "Day"               //日
#define DAYMAX          "DayMax"            //日最大值
#define DAYMIN          "DayMin"            //日最小值
#define DAYSTEP         "DayStep"           //日步长
#define HOUR            "Hour"              //时
#define HOURMAX         "HourMax"           //时最大值
#define HOURMIN         "HourMin"           //时最小值
#define HOURSTEP        "HourStep"          //时步长
#define MINUTE          "Minute"            //分
#define MINUTEMAX       "MinuteMax"         //分最大值
#define MINUTEMIN       "MinuteMin"         //分最小值
#define MINUTESTEP      "MinuteStep"        //分步长
#define SECOND          "Second"            //分
#define SECONDMAX       "SecondMax"         //分最大值
#define SECONDMIN       "SecondMin"         //分最小值
#define SECONDSTEP      "SecondStep"        //分步长

#define DISTYPEDESC    "DisTypeDesc"       //显示描述标识
#define DISTYPE        "DisType"           //显示类型
#define DISPLAYTEXT    "DisplayText"       //显示字符串
#define DISPLAYICONINDEX "DisplayIconIndex"//显示图标编号
#define DISPARAMETERS "DisParameters"      //显示参数集合

#define DISPLAYSTRING "DisplayString"     //显示字符串
#define DISPLAYPICTUREINDEX "DisplayPictureIndex"//显示图标编号
#define DISPLAYYEAR    "DisplayYear"      //年显示
#define DISPLAYMONTH   "DisplayMonth"     //月显示
#define DISPLAYDAY      "DisplayDay"      //日显示

#define DISPLAYHOUR    "DisplayHour"     // 时显示
#define DISPLAYMINUTE  "DisplayMinute"   //分显示
#define DISPLAYSECOND  "DisplaySecond"   //秒显示
#define DISPLAYITEM    "DisplayItem"     //显示
#define DISPLAYITEMSTRING "DisplayItemString" //显示字符串
#define DISPLAYITEMPICTUREINDEX "DisplayPictureIndex" //显示图片索引

#define DISPLAYMAX     "DisplayMax"      //显示最大值
#define DISPLAYMIN     "DisplayMin"      //显示最小值
#define DISPLAYSTEP     "DisplayStep"    //显示步长
#define DISPLAYUNIT     "DisplayUnit"    //显示单位序号
#define DISPLAYDEFAULT  "DisplayDefault" //默认显示


#define DATAPOS         "DataPos"        //位置
#define DATATYPE        "DataType"       //数据类型


#define REPORTSTATE      "ReportState"  //上报状态标识
#define STATESTRUCT      "StateStruct"  //上报结构

int checkSystem()  ;
void ToBigEndian16(uint16_t *v);
void ExChangeEdition(uint16_t *v);


class VarDescCmd                        //开关描述参数
{
public:
    VarDescCmd();
    VarDescCmd(const VarDescCmd &v);
    QList<uint8_t> getItem() const;
    void setItem(const QList<uint8_t> &value);

    QList<QString> getDisItem() const;
    void setDisItem(const QList<QString> &value);

private:
    QList<uint8_t> Item;
    QList<QString> DisItem;


};
class VarDescNumber                    //数字描述参数
{
public:
    VarDescNumber();
    VarDescNumber(const VarDescNumber &v);
    int16_t getMax() const;
    void setMax(const int16_t &value);

    int16_t getMin() const;
    void setMin(const int16_t &value);

    int16_t getScalling() const;
    void setScalling(const int16_t &value);

    uint8_t getUnit() const;
    void setUnit(const uint8_t &value);

private:
    int16_t Max;
    int16_t Min;
    int16_t Scalling;
    uint8_t Unit;

};
class VarDescEnumberate               //枚举描述参数
{
public:
    VarDescEnumberate();
    VarDescEnumberate(const VarDescEnumberate &v);
    QList<uint8_t> getItem() const;
    void setItem(const QList<uint8_t> &value);

    QList<QString> getDisItem() const;
    void setDisItem(const QList<QString> &value);

private:
    QList<uint8_t> Item;
    QList<QString> DisItem;

};
class VarDescAlarm                    //报警描述参数
{
public:
    VarDescAlarm();
    VarDescAlarm(const VarDescAlarm &v);

    QList<QString> getAlarmString() const;
    void setAlarmString(const QList<QString> &value);

    QList<uint16_t> getAlarmCode() const;
    void setAlarmCode(const QList<uint16_t> &value);

    QList<uint8_t> getAlarmClass() const;
    void setAlarmClass(const QList<uint8_t> &value);

    QList<uint8_t> getBytePos() const;
    void setBytePos(const QList<uint8_t> &value);

    QList<uint8_t> getBitPos() const;
    void setBitPos(const QList<uint8_t> &value);

private:
    QList<QString> AlarmString;
    QList<uint16_t> AlarmCode;
    QList<uint8_t> AlarmClass;
    QList<uint8_t> BytePos;
    QList<uint8_t> BitPos;


};
class VarDescDate                    //日期描述参数
{
public:
    VarDescDate();
    VarDescDate(const VarDescDate &v);
    QDate getDate() const;
    void setDate(const QDate &value);

    QDate getMaxDate() const;
    void setMaxDate(const QDate &value);

    QDate getMinDate() const;
    void setMinDate(const QDate &value);

private:
    QDate Date;
    QDate MaxDate;
    QDate MinDate;
};
class VarDescTime                   //时间描述参数
{
    QTime Time;
    QTime MaxTime;
    QTime MinTime;
public:
    VarDescTime(const VarDescTime &v);
    VarDescTime();
    QTime getTime() const;
    void setTime(const QTime &value);
    QTime getMaxTime() const;
    void setMaxTime(const QTime &value);
    QTime getMinTime() const;
    void setMinTime(const QTime &value);
};

union VarDesc
{
    VarDescCmd *pVarDescCmd;
    VarDescNumber *pVarDescNumber;
    VarDescEnumberate *pVarDescEnumberate;
    VarDescAlarm *pVarDescAlarm;
    VarDescDate *pVarDescDate;
    VarDescTime *pVarDescTime;
};


class FunctionItem
{
    uint16_t functionNumber; //功能编号
    QString functionName;     //功能名称
    uint8_t varType;          //变量类型
    uint8_t direction;        //权限
    uint8_t dataPos;          //位置
    VarDesc vardesc;
    uint16_t varIndex;
    uint8_t datalength;
    uint8_t dataType;




public:
    FunctionItem(VarDesc v);
    FunctionItem(VarDescCmd v);
    FunctionItem();
    uint16_t getFunctionNumber() const;
    void setFunctionNumber(const uint16_t &value);
    QString getFunctionName() const;
    void setFunctionName(const QString &value);
    uint8_t getDirection() const;
    void setDirection(const uint8_t &value);
    uint8_t getDataPos() const;
    void setDataPos(const uint8_t &value);


    void setVarDesc(const VarDescCmd v);
    void setVarDesc(const VarDescNumber v);
    void setVarDesc(const VarDescEnumberate v);
    void setVarDesc(const VarDescAlarm v);
    void setVarDesc(const VarDescDate v);
    void setVarDesc(const VarDescTime v);

    VarDesc getVarDesc();

    uint8_t getVarType() const;
    void setVarType(const uint8_t &value);
    uint16_t getVarIndex() const;
    void setVarIndex(const uint16_t &value);
    uint8_t getDatalength() const;
    void setDatalength(const uint8_t &value);
    uint8_t getDataType() const;
    void setDataType(const uint8_t &value);
};

#endif // DEFINE


