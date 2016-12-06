#ifndef SELECTPANEL_H
#define SELECTPANEL_H

#include <QDialog>
#include <QCheckBox>

namespace Ui {
class SelectPanel;
}

class SelectPanel : public QDialog
{
    Q_OBJECT

public:
    explicit SelectPanel(QList<QString> strlist, QList<uint16_t> codeList,QWidget *parent = 0);
    ~SelectPanel();

private:
    Ui::SelectPanel *ui;
    QList<QString> alarmStringList;
    QList<uint16_t> alarmCodeList;
    QList<QCheckBox *> checkBoxList;
signals:
    void signalALarmStateChanged(QList<uint16_t>l);


private slots:
    void on_pushButton_clicked();
};

#endif // SELECTPANEL_H
