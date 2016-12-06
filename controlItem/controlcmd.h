#ifndef CONTROLCMD_H
#define CONTROLCMD_H

#include <QWidget>
#include"define.h"

namespace Ui {
class ControlCmd;
}

class ControlCmd : public QWidget
{
    Q_OBJECT

public:
    explicit ControlCmd(QWidget *parent = 0);
    ControlCmd(VarDesc v,QWidget *parent=0);
    uint8_t  getStatus();
    void setStatus(uint8_t v);


    ~ControlCmd();

private slots:

    void on_horizontalSlider_valueChanged();

private:
    Ui::ControlCmd *ui;
    VarDescCmd *varDescCmd;
signals:
    void signalStateChanged(uint8_t);
};

#endif // CONTROLCMD_H
