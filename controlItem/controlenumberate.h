#ifndef CONTROLENUMBERATE_H
#define CONTROLENUMBERATE_H

#include <QWidget>
#include"define.h"

namespace Ui {
class ControlEnumberate;
}

class ControlEnumberate : public QWidget
{
    Q_OBJECT

public:
    explicit ControlEnumberate(QWidget *parent = 0);
    ControlEnumberate(VarDesc v,QWidget *parent = 0);
    ~ControlEnumberate();
    uint8_t getCurrentStatus();
    void setCurrentStatus(uint8_t index);

private:
    Ui::ControlEnumberate *ui;
signals:
    void signalStateChanged(uint8_t);
private slots:
    void on_comboBox_currentIndexChanged();
};

#endif // CONTROLENUMBERATE_H
