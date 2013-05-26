#include "NumSensorsDialog.h"

NumSensorsDialog::NumSensorsDialog(int column, QWidget *parent)
    : QDialog(parent)
{
    columnInDataBase = column;
    qClicked = false;

    spinBoxForDialog = new QSpinBox();
    spinBoxForDialog->setRange(4, 16);
    spinBoxForDialog->setSingleStep(1);
    spinBoxForDialog->setValue(columnInDataBase);
    connect(spinBoxForDialog, SIGNAL(valueChanged(int)), this, SLOT(setColumnInDataBase(int)));

    labelSensorTextForDialog = new QLabel();
    labelSensorTextForDialog->setBuddy(spinBoxForDialog);

    labelWarningTextForDialog = new QLabel();

    btnOkDialog = new QPushButton();
    connect(btnOkDialog, SIGNAL(clicked()), this, SLOT(addColumnCountInDataBase()));

    btnCancelDialog = new QPushButton();
    connect(btnCancelDialog, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *hLayForSpinBoxDialog = new QHBoxLayout;
    hLayForSpinBoxDialog->addWidget(labelSensorTextForDialog);
    hLayForSpinBoxDialog->addStretch();
    hLayForSpinBoxDialog->addWidget(spinBoxForDialog);

    QHBoxLayout *hLayForButtonsDialog = new QHBoxLayout;
    hLayForButtonsDialog->addStretch();
    hLayForButtonsDialog->addWidget(btnOkDialog);
    hLayForButtonsDialog->addWidget(btnCancelDialog);

    QVBoxLayout *vLayForWidgetsDialog = new QVBoxLayout;
    vLayForWidgetsDialog->addLayout(hLayForSpinBoxDialog);
    vLayForWidgetsDialog->addWidget(labelWarningTextForDialog);
    vLayForWidgetsDialog->addLayout(hLayForButtonsDialog);

    setLayout(vLayForWidgetsDialog);
    setModal(true);
}

void NumSensorsDialog::retranslateUi()
{
    spinBoxForDialog->setToolTip(tr("Please change the number of sensors on the building"));
    labelSensorTextForDialog->setText(tr("&The number of sensors:"));
    labelWarningTextForDialog->setText(tr("Please note, the recommended number of sensors in the building - 16.\n"
                                          "After changing the number of sensors, the DataBase clears."));
    btnOkDialog->setText(tr("&Ok"));
    btnCancelDialog->setText(tr("&Cancel"));
    setWindowTitle(tr("Number of sensors"));
}

int NumSensorsDialog::getCountColumn()
{
    return columnInDataBase;
}

bool NumSensorsDialog::getClicked()
{
    return qClicked;
}

void NumSensorsDialog::setColumnInDataBase(int column)
{
    columnInDataBase = column;
}

void NumSensorsDialog::addColumnCountInDataBase()
{
    qClicked = true;
    close();
}

NumSensorsDialog::~NumSensorsDialog()
{
    /* Empty Destructor */
}
