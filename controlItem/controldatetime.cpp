#include "controldatetime.h"
#include "ui_controldatetime.h"

ControlDateTime::ControlDateTime(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlDateTime)
{
    ui->setupUi(this);
}

ControlDateTime::~ControlDateTime()
{
    delete ui;
}
