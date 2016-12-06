#ifndef CONTROLDATETIME_H
#define CONTROLDATETIME_H

#include <QWidget>

namespace Ui {
class ControlDateTime;
}

class ControlDateTime : public QWidget
{
    Q_OBJECT

public:
    explicit ControlDateTime(QWidget *parent = 0);
    ~ControlDateTime();

private:
    Ui::ControlDateTime *ui;
};

#endif // CONTROLDATETIME_H
