#ifndef CONTROLNUMBER_H
#define CONTROLNUMBER_H

#include <QWidget>
#include"define.h"

namespace Ui {
class ControlNumber;
}

class ControlNumber : public QWidget
{
    Q_OBJECT

public:
    explicit ControlNumber(QWidget *parent = 0);
    ControlNumber(VarDesc v,QWidget *parent=0);
    ~ControlNumber();

    int16_t getNumber();
    void setNumber(int16_t number);

private:
    Ui::ControlNumber *ui;
    VarDescNumber *varDescNumber;
signals:
    void signalStateChanged(uint8_t);
private slots:
    void on_dial_sliderReleased();
};

#endif // CONTROLNUMBER_H
