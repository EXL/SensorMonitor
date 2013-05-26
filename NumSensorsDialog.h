#ifndef NUMSENSORSDIALOG_H
#define NUMSENSORSDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

class NumSensorsDialog : public QDialog
{
    Q_OBJECT

    size_t columnInDataBase;
    bool qClicked;

    QSpinBox *spinBoxForDialog;
    QPushButton *btnOkDialog;
    QLabel *labelSensorTextForDialog;
    QLabel *labelWarningTextForDialog;
    QPushButton *btnCancelDialog;

private slots:
    void setColumnInDataBase(int);
    void addColumnCountInDataBase();
public:
    NumSensorsDialog(int column, QWidget *parent = 0);
    void retranslateUi();
    int getCountColumn();
    bool getClicked();
    ~NumSensorsDialog();
};

#endif // NUMSENSORSDIALOG_H
