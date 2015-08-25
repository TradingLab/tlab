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
//    int i = 0;
//    float last_price;
    float val_min, val_max;
    int num_items;
    float th_min, th_max;

    /* A week trading period */
//  EURUSD=X
//  EURGBP=X
//  EURAUD=X
//  EURJPY=X
//  EURCHF=X
//  EURSEK=X
//  EURCAD=X
//  EURNZD=X
//  EURSGD=X
//  EURNOK=X
//  ini_tstamp = "'2015-08-15 20:00:00'"; //Week 34
//  end_tstamp = "'2015-08-21 20:00:00'";
    ini_tstamp = "'2015-08-24 00:00:00'"; //Week 35
    end_tstamp = "'2015-08-28 21:00:00'";
    symbol = "'EURUSD=X'";
    cout << "Symbol: " << symbol << endl;
    cout << "Quotes period: " << ini_tstamp << " to " << end_tstamp << endl;

    plotArea(C, symbol, ini_tstamp, end_tstamp, &val_min, &val_max, &num_items);
    std::cout << "Quote Value from " << val_min << " to " << val_max << " num. items " << num_items << std::endl;

    /* Create SQL statement */
    sql = "SELECT * FROM quotes WHERE symbol = " + \
          symbol + " AND tstamp >=" + ini_tstamp +\
          "AND  tstamp <=" + end_tstamp;

    /* Create a non-transactional object. */
    pqxx::nontransaction N(C);

    /* Execute SQL query */
    pqxx::result R( N.exec( sql ));

    setBackgroundRole(QPalette::Base);					//Background color: this nice gradient of gray
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);	//allow for expanding of the widget, aka use the minimumSizeHint and sizeHint

    symbolVal = "Value " + symbol;
    // For xy plot using KplotWidget
    this->xyPlot = xyPlot;						//set pointer
    xyPlot->setMinimumSize( 600, 600 );					//set minimum size in pixel
    xyPlot->setLimits(0, num_items, val_min, val_max);			//the data limits are 0 to 100 in the x-direction and 0 to 600 in the y-direction (initially we start at point 590)
    xyPlot->setBackgroundColor( QColor(240, 240, 240) );		//background: light shade of gray
    xyPlot->setForegroundColor( Qt::black );				//foreground: black
    xyPlot->axis( KPlotWidget::BottomAxis )->setLabel("Time [s]");	//set x-axis labeling
    xyPlot->axis( KPlotWidget::LeftAxis )->setLabel(symbolVal.c_str());	//set y-axis labeling
    xyPlot->setShowGrid(true);
    xyPlot->antialiasing();

    plotPrices(R, xyPlot, val_min, val_max);

    N.commit();

    //    meanCurve(R_LT, val_min, val_max, num_items);
    th_min = 0.0018 + 0.0002; //Ask profit price limit
    th_max = 0.0018 + 0.0002; //Bid profit price limit
    plotMinMaxPrices(R, xyPlot, th_min, th_max); //EURUSD
//     th_min = 0.0060; //Ask low profit price limit
//     th_max = 0.0060; //Bid low loss price limit
//     plotMinMaxPrices(R_LT, xyPlot, th_min, th_max); //EURUSD

    xyPlot->update();
    xyPlot->show();
//    pqxx::internal::sleep_seconds(4);
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

int PlotGraph::plotMinMaxPrices(pqxx::result &R, KPlotWidget* xyPlot, float &th_min, float &th_max) {
    int i = 0;
    int t_ini, t_max, t_th_max;
    int t_min, t_th_min;
    float first_price, last_price;
    float max_price, threshold_max_price;
    float min_price, threshold_min_price;

    pqxx::result::const_iterator c_ini = R.begin();
    first_price = c_ini[3].as<float>();
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
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            if (i >= t_ini) {

                last_price = c[3].as<float>();
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
        plotLine(xyPlot,Qt::red, true, t_ini,first_price,t_min, min_price);
        if (t_th_min >0) plotLine(xyPlot,Qt::cyan,false,t_min, min_price,t_th_min,threshold_min_price);

        i = 1;
        t_ini = t_min;
        first_price = min_price;
        max_price = 0;
        threshold_max_price = - th_max;//0.0022;
        t_th_max = 0;

        /* Evaluate selling price */
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            if (i >= t_ini) {
                last_price = c[3].as<float>();
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
        plotLine(xyPlot,Qt::green,true, t_ini,first_price,t_max, max_price);
        if (t_th_max > 0) plotLine(xyPlot,Qt::cyan,false,t_max,max_price,t_th_max,threshold_max_price);

        if (t_th_min == 0 || t_th_max == 0 ) {
            break;
        }
    }
    return 0;

}
int PlotGraph::plotPrices(pqxx::result &R, KPlotWidget* xyPlot, float &val_min, float &val_max ) {
    int i = 0;
    KPlotObject *xyData;
    string hhmm, mm;

    //     string sym;
    string tstamp, old_tstamp;
//     string tlast;
    float  last_trade;
//     float  ask;
//     float  bid;
//     int    volume;

    xyData  = new KPlotObject( Qt::blue, KPlotObject::Lines, 1);
    hhmm = "00:00";
    mm   = "59:59";
    for (pqxx::result::const_iterator d = R.begin(); d != R.end(); ++d) {
//       sym        = d[0].as<string>();
        tstamp     = d[1].as<string>();
        if (tstamp.substr(11,5)==hhmm) {
            plotLine(xyPlot, Qt::blue, false, i, val_min, i, val_max);
        }
// 	 if (tstamp.substr(14,5)==mm) {
//  	    plotLine(xyPlot, Qt::blue, i, val_min, i, val_max);
//  	 }
//       tlast      = d[2].as<string>();
        last_trade = d[3].as<float>();
//       ask        = d[4].as<float>();
//       bid        = d[5].as<float>();
//       volume     = d[6].as<int>();
//       cout << i << " " << sym << " " << tstamp << " " << tlast << " " << last_trade << " " << ask << " " << bid << " " << volume << " " << endl;
        xyData->addPoint(i,last_trade);
        i += 1;
    }
    xyPlot->addPlotObject(xyData);
    return 0;

}

int PlotGraph::plotLine(KPlotWidget* xyPlot, const QColor &Color, bool val, int &x1,float &y1, int &x2, float &y2) {

    KPlotObject *xyData;
    string y1_txt;
    stringstream ss (stringstream::in | stringstream::out);
    xyData = new KPlotObject(Color, KPlotObject::Lines, 2);
    xyData->setLinePen( QPen( Color, 2, Qt::SolidLine ) );

    if (val) {
        ss << y1;
        y1_txt = ss.str();
        xyData->addPoint(x1,y1, y1_txt.c_str());
    } else {
        xyData->addPoint(x1,y1);
    }
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
