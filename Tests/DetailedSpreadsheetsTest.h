#ifndef DETAILEDSPREADSHEETSTEST_H
#define DETAILEDSPREADSHEETSTEST_H

#include <QObject>

#include <ColumnType.h>

class DatasetSpreadsheet;
class Dataset;

class DetailedSpreadsheetsTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();

    /// Detailed test for 5 files.
    void testDetailedSpreadsheetFile01();
    // void detailedSpreadsheetFileTest02();
    void testDetailedSpreadsheetFile03();
    void testDetailedSpreadsheetFile04();
    // void detailedSpreadsheetFileTest05();

    //    void compareAllDefinitionDumps();

    //    void cleanupTestCase();

private:
    /**
     * @brief test few basic info of dataset.
     * @param definition definition to test.
     * @param rows number of rows to compare.
     * @param columns number of columns to compare.
     * @param activeColumns number of active columns.
     * @param name name of dataset to compare.
     */
    void testBasicInfo(Dataset& dataset, int rows, int columns, QString name);

    /**
     * @brief check some columns if matches hardcoded values.
     * @param definition dataset definition.
     * @param columnFormats format of columns to compare.
     * @param columnNames names of columns to compare.
     */
    void testColumnInfo(Dataset& definition,
                        const QVector<QPair<int, ColumnType> >& columnFormats,
                        const QVector<QPair<int, QString> >& columnNames);

    /**
     * @brief check number of rows, columns and some values in data.
     * @param definition dataset definition to test.
     * @param rows number of rows.
     * @param columns number of columns.
     * @param fields values to compare with data in dataset.
     */
    void testSampleData(Dataset& definition, int rows, int columns,
                        const QVector<std::tuple<QVariant, int, int> >& fields);

    QString getSpreadsheetsDir();

    void testDatasetConstruction(const Dataset& dataset,
                                 QVector<int>& columnsToTest,
                                 QVector<double>& compareNumericValues,
                                 QVector<QDate>& compareDateValues,
                                 QStringList& compareList, bool emptyDates);

    void testSpreadsheetFile01(DatasetSpreadsheet* dataset, QString file);

    void testSpreadsheetFile01SomeColumns(DatasetSpreadsheet* dataset);

    //    void testSpreadsheetFile02(DatasetDefinitionSpreadsheet* definition,
    //                               QString file);
    void testSpreadsheetFile03(DatasetSpreadsheet* dataset, QString file);
    void testSpreadsheetFile04(DatasetSpreadsheet* dataset, QString file);
    //    void testSpreadsheetFile05(DatasetDefinitionSpreadsheet* definition,
    //                               QString file);
};

#endif  // DETAILEDSPREADSHEETSTEST_H