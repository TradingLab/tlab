/*
 * Copyright 2015 Jordi Estrada <email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef PLOTGRAPH_H
#define PLOTGRAPH_H

#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>

#include <pqxx/pqxx>

#include <QWidget>
#include <KPlotWidget>
#include <KPlotObject>
#include <KPlotAxis>
#include <QtGui>

using namespace std;

class PlotGraph : public QWidget {
    Q_OBJECT
public:
    PlotGraph(pqxx::connection &, KPlotWidget *xyPlot, QWidget *parent = 0);

private:
    int plotArea(pqxx::result &R, int*num_items);
    int plotCalcTh(pqxx::result &R, int &numBuySell, float* th_min, float* th_max);
    int plotMinMaxPrices(pqxx::result &R, KPlotWidget* xyPlot, bool &plot, float &th_buy, float &th_sell, int *numBuy, int *numSell);
    int plotPutText(KPlotWidget* xyPlot, string & text, int &posx, int &posy, float &val_min, float &val_max);
    int plotPutTextBoard(KPlotWidget* xyPlot, std::string& symbol, float & th_buy, float &val_min, float &val_max);
    int plotPrices(pqxx::result &R, KPlotWidget* xyPlot);
    int plotLine(KPlotWidget* xyPlot, const QColor &Color, bool val, double &x1,float &y1, double &x2, float &y2);
    int plotTimeRange(double &range_len);
    int plotScanner(double &range_len);
    double julianTime(std::string tstamp); 
    std::string GregorianTime(double jd);
protected:
    KPlotWidget *xyPlot;
    std::string ini_tstamp, end_tstamp; //Desired chart time range, "AAAA-MM-DD HH:MM:SS"
    std::string ts_ini,ts_end;          //Chart x timestamp ini and end, "AAAA-MM-DD HH:MM:SS"
    double jd_ini, jd_end;              //Chart x timestamp ini and end, julian day and time
    double jd_min, jd_max;              //Chart x for val_min and val_max, julian day and time
    float val_min, val_max;             //Chart y vertical min and max value, to represent char 
};

#endif // PLOTGRAPH_H
