#include <math.h>
#include <algorithm>

#include <QDebug>
#include <QLocale>
#include <QVariant>
#include <QApplication>
#include <QtCore>

#include "Common/Constants.h"
#include "Shared/Logger.h"
#include "Common/ProgressBar.h"

#include "PlotDataProvider.h"

PlotDataProvider::PlotDataProvider(int defaultGroupingColumn) :
    QObject(),
    calcData_(NULL),
    groupingColumn_(defaultGroupingColumn)
{

}

PlotDataProvider::~PlotDataProvider()
{

}

void PlotDataProvider::reCompute(QVector<TransactionData>* newCalcData,
                                 DataFormat columnFormat)
{
    setNewCalcData(newCalcData);

    QVector<float> valuePerUnit;
    quantiles_.clear();

    int dataSize = calcData_->size();

    if( 0 != dataSize )
    {
        for( int i = 0; i < dataSize; ++i )
        {
            valuePerUnit.push_back(calcData_->at(i).pricePerMeter_);
        }

        quantiles_.computeQuantiles(valuePerUnit);
    }

    //Left part of group plot.
    recomputeGroupData(calcData_, groupingColumn_, columnFormat);

    //Basic data plot.
    computeBasicData();
}

void PlotDataProvider::setNewCalcData(QVector<TransactionData>* calcData)
{
    if( calcData != calcData_ )
    {
        delete calcData_;
        calcData_ = calcData;
    }
}

void PlotDataProvider::recomputeGroupData(QVector<TransactionData>* calcData,
                                          int groupingColumn,
                                          DataFormat columnFormat)
{
    //Set new data in case when only grouping column changed.
    setNewCalcData(calcData);

    //Set grouping column. Different only than actual one when changed on
    //group plot.
    groupingColumn_ = groupingColumn;

    //Remove when other column types will be managed.
    if( -1 == groupingColumn_ )
    {
        return;
    }

    QVector<QString> names;
    QVector<Quantiles> quantilesForIntervals;

    //For now only string type columns managed.
    if( DATA_FORMAT_STRING == columnFormat )
    {
        fillDataForStringGrouping(calcData, names, quantilesForIntervals);
    }

    emit setNewDataForGrouping(quantiles_.min_,
                               quantiles_.max_,
                               names,
                               quantilesForIntervals,
                               quantiles_);

    return;
}

void PlotDataProvider::fillDataForStringGrouping(QVector<TransactionData>* calcData,
                                                 QVector<QString>& names,
                                                 QVector<Quantiles>& quantilesForIntervals)
{
    QMap<QString, QVector<float> > map;

    int dataSize = calcData->size();

    //Group by name/string.
    for(int i = 0; i < dataSize; ++i)
    {
        map[calcData->at(i).groupedBy_.toString()].append(calcData->at(i).pricePerMeter_);
    }

    //For each group calculate quantiles and create names.
    QMap<QString, QVector<float> >::iterator iterator = map.begin();
    while (iterator != map.end())
    {
        names.append(iterator.key());
        Quantiles quantiles;
        quantiles.computeQuantiles(iterator.value());
        quantilesForIntervals.append(quantiles);
        ++iterator;
    }
}

void PlotDataProvider::computeBasicData()
{
    double* pointsQuantilesX = NULL;
    double* pointsQuantilesY = NULL;

    int dataSize = calcData_->size();
    if( dataSize <= 0 )
    {
        PlotData plotData(NULL, NULL, 0);

        emit basicPlotDataChanged(plotData,
                                  quantiles_,
                                  QVector<QPointF>());

        return;
    }

    //Create points for quantiles - x, y, min, max.
    pointsQuantilesX = new double[dataSize];
    pointsQuantilesY = new double[dataSize];

    double sumX = 0.0, sumY = 0.0, sumXX = 0.0, sumXY = 0.0;
    double x = 0.0, y = 0.0, a = 0.0, b = 0.0;

    double minX = 0;
    double maxX = 0;
    bool set = false;

    for( int i = 0; i < dataSize; ++i )
    {
        x = Constants::startOfTheWorld_.daysTo(calcData_->at(i).date_);
        y = calcData_->at(i).pricePerMeter_;
        pointsQuantilesX[i] = x;
        pointsQuantilesY[i] = y;

        sumX += x;
        sumY += y;
        sumXX += x * x;
        sumXY += x * y;

        if (set == false)
        {
            set = true;
            minX = x;
            maxX = x;
        }
        else
        {
            if (minX > x)
            {
                minX = x;
            }
            if (maxX < x)
            {
                maxX = x;
            }
        }
    }

    quantiles_.minX_ = minX;
    quantiles_.maxX_ = maxX;

    //Calc linear regression and create points.
    a = (dataSize * sumXY - sumX * sumY)/(dataSize * sumXX - sumX * sumX);
    b = sumY/dataSize - a * sumX / dataSize;

    QPointF linearRegressionFrom(minX, a * minX + b);
    QPointF linearRegressionTo(maxX, a * maxX + b);
    QVector<QPointF> linearRegression;
    linearRegression.append(linearRegressionFrom);
    linearRegression.append(linearRegressionTo);

    //Create plot data and propagate it.
    PlotData plotData(pointsQuantilesX, pointsQuantilesY, dataSize);

    //Basic data plot.
    emit basicPlotDataChanged(plotData,
                              quantiles_,
                              linearRegression);

    //Currently histogram only.
    emit basicDataChanged(plotData,
                          quantiles_);
}

int PlotDataProvider::getGroupByColumn()
{
    return groupingColumn_;
}