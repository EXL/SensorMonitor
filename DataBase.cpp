#include "LoadData.h"
#include "DataBase.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlQuery>

#include <QDesktopServices>
#include <QAbstractButton>
#include <QTextStream>
#include <QFileDialog>
#include <QUrl>

#ifdef _DEBUG
#include <QDebug>
#include <iostream>
#endif

/* Public */

DataBase::DataBase(QWidget *parent)
    : QWidget(parent)
{
    generator = new Generators();
    tableModelOfVectors = new TableModelOfVectors(this);
}

void DataBase::connectToSQLiteDataBase()
{
    QSqlDatabase dbase = QSqlDatabase::addDatabase("QSQLITE");
    dbase.setDatabaseName("sensors.sqlite");
    if (!dbase.open())
    {
#ifdef _DEBUG
        qDebug() << "Error: Cannot create or open \"sqlite\" file!"
                 << "Please Check SQLite Driver!";
#endif
        QMessageBox::warning(0, dataBaseErrorTitle, dataBaseErrorBody);

    }
    else
    {
#ifdef _DEBUG
        qDebug() << "Success: Open \"sensors.sqlite\" file!";
#endif
    }

    tableViewWidget = new QTableView(this);
    sqlTableModel = new QSqlTableModel(this);
}

void DataBase::createTableInSQLiteDataBase()
{
    QSqlQuery a_query;

    /* DDL Query */
    QString str = "CREATE TABLE sensors ("
            "date VARCHAR(255), "
            "s1 DOUBLE, "
            "s2 DOUBLE, "
            "s3 DOUBLE, "
            "s4 DOUBLE, "
            "s5 DOUBLE, "
            "s6 DOUBLE, "
            "s7 DOUBLE, "
            "s8 DOUBLE, "
            "s9 DOUBLE, "
            "s10 DOUBLE, "
            "s11 DOUBLE, "
            "s12 DOUBLE, "
            "s13 DOUBLE, "
            "s14 DOUBLE, "
            "s15 DOUBLE, "
            "s16 DOUBLE "
            ");";

    bool b = a_query.exec(str);
    if (!b)
    {
#ifdef _DEBUG
        qDebug() << "Warning: Could not create DB table, it exists";
#endif
    }
    else
    {
#ifdef _DEBUG
        qDebug() << "Success: Table was created";
#endif
    }
}

void DataBase::formTableInSQLiteDataBase()
{
    QSqlQuery a_query;

    /* Setting QSqlTableModel */
    sqlTableModel->setTable("sensors");

    /* For not to edit table cells */
    tableViewWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    /* For edit table cells */
    //sqlTableModel->setEditStrategy(QSqlTableModel::OnFieldChange);

    sqlTableModel->select();
    sqlTableModel->setHeaderData(0, Qt::Horizontal, date);

    for (size_t i = 1; i <= 16; ++i)
    {
        sqlTableModel->setHeaderData(i, Qt::Horizontal, sensor + QString(" %1").arg(i));
    }

    countRowsOfDataBase = sqlTableModel->rowCount();
    countColumnOfDataBase = sqlTableModel->columnCount() - 1;

#ifdef _DEBUG
    qDebug() << "countRowsOfDataBase:" << countRowsOfDataBase;
    qDebug() << "countColumnOfDataBase" << countColumnOfDataBase;
#endif

    tableViewWidget->setModel(sqlTableModel);
    switchTableModel = true;
    tableViewWidget->selectRow(countRowsOfDataBase - 1);

    // imatrix.clear();

    readDataBaseTableToVectors(a_query);

}

bool DataBase::getSwitchTable()
{
    return switchTableModel;
}

QTableView *DataBase::getTableViewWidget()
{
    return tableViewWidget;
}

QVector<QVector<double> > &DataBase::getSensorsReadingsVector2D()
{
#ifdef _DEBUG
    qDebug() << &sensorsReadingsVector2D;
#endif
    return sensorsReadingsVector2D;

}

QVector<QString> &DataBase::getSensorsDateVector()
{
#ifdef _DEBUG
    qDebug() << &sensorsDateVector;
#endif
    return sensorsDateVector;
}

void DataBase::retranslateUi()
{
    date = tr("DATA");
    sensor = tr("Sensor");
    datePtr = &date;
    sensorPtr = &sensor;

    dataBaseErrorTitle = tr("FATAL: Database Error!");
    dataBaseErrorBody = tr("SQLite Driver not found!");

    overflowWarningTitle = tr("Warning!");
    overflowWarningBody = tr("<html>"
                             "The table was very large (256 rows overflow limit). <br>"
                             "Adding items can slow down the program. <br><br>"
                             "You can:"
                             "<ul>"
                             "<li>Continue;</li>"
                             "<li>Clear the DataBase;</li>"
                             "<li>Generate the HTML Table.</li>"
                             "</ul>"
                             "</html>");
    overflowWarningButtons[0] = tr("Continue");
    overflowWarningButtons[1] = tr("Clear DataBase");
    overflowWarningButtons[2] = tr("Generate HTML");

    errorTitleGeneral = tr("Error!");
    successTitleGeneral = tr("Success!");

    clearDataBaseSlot[0] = tr("DataBase Question");
    clearDataBaseSlot[1] = tr("Delete SQLite DataBase?");

    emptyDataBaseTableDialog = tr("Empty SQLite DataBase table!");


    saveHTMLFileSlot[0] = tr("Save *.html file");
    saveHTMLFileSlot[1] = tr("HTML page (*.html);;All Files (*)");
    saveHTMLFileSlot[2] = tr("Can't write HTML file!\n"
                             "Please check RW permission or correct select the file.\n"
                             "And try again!");
    saveHTMLFileSlot[3] = tr("HTML page is created successfully.\n"
                             "Written %1x%2 cells!\n\n"
                             "Show HTML Page in your browser?");

    saveTextFileSlot[0] = tr("Save *.txt file");
    saveTextFileSlot[1] = tr("Text File (*.txt);;All Files (*)");
    saveTextFileSlot[2] = tr("Can't write TXT file!\n"
                             "Please check RW permission or correct select the file.\n"
                             "And try again!");
    saveTextFileSlot[3] = tr("TXT file is created successfully."
                             "\nWritten %1 lines!");

    saveDataFileSlot[0] = tr("Save *.dat file");
    saveDataFileSlot[1] = tr("Data File (*.dat);;All Files (*)");
    saveDataFileSlot[2] = tr("Can't write DATA file!\n"
                             "Please check RW permission or correct select the file.\n"
                             "And try again!");
    saveDataFileSlot[3] = tr("DATA file is created successfully."
                             "\nAdded %1 lines!");

    loadTextFileSlot[0] = tr("Open *.txt file");
    loadTextFileSlot[1] = tr("Text File (*.txt);;All Files (*)");
    loadTextFileSlot[2] = tr("Can't read TXT file!\n"
                             "Please check RW permission or correct select the file.\n"
                             "And try again!");
    loadTextFileSlot[3] = tr("Attention!\n"
                             "You try to read TXT file format, which unsuitable size of columns!\n"
                             "The number of columns must be equal to 17!\n"
                             "Information: "
                             "Number of columns in the file - %1");
    loadTextFileSlot[4] = tr("The data in the TXT file is corrupted!\n"
                             "The first mistake: %1");
    loadTextFileSlot[5] = tr("TXT file is read successfully.\n"
                             "Read %1 lines!\n\n"
                             "Show plot?");

    loadDataFileSlot[0] = tr("Open *.dat file");
    loadDataFileSlot[1] = tr("Data File (*.dat);;All Files (*)");
    loadDataFileSlot[2] = tr("Can't read DATA file!\n"
                             "Please check RW permission or correct select the file.\n"
                             "And try again!");
    loadDataFileSlot[3] = tr("Invalid \"*.dat\" file!\n"
                             "Please check data file and try again!");
    loadDataFileSlot[4] = tr("Data file is read successfully.\n"
                             "Read %1 lines!\n\n"
                             "Show plot?");

#ifdef _DEBUG
    qDebug() << "Language Changed";
#endif

    formTableInSQLiteDataBase();
    tableModelOfVectors->setHeaderDataOfTable(datePtr, sensorPtr);
}

/* Public slots */
void DataBase::addRandomRowToSQLiteDataBase()
{
    QSqlQuery a_query;

    if (sqlTableModel->rowCount() >= 256)
    {
        QMessageBox *pmbx = new QMessageBox(QMessageBox::Information,
                                            overflowWarningTitle,
                                            overflowWarningBody,
                                            QMessageBox::Yes | QMessageBox::No | QMessageBox::Abort | QMessageBox::Cancel);

        pmbx->button(QMessageBox::Yes)->setText(overflowWarningButtons[0]);
        pmbx->button(QMessageBox::No)->setText(overflowWarningButtons[1]);
        pmbx->button(QMessageBox::Abort)->setText(overflowWarningButtons[2]);

        int n = pmbx->exec();
        delete pmbx;

        if (n == QMessageBox::Cancel)
        {
            return;
        }
        else if (n == QMessageBox::Yes)
        {
            /* Nothing */
        }
        else if (n == QMessageBox::No)
        {
            clearDataBase();
            return;
        }
        else if (n == QMessageBox::Abort)
        {
            saveTableToHtmlFile();
            return;
        }
    }

    QString str_insert = "INSERT INTO sensors(date, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16) "
            "VALUES ('%1', %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14, %15, %16, %17);";
    QString add_str = str_insert.arg(generator->dateGenerator())
            .arg(generator->fRand(rlim[0][0], rlim[0][1]))
            .arg(generator->fRand(rlim[1][0], rlim[1][1]))
            .arg(generator->fRand(rlim[2][0], rlim[2][1]))
            .arg(generator->fRand(rlim[3][0], rlim[3][1]))
            .arg(generator->fRand(rlim[4][0], rlim[4][1]))
            .arg(generator->fRand(rlim[5][0], rlim[5][1]))
            .arg(generator->fRand(rlim[6][0], rlim[6][1]))
            .arg(generator->fRand(rlim[7][0], rlim[7][1]))
            .arg(generator->fRand(rlim[8][0], rlim[8][1]))
            .arg(generator->fRand(rlim[9][0], rlim[9][1]))
            .arg(generator->fRand(rlim[10][0], rlim[10][1]))
            .arg(generator->fRand(rlim[11][0], rlim[11][1]))
            .arg(generator->fRand(rlim[12][0], rlim[12][1]))
            .arg(generator->fRand(rlim[13][0], rlim[13][1]))
            .arg(generator->fRand(rlim[14][0], rlim[14][1]))
            .arg(generator->fRand(rlim[15][0], rlim[15][1]));
    bool add_b = a_query.exec(add_str);
    if (!add_b)
    {
#ifdef _DEBUG
        qDebug() << "Error: INSERT INTO from table";
#endif
    }
    else
    {
#ifdef _DEBUG
        qDebug() << "Success: INSERT INTO from table";
#endif
    }

    formTableInSQLiteDataBase();

#ifdef _DEBUG
    qDebug() << countColumnOfDataBase;
#endif
}

void DataBase::clearDataBase()
{
    if (countRowsOfDataBase > 0)
    {
        QMessageBox *pmbx = new QMessageBox(QMessageBox::Question,
                                            clearDataBaseSlot[0],
                clearDataBaseSlot[1],
                QMessageBox::Yes | QMessageBox::No);
        int n = pmbx->exec();
        delete pmbx;

        if (n == QMessageBox::No)
        {
            return;
        }
        else if (n == QMessageBox::Yes)
        {
#ifdef _DEBUG
            qDebug() << "Droping Table...";
#endif

            QSqlQuery a_query;
            QString str_insert = "DROP TABLE sensors";
            bool add_drop = a_query.exec(str_insert);
            if (!add_drop)
            {
#ifdef _DEBUG
                qDebug() << "Error: DROP TABLE sensors";
#endif
            }
            else
            {
#ifdef _DEBUG
                qDebug() << "Success: DROP TABLE sensors";
#endif
            }

            createTableInSQLiteDataBase();
            formTableInSQLiteDataBase();
        }
    }
}

void DataBase::loadTableFromHeader()
{
    clearDataBase();

    QSqlQuery a_query;
    for(int i = 0; i < TEMP_TABLE_ROWS; i++)
    {
        QString str_insert = "INSERT INTO sensors(date, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16) "
                "VALUES ('%1', %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, %13, %14, %15, %16, %17);";
        QString load_str = str_insert.arg(DateT[i])
                .arg(temp_table[i][0])
                .arg(temp_table[i][1])
                .arg(temp_table[i][2])
                .arg(temp_table[i][3])
                .arg(temp_table[i][4])
                .arg(temp_table[i][5])
                .arg(temp_table[i][6])
                .arg(temp_table[i][7])
                .arg(temp_table[i][8])
                .arg(temp_table[i][9])
                .arg(temp_table[i][10])
                .arg(temp_table[i][11])
                .arg(temp_table[i][12])
                .arg(temp_table[i][13])
                .arg(temp_table[i][14])
                .arg(temp_table[i][15]);

        bool load_table = a_query.exec(load_str);
        if (!load_table)
        {
#ifdef _DEBUG
            qDebug() << "Error: INSERT INTO from table";
#endif
        }
        else
        {
#ifdef _DEBUG
            qDebug() << "Success: INSERT INTO from table";
#endif
        }
    }
    formTableInSQLiteDataBase();
}

void DataBase::saveTableToHtmlFile()
{
    if (countRowsOfDataBase > 0)
    {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        saveHTMLFileSlot[0],
                "",
                saveHTMLFileSlot[1]);
        QFile file(fileName);
        if (!(file.open(QIODevice::WriteOnly | QIODevice::Text)))
        {
            QMessageBox::critical(0, errorTitleGeneral, saveHTMLFileSlot[2]);
            return;
        }

        /* Create TextStream for HTML */
        QTextStream out(&file);

        /* HTML Structure */
        structHTML htmlPage;
        htmlPage.header = "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n"
                "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n"
                "<head>\n"
                "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n"
                "<title>HTML Table of SQLite DataBase</title>\n"
                "<body bgcolor =\"#FFFFCC\">\n"
                "<center>\n"
                "<h1>Table of sensors:</h1>\n";

        /* Taple properties and table header */
        htmlPage.table += "<table border =\"1\" bgcolor=\"#FFFF99\" cellpadding=\"5\">\n"
                "<tr>\n<td align = \"center\"><strong><i>DATE</i></strong></td>";
        for (size_t i = 0; i < countColumnOfDataBase; ++i)
        {
            htmlPage.table += QString("<td align = \"center\"><strong>Sensor %1</strong></td>").arg(i+1);
        }

        htmlPage.table += "\n</tr>\n";
        /************************************/

        htmlPage.trOpen = "<tr>\n";
        htmlPage.trClose = "</tr>\n";

        htmlPage.tdOpen = "<td align = \"center\">";
        htmlPage.tdClose = "</td>";

        htmlPage.footer = "</table>\n"
                "</center>\n"
                "</body>\n"
                "</html>";

        out << htmlPage.header;
        out << htmlPage.table;

        /* Generate HTML content table */
        for(size_t i = 0; i < countRowsOfDataBase; ++i)
        {
            /* Put DATE to HTML Table */
            out << htmlPage.trOpen << htmlPage.tdOpen << sensorsDateVector[i] << htmlPage.tdClose;

            /* Put Sensors Grid to HTML Table */
            for(size_t j = 0; j < countColumnOfDataBase; ++j)
            {
                out << htmlPage.tdOpen << sensorsReadingsVector2D[i][j] << htmlPage.tdClose;
            }
            out << htmlPage.trClose;
        }

        out << htmlPage.footer;

        file.close();

        QMessageBox *pmbx = new QMessageBox(QMessageBox::Question,
                                            successTitleGeneral,
                                            QString(saveHTMLFileSlot[3]).arg(countColumnOfDataBase).arg(countRowsOfDataBase),
                QMessageBox::Yes | QMessageBox::No);
        int n = pmbx->exec();
        delete pmbx;

        if (n == QMessageBox::No)
        {
            return;
        }
        else if (n == QMessageBox::Yes)
        {
            /* Open HTML Table in your browser */
            QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
        }
    }
    else
    {
        emptyDataBaseCriticalError();
        return;
    }
}

void DataBase::saveTableToTextFile()
{
    if(countRowsOfDataBase > 0)
    {
        QString fileName = QFileDialog::getSaveFileName(this,
                                                        saveTextFileSlot[0],
                "",
                saveTextFileSlot[1]);

        QFile file(fileName);
        if (!(file.open(QIODevice::WriteOnly | QIODevice::Text)))
        {
            QMessageBox::critical(0, errorTitleGeneral, saveTextFileSlot[2]);
            return;
        }

        /* Create TextStream */
        QTextStream out(&file);

        /* Generate txt file */
        for(size_t i = 0; i < countRowsOfDataBase; ++i)
        {
            /* Save to txt file date */
            out << sensorsDateVector[i] << "\t";

            for(size_t j = 0; j < countColumnOfDataBase; ++j)
            {
                if (j == countColumnOfDataBase - 1)
                {
                    out << sensorsReadingsVector2D[i][j];
                }
                else
                {
                    out << sensorsReadingsVector2D[i][j] << "\t";
                }
            }
            out << "\n";
        }

        file.close();
        QMessageBox::information(0, successTitleGeneral, saveTextFileSlot[3].arg(countRowsOfDataBase));
    }
    else
    {
        emptyDataBaseCriticalError();
        return;
    }
}

void DataBase::saveTableToDataFile()
{
    if (countRowsOfDataBase > 0)
    {

        QString fileName = QFileDialog::getSaveFileName(this,
                                                        saveDataFileSlot[0], "",
                saveDataFileSlot[1]);
        QFile file(fileName);
        if (!(file.open(QIODevice::WriteOnly)))
        {
            QMessageBox::critical(0, errorTitleGeneral, saveDataFileSlot[2]);
            return;
        }

        /* DataStream to save */
        QDataStream out(&file);

        /* Get global variables */
        const unsigned int row = countRowsOfDataBase;
        const unsigned int col = countColumnOfDataBase;

        /* Save MagicNumber to stream */
        out << (quint32)MagicNumber;

        /* Save global variables to stream */
        out << row;
        out << col;

        /* Loop for save arr to stream */
        for(size_t i = 0; i < row; ++i)
        {
            /* Date */
            out << sensorsDateVector[i];

            /* Sensors */
            for(size_t j = 0; j < col; ++j)
            {
                out << sensorsReadingsVector2D[i][j];
            }
        }

        file.close();
        QMessageBox::information(0, successTitleGeneral, saveDataFileSlot[3].arg(countRowsOfDataBase));
    }
    else
    {
        emptyDataBaseCriticalError();
        return;
    }
}

void DataBase::loadTableFromTextFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    loadTextFileSlot[0],
            "",
            loadTextFileSlot[1]);

    /* Clearing extern arrays */
    sensorsDateVector.clear();
    sensorsReadingsVector2D.clear();

    QFile read_file(fileName);
    QString all_file;

#ifdef _DEBUG
    qDebug() << fileName;
#endif

    if(read_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream out(&read_file);
        out.setCodec("UTF-8");
        all_file = out.readAll();
    }
    else
    {
        QMessageBox::critical(0, errorTitleGeneral, loadTextFileSlot[2]);
        return;
    }

    /* Read ALL contains text file in QStringList */
    QStringList temp = all_file.split("\n");

    /* The calculation count of the first row...
     * Maybe loop for check all contains text file?
     * - Relesed 3/5/2013
     */

    /* RegExp for check string to Float/Double */
    QRegExp exp("^[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?$");

    QString forCountRow = temp.at(0);

    /* Loop to verify that a text file with the table */
    for (int i = 0; i < temp.size() - 1; ++i)
    {
        QString forCountRowTmp = temp.at(i);

        if(forCountRowTmp.count('\t') != 16)
        {
            QMessageBox *warnCountRow = new QMessageBox(QMessageBox::Information,
                                                        overflowWarningTitle,
                                                        loadTextFileSlot[3].arg(forCountRowTmp.count('\t')+1),
                    QMessageBox::Ok);
            int n = warnCountRow->exec();
            delete warnCountRow;

            if(n == QMessageBox::Ok)
            {
                return;
            }
        }
#ifdef _DEBUG
        qDebug() << "temp.at(0) is:" << forCountRowTmp;
        qDebug() << "Tabs in row:" << forCountRowTmp.count('\t');
        qDebug() << "temp.size() is:" << temp.size();
#endif
    }

    /* Loop to check arr with RegExp */
    for(int i = 0; i < temp.size() - 1; ++i)
    {
        for(int j = 0; j < forCountRow.count('\t') + 1; ++j)
        {
            if(exp.indexIn(temp.at(i).split("\t").at(j)) == -1)
            {
                QMessageBox::information(0, errorTitleGeneral, loadTextFileSlot[4].arg(temp.at(i).split("\t").at(j)));
                return;
            }
        }
    }

    /* Loop to fill the imatrix_ext/imatrix_ext_data array */
    for(int i = 0; i < temp.size() - 1; ++i)
    {
        QVector<double> temp_vec;
        sensorsDateVector.push_back(temp.at(i).split("\t").at(0));
        for(int j = 1; j < forCountRow.count('\t') + 1; ++j)
        {
            /* Why j == 8? Need to implement a function(or not?)
             * That counts the number of values ​​in the row.
             * - Released 3/5/2013
               - Fixed 3/19/2013   */
            temp_vec.push_back(temp.at(i).split("\t").at(j).toDouble());
        }
        sensorsReadingsVector2D.push_back(temp_vec);
    }

    countRowsOfDataBase = temp.size() - 1;
    countColumnOfDataBase = forCountRow.count('\t');

#ifdef _DEBUG
    qDebug() << "countRowsOfDataBase" << countRowsOfDataBase;
    qDebug() << "countColumnOfDataBase" << countColumnOfDataBase;
#endif

    read_file.close();

    showTableModelOfVectors();

    QMessageBox *pmbx = new QMessageBox(QMessageBox::Information,
                                        successTitleGeneral,
                                        loadTextFileSlot[5].arg(temp.size() - 1),
            QMessageBox::Yes | QMessageBox::No);
    int n = pmbx->exec();
    delete pmbx;

    if (n == QMessageBox::No)
    {
        return;
    }
    else if (n == QMessageBox::Yes)
    {
#ifdef _DEBUG
        qDebug() << sensorsDateVector;
#endif
        lineChartWidget = new LineChartWidget(sensorsDateVector, sensorsReadingsVector2D);
        lineChartWidget->show();
    }
}

void DataBase::loadTableFromDataFile()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    loadDataFileSlot[0],
            "",
            loadDataFileSlot[1]);

    /* Clear extern arrays */
    sensorsDateVector.clear();
    sensorsReadingsVector2D.clear();

    QFile file(fileName);
    QDataStream in(&file);
    if (!(file.open((QIODevice::ReadOnly))))
    {
        QMessageBox::critical(0, errorTitleGeneral, loadDataFileSlot[2]);
        return;
    }

    /* Variables */
    unsigned int row_str;
    unsigned int col_str;

    QString dateTemp;
    quint32 magic;
    double temp;

    /* Read and check MagicNumber */
    in >> magic;
    if (magic != MagicNumber)
    {
        QMessageBox::critical(0, errorTitleGeneral, loadDataFileSlot[3]);
        return;
    }

    /* Read count row and col */
    in >> row_str;
    in >> col_str;

#ifdef _DEBUG
    qDebug() << "\"row\" is" << row_str << "\"col\" is" << col_str;
#endif

    for (size_t i = 0; i < row_str; ++i)
    {
        QVector<double> temp_vec;
        in >> dateTemp;
        sensorsDateVector.push_back(dateTemp);

        // qDebug() << dateTemp;

        for (size_t j = 0; j < col_str; ++j)
        {
            in >> temp;
            temp_vec.push_back(temp);
        }
        sensorsReadingsVector2D.push_back(temp_vec);
    }

    countRowsOfDataBase = row_str; // Row. Succes!
#ifdef _DEBUG
    qDebug() << "****************************************************" << col_str;
#endif
    countColumnOfDataBase = col_str; // Column. Succes!

#ifdef _DEBUG
    qDebug() << "Success read \"*.dat\" to array!";
#endif

    file.close();

    showTableModelOfVectors();

    QMessageBox *pmbx = new QMessageBox(QMessageBox::Information,
                                        successTitleGeneral,
                                        loadDataFileSlot[4].arg(row_str),
            QMessageBox::Yes | QMessageBox::No);
    int n = pmbx->exec();
    delete pmbx;

    if (n == QMessageBox::No)
    {
        return;
    }
    else if (n == QMessageBox::Yes)
    {
#ifdef _DEBUG
        qDebug() << sensorsDateVector;
#endif
        lineChartWidget = new LineChartWidget(sensorsDateVector, sensorsReadingsVector2D);
        lineChartWidget->show();
    }
}

void DataBase::changeTableModel()
{
    if (switchTableModel && countRowsOfDataBase > 0)
    {
        tableViewWidget->setModel(tableModelOfVectors);
        tableViewWidget->selectRow(0);
        switchTableModel = false;
    }
    else
    {
        formTableInSQLiteDataBase();
        tableViewWidget->setModel(sqlTableModel);
        tableViewWidget->selectRow(0);
        switchTableModel = true;
    }
}

void DataBase::setNoEditTablePolicy()
{
    tableViewWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void DataBase::setEditTablePolicy()
{
    tableViewWidget->setEditTriggers(QAbstractItemView::DoubleClicked);
}

/* Private */
void DataBase::readDataBaseTableToVectors(QSqlQuery &a_query)
{
    sensorsReadingsVector2D.clear();
    sensorsDateVector.clear();

    if (!a_query.exec("SELECT * FROM sensors"))
    {
#ifdef _DEBUG
        qDebug() << "Error: SELECT from table";
#endif
    }

    QSqlRecord rec = a_query.record();

    while (a_query.next())
    {
        QVector<double> temp;

        /* To Add Date to Vector */
        sensorsDateVector.push_back(a_query.value(rec.indexOf("date")).toString());

        /* To Add Sensor Readings to 2D Vector */
        temp.push_back(a_query.value(rec.indexOf("s1")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s2")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s3")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s4")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s5")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s6")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s7")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s8")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s9")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s10")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s11")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s12")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s13")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s14")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s15")).toDouble());
        temp.push_back(a_query.value(rec.indexOf("s16")).toDouble());
        sensorsReadingsVector2D.push_back(temp);
    }

    tableModelOfVectors->setCurrencyVectors(sensorsDateVector, sensorsReadingsVector2D);

#ifdef _DEBUG
    printVector2DDebug();
#endif
}

#ifdef _DEBUG
void DataBase::printVector2DDebug()
{
    const size_t row = sqlTableModel->rowCount();
    const size_t col = 16;
    for(size_t i = 0; i < row; ++i)
    {
        for(size_t j = 0; j < col; ++j)
        {
            std::cout << sensorsReadingsVector2D[i][j] << " ";
        }
        std::cout << std::endl;
    }
}
#endif

void DataBase::emptyDataBaseCriticalError()
{
    QMessageBox::critical(0, errorTitleGeneral, emptyDataBaseTableDialog);
}

void DataBase::showTableModelOfVectors()
{
    tableModelOfVectors->setCurrencyVectors(sensorsDateVector, sensorsReadingsVector2D);
    tableModelOfVectors->setHeaderDataOfTable(datePtr, sensorPtr);

    switchTableModel = false;

    tableViewWidget->setModel(tableModelOfVectors);
    tableViewWidget->selectRow(0);
}

DataBase::~DataBase()
{
    /* Empty Destructor */
}

/************** TABLE MODEL *************/

TableModelOfVectors::TableModelOfVectors(QObject *parent)
    : QAbstractTableModel(parent)
{
    lightYellow.setRgb(255, 255, 153);
}

void TableModelOfVectors::setCurrencyVectors(const QVector<QString> &vectorSensorsDate, const QVector<QVector<double> > &vectorSensorReadings2D)
{
    sensorDateOfReceivedVector = vectorSensorsDate;
    sensorReadingsOfReceived2DVector = vectorSensorReadings2D;

    reset();
}

void TableModelOfVectors::setHeaderDataOfTable(const QString *date, const QString *sensor)
{
    dateTable = date;
    sensorTable = sensor;
}

QVariant TableModelOfVectors::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
    {
        return QVariant();
    }
    else if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return (!section) ? QString(*dateTable) : QString(*sensorTable) + QString(" %1").arg(section);
    }
    else
    {
        return QString("%1").arg(section + 1);
    }
}

int TableModelOfVectors::rowCount(const QModelIndex & /* parent */) const
{
    return sensorDateOfReceivedVector.size();
}

int TableModelOfVectors::columnCount(const QModelIndex & /* parent */) const
{
    return sensorReadingsOfReceived2DVector[0].size() + 1;
}

QVariant TableModelOfVectors::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if(index.column())
        {
            double answer = sensorReadingsOfReceived2DVector[index.row()][index.column() - 1];
            return QVariant(answer);
        }
        else
        {
            QString answer = sensorDateOfReceivedVector[index.row()];
            return QVariant(answer);
        }
    }
    else if (role == Qt::BackgroundColorRole && index.column() == 0)
    {
        return QVariant(lightYellow);
    }
    return QVariant();
}

TableModelOfVectors::~TableModelOfVectors()
{
    /* Empty Destructor */
}
