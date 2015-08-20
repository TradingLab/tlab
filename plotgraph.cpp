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
    std::string sql, ini_tstamp;
    int i = 0;
    float last_price;
    float val_min, val_max;
    int num_items;

    ini_tstamp = "'2015-08-20 12:00:00'";
    PlotArea(C, ini_tstamp, &val_min, &val_max, &num_items);

    /* List down all the records */
    // SYMBOL         CHAR(10) NOT NULL,"
    // TSTAMP         TIMESTAMP NOT NULL,"
    // TLAST          TIMESTAMP,"
    // LAST_TRADE     FLOAT4,"
    // ASK            FLOAT4,"
    // BID            FLOAT4,"
    // VOLUME         INT4,"

    /* Create SQL statement */
    sql = "SELECT last_trade FROM quotes WHERE symbol = 'EURUSD=X' and tstamp >" + ini_tstamp;

//    N_CNT.commit();
    /* Create a non-transactional object. */
    pqxx::nontransaction N_LT(C);

    /* Execute SQL query */
    pqxx::result R_LT( N_LT.exec( sql ));

//     i = 0;
//     for (pqxx::result::const_iterator c = R_LT.begin(); c != R_LT.end(); ++c) {
// //        cout << i << endl;
// //         std::cout << "Symbol     = " << c[0].as<string>() << std::endl;
// //         std::cout << "TStamp     = " << c[1].as<string>() << std::endl;
// //         std::cout << "TLast      = " << c[2].as<string>() << std::endl;
// //         std::cout << "-------------------------" << std::endl;
// // //            cout << "Salary = " << c[4].as<float>() << endl;
//         i += 1;
//     }
    std::cout << val_min << " " << val_max << " " << num_items << std::endl;
    std::cout << "Operation done successfully" << std::endl;


    setBackgroundRole(QPalette::Base);					//Background color: this nice gradient of gray
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);	//allow for expanding of the widget, aka use the minimumSizeHint and sizeHint

    // For xy plot using KplotWidget
    this->xyPlot = xyPlot;						//set pointer
    xyPlot->setMinimumSize( 600, 600 );					//set minimum size in pixel
    xyData = new KPlotObject( Qt::green, KPlotObject::Lines, 1);//, KPlotObject::Star); //Bars are drawn in red with thickness 2
    xyData2 = new KPlotObject( Qt::blue, KPlotObject::Lines, 1);//, KPlotObject::Square); //Bars are drawn in red with thickness 2
    xyPlot->setLimits(0, num_items, val_min, val_max);				//the data limits are 0 to 100 in the x-direction and 0 to 600 in the y-direction (initially we start at point 590)
    xyPlot->addPlotObject(xyData);					//assign the data pointer to the graph. From now on, if the data pointer contains data, it is plotted in the graph
    xyPlot->addPlotObject(xyData2);					//assign the data pointer to the graph. From now on, if the data pointer contains data, it is plotted in the graph
    xyPlot->setBackgroundColor( QColor(240, 240, 240) );		//background: light shade of gray
    xyPlot->setForegroundColor( Qt::black );				//foreground: black
    xyPlot->axis( KPlotWidget::BottomAxis )->setLabel("Time [s]");	//set x-axis labeling
    xyPlot->axis( KPlotWidget::LeftAxis )->setLabel("Value [EUR/USD]");    //set y-axis labeling
    xyPlot->setShowGrid(true);
    xyData->setShowLines( true );


    float margin = 0.2;
    xyData->setLinePen( QPen( Qt::red, 4, Qt::SolidLine ) );
    TopPrice(R_LT, val_min, val_max, margin);

// //    xyData->setShowBars( true );
//    for(int x=1; x<2000; x+=10) {
//        xyData->addPoint(x,1);
//        xyData->addPoint(x,1.109);
//        xyData->setLinePen( QPen( Qt::red, 3.0, Qt::NoPen ) );
//    }
//    xyPlot->update();

    xyData2->setLinePen( QPen( Qt::blue, 1, Qt::SolidLine ) );
    i = 0;
    for (pqxx::result::const_iterator c = R_LT.begin(); c != R_LT.end(); ++c) {
        last_price = c[0].as<float>();
        xyData2->addPoint(i,last_price);
        i += 1;
    }
    xyPlot->update();

//    pqxx::internal::sleep_seconds(4);
}

// Plot Area
int PlotGraph::PlotArea(pqxx::connection & C, std::string ini_tstamp, float *val_min, float *val_max, int *num_items) {
    std::string sql;
    float v_min, v_max;
    int n_items;
    /* Create SQL statement */
    sql = "SELECT MAX(last_trade) FROM quotes WHERE symbol = 'EURUSD=X' and tstamp > " + ini_tstamp;

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
    sql = "SELECT MIN(last_trade) FROM quotes WHERE symbol = 'EURUSD=X' and tstamp > " + ini_tstamp;

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
    sql = "SELECT COUNT(*) FROM quotes WHERE symbol = 'EURUSD=X' and tstamp > " + ini_tstamp;

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
int PlotGraph::TopPrice(pqxx::result &R, float &val_min, float &val_max, float &margin) {
    int i = 0, t_max;
    float first_price, max_price, threshold_price, last_price, abs_margin;
    
    abs_margin = (val_max - val_min) * margin;
    
    pqxx::result::const_iterator c_ini = R.begin();
    first_price = c_ini[0].as<float>();
    
    for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
        last_price = c[0].as<float>();
	if (last_price >= max_price) {
	  t_max = i;
	  max_price = last_price;
	  threshold_price = max_price - abs_margin;
	} else {
	  if (last_price <= threshold_price) break; 
	}
//        xyData->setLinePen( QPen( Qt::red, 3.0, Qt::NoPen ) );
        i += 1;
    }
    xyData->addPoint(1,first_price);
    xyData->addPoint(t_max,max_price);
    xyData->addPoint(i,threshold_price);
    xyPlot->update();
    return 0;

}
