#ifndef DATABASE_H
#define DATABASE_H

#include "Generators.h"
#include "LineChartWidget.h"

#include <QMessageBox>
#include <QTableView>
#include <QtSql/QSqlTableModel>
#include <QAbstractTableModel>
#include <QItemDelegate>

class TableModelOfVectors;
class DoubleSpinBoxDelegate;

struct structHTML
{
    QString header;
    QString table;
    QString footer;
    QString tdOpen;
    QString tdClose;
    QString trOpen;
    QString trClose;
};

class DataBaseEngine: public QWidget
{
    Q_OBJECT

    size_t countRowsOfDataBase;
    size_t countColumnOfDataBase;

    /* Magic Number */
    enum
    {
        MagicNumber = 0x44415441
    };

    bool switchTableModel;

    QString sensor;
    QString date;
    QString *datePtr;
    QString *sensorPtr;

    QString errorTitleGeneral;
    QString successTitleGeneral;

    QString overflowWarningTitle;
    QString overflowWarningBody;
    QString overflowWarningButtons[3];

    QString emptyDataBaseTableDialog;
    QString clearDataBaseSlot[3];

    QString saveHTMLFileSlot[4];
    QString saveTextFileSlot[4];
    QString saveDataFileSlot[4];

    QString loadTextFileSlot[6];
    QString loadDataFileSlot[5];

    Generators *generator;
    LineChartWidget *lineChartWidget;

    QVector<QVector<double> > sensorsReadingsVector2D;
    QVector<QString> sensorsDateVector;

    QTableView *tableViewWidget;
    QSqlTableModel *sqlTableModel;

    TableModelOfVectors *tableModelOfVectors;

    DoubleSpinBoxDelegate *spinBoxDelegate;

    void readDataBaseTableToVectors(QSqlQuery &a_query);
#ifdef _DEBUG
    void printVector2DDebug();
#endif
    void showTableModelOfVectors();
    void dropingTable();
public slots:
    void addRandomRowToSQLiteDataBase();
    void clearDataBase();
    void loadTableFromHeader();

    void saveTableToHtmlFile();
    void saveTableToTextFile();
    void saveTableToDataFile();

    void loadTableFromTextFile();
    void loadTableFromDataFile();

    void changeTableModel();
    void setNoEditTablePolicy();
    void setEditTablePolicy();

    void emptyDataBaseCriticalError();

    void setColumnsGlobal(int);
public:
    DataBaseEngine(QWidget *parent = 0);

    bool connectToSQLiteDataBase();
    void createTableInSQLiteDataBase();
    void formTableInSQLiteDataBase();

    bool getSwitchTable();
    QTableView *getTableViewWidget();

    QVector<QVector<double> > &getSensorsReadingsVector2D();
    QVector<QString> &getSensorsDateVector();

    void retranslateUi();

    int getColumnsCount();

    ~DataBaseEngine();
};

class TableModelOfVectors: public QAbstractTableModel
{
private:
    QVector<QString> sensorDateOfReceivedVector;
    QVector<QVector<double> > sensorReadingsOfReceived2DVector;

    QStringList header_data;

    QColor lightYellow;

    const QString *dateTable;
    const QString *sensorTable;

    int rowCount(const QModelIndex &) const;
    int columnCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role) const;

public:
    TableModelOfVectors(QObject *parent = 0);

    void setCurrencyVectors(const QVector<QString> &vectorSensorsDate,
                            const QVector<QVector<double> > &vectorSensorReadings2D);
    void setHeaderDataOfTable(const QString *date,
                              const QString *sensor);

    ~TableModelOfVectors();
};

class DoubleSpinBoxDelegate : public QItemDelegate
{
    Q_OBJECT

    int decimals;

    void setDecimals(int decimals);
    int getDecimals();
protected:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
public:
    DoubleSpinBoxDelegate(QObject *parent = 0);
    ~DoubleSpinBoxDelegate();
};

#endif // DATABASE_H
