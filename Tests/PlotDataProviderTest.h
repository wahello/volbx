#ifndef PLOTDATAPROVIDERTEST_H
#define PLOTDATAPROVIDERTEST_H

#include <QObject>
#include <QSignalSpy>

#include <ColumnType.h>
#include <Quantiles.h>

#include "TransactionData.h"

class PlotDataProviderTest : public QObject
{
public:
    explicit PlotDataProviderTest(QObject* parent = nullptr);

    Q_OBJECT
private Q_SLOTS:
    void initTestCase();

    void testRecomputeGroupingDataWrongColumnFormat();
    void testRecomputeGroupingData();
    void testRecomputeGroupingDataEmptyCalcData();

    void testRecomputeEmptyCalcData();

private:
    void checkRecomputeGroupingDataForColumnType(ColumnType columnType);
    void checkSignalParamsForRecomputeGroupingAreEmpty(
        const QList<QVariant>& signalParameters);

    static constexpr int NO_SIGNAL{0};
    static constexpr int SIGNAL{1};

    const QVector<TransactionData> calcData_{
        {QDate(1, 3, 2010), QVariant("column1"), 10},
        {QDate(4, 3, 2010), QVariant("column1"), 15},
        {QDate(6, 3, 2010), QVariant("column1"), 12},
        {QDate(1, 3, 2010), QVariant("column2"), 1},
        {QDate(4, 3, 2010), QVariant("column2"), 5},
        {QDate(6, 3, 2010), QVariant("column2"), 2}};

    Quantiles mainQuantiles_;
    Quantiles firstQuantiles_;
    Quantiles secondQuantiles_;
};

#endif  // PLOTDATAPROVIDERTEST_H
