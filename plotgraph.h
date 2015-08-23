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
    int plotArea(pqxx::connection &, std::string, std::string, std::string, float *, float *, int *);
//    int TopPrice(pqxx::result &, float &, float &, float &, int &);
    int plotMinMaxPrices(pqxx::result &, KPlotWidget* , float &, float &);
    int plotLine(KPlotWidget*, const QColor &, int &,float &,int &, float &);
//    int meanCurve(pqxx::result &, float &, float &, int &);
protected:
//    BallLogic *logic;
    KPlotWidget *xyPlot;
    KPlotObject *xyData_green;
    KPlotObject *xyData_red;
    KPlotObject *xyData_blue;
    KPlotObject *xyData_dark_blue;
};

#endif // PLOTGRAPH_H
