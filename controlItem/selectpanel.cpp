#include "selectpanel.h"
#include "ui_selectpanel.h"

SelectPanel::SelectPanel(QList<QString> strlist, QList<uint16_t> codeList,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectPanel)
{
    ui->setupUi(this);
    this->alarmCodeList = codeList;
    this->alarmStringList = strlist;

    for(int i=0; i< codeList.size(); i++)
    {
        QCheckBox *checkBox = new QCheckBox(alarmStringList.at(i));
        this->checkBoxList.append(checkBox);
        this->ui->verticalLayout->addWidget(checkBox);
    }
    this->ui->verticalLayout->addWidget(this->ui->pushButton);

}

SelectPanel::~SelectPanel()
{
    delete ui;
}

void SelectPanel::on_pushButton_clicked()
{
    QList<uint16_t> selectedList;
    for(int i=0; i< checkBoxList.size();i++)
    {
        if(checkBoxList.at(i)->isChecked())
        {
            selectedList.append(alarmCodeList.at(i));
        }
    }
    emit signalALarmStateChanged(selectedList);
    this->close();
}
