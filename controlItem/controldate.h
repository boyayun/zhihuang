#ifndef CONTROLDATE_H
#define CONTROLDATE_H

#include <QWidget>
#include "define.h"

namespace Ui {
class ControlDate;
}

class ControlDate : public QWidget
{
    Q_OBJECT

public:
    explicit ControlDate(QWidget *parent = 0);
    ControlDate(VarDesc v,QWidget *parent = 0);
    ~ControlDate();
    QDate getDate();
    void setDate(QDate d);

private:
    Ui::ControlDate *ui;
    VarDescDate *varDescDate;
signals:
    void signalStateChanged(uint8_t);



private slots:
    void on_dateEdit_dateChanged(const QDate &date);
};

#endif // CONTROLDATE_H
