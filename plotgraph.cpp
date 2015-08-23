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

#include "plotgraph.h"
#include<QPainter>
#include <string>

// Plot Graph
PlotGraph::PlotGraph(pqxx::connection &C, KPlotWidget* xyPlot, QWidget* parent): QWidget(parent) {
    std::string symbol, symbolVal, sql, tstamp, ini_tstamp, end_tstamp;
    int i = 0;
    float last_price;
    float val_min, val_max;
    int num_items;
    
    /* A week trading period */
    ini_tstamp = "'2015-08-15 20:00:00'";
    end_tstamp = "'2015-08-24 20:00:00'";
    symbol = "'EURJPY=X'";
    cout << "Symbol: " << symbol << endl;
    cout << "Quotes period: " << ini_tstamp << " to " << end_tstamp << endl;
    
    plotArea(C, symbol, ini_tstamp, end_tstamp, &val_min, &val_max, &num_items);
    std::cout << "Quote Value from " << val_min << " to " << val_max << " num. items " << num_items << std::endl;

    /* Create SQL statement */
    sql = "SELECT last_trade FROM quotes WHERE symbol = " + \
          symbol + " AND tstamp >=" + ini_tstamp +\
          "AND  tstamp <=" + end_tstamp;

    /* Create a non-transactional object. */
    pqxx::nontransaction N_LT(C);

    /* Execute SQL query */
    pqxx::result R_LT( N_LT.exec( sql ));

    setBackgroundRole(QPalette::Base);					//Background color: this nice gradient of gray
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);	//allow for expanding of the widget, aka use the minimumSizeHint and sizeHint

    // For xy plot using KplotWidget
    this->xyPlot = xyPlot;						//set pointer
    xyPlot->setMinimumSize( 600, 600 );					//set minimum size in pixel
    xyData_blue  = new KPlotObject( Qt::blue, KPlotObject::Lines, 1);	//, KPlotObject::Square); //Bars are drawn in red with thickness 2

    xyPlot->setLimits(0, num_items, val_min, val_max);			//the data limits are 0 to 100 in the x-direction and 0 to 600 in the y-direction (initially we start at point 590)

    xyPlot->addPlotObject(xyData_blue);					//assign the data pointer to the graph. From now on, if the data pointer contains data, it is plotted in the graph

    xyPlot->setBackgroundColor( QColor(240, 240, 240) );		//background: light shade of gray
    xyPlot->setForegroundColor( Qt::black );				//foreground: black
    xyPlot->axis( KPlotWidget::BottomAxis )->setLabel("Time [s]");	//set x-axis labeling
    symbolVal = "Value " + symbol;
    xyPlot->axis( KPlotWidget::LeftAxis )->setLabel(symbolVal.c_str());	//set y-axis labeling

    xyPlot->setShowGrid(true);

    xyData_blue->setLinePen( QPen( Qt::blue, 1, Qt::SolidLine ) );
//    xyData_dark_blue->setLinePen( QPen( Qt::darkRed, 1, Qt::SolidLine ) );

//    meanCurve(R_LT, val_min, val_max, num_items);
    float th_min = 0.0018 + 0.0002; //Ask profit price limit
    float th_max = 0.0018 + 0.0002; //Bid profit price limit
    plotMinMaxPrices(R_LT, xyPlot, th_min, th_max); //EURUSD
    th_min = 0.0009; //Ask low profit price limit
    th_max = 0.0009; //Bid low loss price limit
    plotMinMaxPrices(R_LT, xyPlot, th_min, th_max); //EURUSD

    i = 0;
    for (pqxx::result::const_iterator c = R_LT.begin(); c != R_LT.end(); ++c) {
//         SYMBOL     = c[0].as<string>();
// 	TSTAMP     = c[1].as<string>();
//         TLAST      = c[2].as<string>();
        last_price = c[0].as<float>();
//         LAST_TRADE = c[3].as<float>();
//         ASK        = c[4].as<float>();
//         BID        = c[5].as<float>();
//         VOLUME     = c[6].as<int>();;
        xyData_blue->addPoint(i,last_price);
        i += 1;
    }
        xyPlot->update();
        xyPlot->show();
   xyPlot->antialiasing();
   pqxx::internal::sleep_seconds(4);
}

// Plot Area
int PlotGraph::plotArea(pqxx::connection & C, std::string symbol, std::string ini_tstamp, std::string end_tstamp, float *val_min, float *val_max, int *num_items) {
    std::string sql;
    float v_min, v_max;
    int n_items;
    /* Create SQL statement */
    sql = "SELECT MAX(last_trade) FROM quotes WHERE symbol = " + \
          symbol + " AND tstamp > " + ini_tstamp + \
          " AND tstamp <= " + end_tstamp;

    /* Create a non-transactional object. */
    pqxx::nontransaction N_MAX(C);

    /* Execute SQL query */
    pqxx::result R_MAX( N_MAX.exec( sql ));

    if (!R_MAX[0][0].to<float>(v_max)) {
        throw std::runtime_error("Max quote value is not float. "
                                 "Maybe the users table is empty?");
    }
    *val_max = v_max;

    /* Create SQL statement */
    sql = "SELECT MIN(last_trade) FROM quotes WHERE symbol = " + \
          symbol + " AND tstamp > " + ini_tstamp + \
          " AND tstamp <= " + end_tstamp;

    N_MAX.commit();
    /* Create a non-transactional object. */
    pqxx::nontransaction N_MIN(C);

    /* Execute SQL query */
    pqxx::result R_MIN( N_MIN.exec( sql ));

    if (!R_MIN[0][0].to<float>(v_min)) {
        throw std::runtime_error("Min quote value is not float. "
                                 "Maybe the users table is empty?");
    }
    *val_min = v_min;
    /* Create SQL statement */
    sql = "SELECT COUNT(*) FROM quotes WHERE symbol = " + \
          symbol + " AND tstamp > " + ini_tstamp + \
          " AND tstamp <= " + end_tstamp;

    N_MIN.commit();
    /* Create a non-transactional object. */
    pqxx::nontransaction N_CNT(C);

    /* Execute SQL query */
    pqxx::result R_CNT( N_CNT.exec( sql ));

    if (!R_CNT[0][0].to<int>(n_items)) {
        throw std::runtime_error("Num. items is not int. "
                                 "Maybe the users table is empty?");
    }
    *num_items = n_items;
    return 0;
}

// Top Price
// int PlotGraph::TopPrice(pqxx::result &R, float &val_min, float &val_max, float &margin, int &items) {
//     int i = 0;
//     int t_ini, t_max, t_th_max;
//     int t_min, t_th_min;
//     float first_price, last_price;
//     float max_price, threshold_max_price;
//     float min_price, threshold_min_price;
//     float abs_margin;
//
//     abs_margin = (val_max - val_min) * margin;
//
//     pqxx::result::const_iterator c_ini = R.begin();
//     first_price = c_ini[0].as<float>();
//     t_min = 1;
//     min_price = first_price;
//     for(int repeat=0; repeat<20; repeat++) {
//         i = 1;
//         t_ini = t_min;
//         first_price = min_price;
//         max_price = 0;
//         for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
//             if (i >= t_ini) {
//                 last_price = c[0].as<float>();
//                 if (last_price >= max_price) {
//                     t_max = i;
//                     max_price = last_price;
//                     threshold_max_price = max_price - abs_margin;
//                 } else {
//                     if (last_price <= threshold_max_price) {
//                         t_th_max = i;
//                         break;
//                     }
//                 }
//             }
//             i += 1;
//         }
//         xyData_green->addPoint(t_ini,first_price);
//         xyData_green->addPoint(t_max,max_price);
//         xyData_green->addPoint(t_th_max,threshold_max_price);
//         xyPlot->update();
//
//         i = 1;
//         t_ini = t_max;
//         first_price = max_price;
//         min_price = max_price;
//         for (pqxx::result::const_iterator d = R.begin(); d != R.end(); ++d) {
//             if (i >= t_ini) {
//
//                 last_price = d[0].as<float>();
//                 if (last_price <= min_price) {
//                     t_min = i;
//                     min_price = last_price;
//                     threshold_min_price = min_price + abs_margin;
//                 } else {
//                     if (last_price >= threshold_min_price) {
//                         t_th_min = i;
//                         break;
//                     }
//                 }
//             }
//             i += 1;
//         }
//         xyData_red->addPoint(t_ini,first_price);
//         xyData_red->addPoint(t_min,min_price);
//         xyData_red->addPoint(t_th_min,threshold_min_price);
//         xyPlot->update();
// 	if (i>=items) break;
//
//     }
//     return 0;
//
// }

int PlotGraph::plotMinMaxPrices(pqxx::result &R, KPlotWidget* xyPlot, float &th_min, float &th_max) {
    int i = 0;
    int t_ini, t_max, t_th_max;
    int t_min, t_th_min;
    float first_price, last_price;
    float max_price, threshold_max_price;
    float min_price, threshold_min_price;

    pqxx::result::const_iterator c_ini = R.begin();
    first_price = c_ini[0].as<float>();
    t_min = 0;
    t_max = 0;
    max_price = first_price;
    /* Evaluate buying price */
//    for(int repeat=0; repeat<100; repeat++) {
    while (true) {
        i = 1;
        t_ini = t_max;
        first_price = max_price;
        min_price = max_price;
        threshold_min_price = th_min;//0.0018;
        t_th_min = 0;
        for (pqxx::result::const_iterator d = R.begin(); d != R.end(); ++d) {
            if (i >= t_ini) {

                last_price = d[0].as<float>();
                if (last_price <= min_price) {
                    t_min = i;
                    min_price = last_price;
                    threshold_min_price = min_price + th_min;//0.0018;
                } else {
                    if (last_price >= threshold_min_price) {
                        t_th_min = i;
                        break;
                    }
                }
            }
            i += 1;
        }
        plotLine(xyPlot,Qt::red,t_ini,first_price,t_min, min_price);
        if (t_th_min >0) plotLine(xyPlot,Qt::red,t_min, min_price,t_th_min,threshold_min_price);

        i = 1;
        t_ini = t_min;
        first_price = min_price;
        max_price = 0;
        threshold_max_price = - th_max;//0.0022;
        t_th_max = 0;

        /* Evaluate selling price */
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            if (i >= t_ini) {
                last_price = c[0].as<float>();
                if (last_price >= max_price) {
                    t_max = i;
                    max_price = last_price;
                    threshold_max_price = max_price - th_max;//0.0022;
                } else {
                    if (last_price <= threshold_max_price) {
                        t_th_max = i;
                        break;
                    }
                }
            }
            i += 1;
        }
        plotLine(xyPlot,Qt::green,t_ini,first_price,t_max, max_price);
        if (t_th_max > 0) plotLine(xyPlot,Qt::green,t_max, max_price,t_th_max,threshold_max_price);

        if (t_th_min == 0 || t_th_max == 0 ) {
            break;
        }
    }
    return 0;

}

int PlotGraph::plotLine(KPlotWidget* xyPlot, const QColor &Color, int &x1,float &y1, int &x2, float &y2) {

    KPlotObject *xyData;

    xyData = new KPlotObject(Color, KPlotObject::Lines, 2);
    xyData->setLinePen( QPen( Color, 2, Qt::SolidLine ) );
    
    xyData->addPoint(x1,y1);
    xyData->addPoint(x2,y2);

    xyPlot->addPlotObject(xyData);

    return 0;

}

//Differential Curve
//int PlotGraph::meanCurve(pqxx::result &R, float &val_min, float &val_max, int &items) {
//     int n = 10; //Items for mean value
//     int i, j;
//     float last_price, mean_price;
//
// //    bias_price = val_min + (val_max-val_min)/2;
//
//     for (j=n; j<=items; ++j) {
//         i = 0;
//         mean_price = 0;
//         for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
//             last_price = c[0].as<float>();
//             if (i>=j-n && i<j) {
//                 mean_price = (mean_price+last_price)/2;
//             }
//             i +=1;
//         }
//         cout << j << " " << mean_price << endl;
// //      xyData_dark_blue->addPoint(j,bias_price+diff_price);
//         xyData_dark_blue->addPoint(j,mean_price+0.001);
// //      xyData_dark_blue->addPoint(i,1.128);
// //      xyData_dark_blue->addPoint(t_max,max_price);
// //      xyData_dark_blue->addPoint(t_th_max,threshold_max_price);
//     }
//     xyPlot->update();
//     return 0;
//
// }
