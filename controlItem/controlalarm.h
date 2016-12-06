#ifndef CONTROLALARM_H
#define CONTROLALARM_H

#include <QWidget>
#include "define.h"
#include <QTimer>
#include <QWidget>
#include <QCheckBox>
#include "selectpanel.h"

namespace Ui {
class ControlAlarm;
}

class ControlAlarm : public QWidget
{
    Q_OBJECT

public:
    explicit ControlAlarm(QWidget *parent = 0);
    ControlAlarm(VarDesc v,QWidget *parent = 0);
    uint8_t getAlarm();

    void stopAlarm();

    ~ControlAlarm();

    QList<uint8_t> getFlagBytePos() const;
    void setFlagBytePos(const QList<uint8_t> &value);

    QList<uint8_t> getFlagBitPos() const;
    void setFlagBitPos(const QList<uint8_t> &value);



    bool getHasBytepos() const;
    void setHasBytepos(const &value);

    QList<uint16_t> getCurrentAlarmCodelist() const;

private slots:
    void on_checkBox_clicked(bool checked);
    void slotAlarmCome();

    void on_comboBox_currentIndexChanged(int index);

    void on_spinBox_valueChanged();

    void on_checkBox_clicked();

    void on_pushButton_clicked();

private:
    Ui::ControlAlarm *ui;
    VarDescAlarm *varDescAlarm;
    SelectPanel *selectPanel;
    QTimer alarmTimer;
    uint16_t currentErrorCode;
    bool hasBytepos;
    QList<uint16_t> errorCodeList;
    QList<uint8_t> flagBytePos;
    QList<uint8_t> flagBitPos;
    QList<uint16_t> currentAlarmCodelist;
private slots:
    void slotAlarmStatusChanged(QList<uint16_t> l);

signals:
    void signalAlarms(uint16_t errorCode);
    void signalStateChanged(uint8_t);
};

#endif // CONTROLALARM_H
