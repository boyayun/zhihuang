#ifndef CONTROLTIME_H
#define CONTROLTIME_H

#include <QWidget>
#include "define.h"

namespace Ui {
class ControlTime;
}

class ControlTime : public QWidget
{
    Q_OBJECT

public:
    explicit ControlTime(QWidget *parent = 0);
    ControlTime(VarDesc v,QWidget *parent = 0);
    ~ControlTime();
    QTime getTime();
    void setTime(QTime t);

private:
    Ui::ControlTime *ui;
    VarDescTime *varDescTime;
signals:
    void signalStateChanged(uint8_t);
private slots:
    void on_timeEdit_timeChanged();
};

#endif // CONTROLTIME_H
