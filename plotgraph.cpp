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

PlotGraph::PlotGraph(pqxx::connection &C, KPlotWidget* xyPlot, QWidget* parent): QWidget(parent) {
    std::string sql;
    int i = 0;
    float ft_max, ft_min, last;
    int in_cnt;

    /* Create SQL statement */
    sql = "SELECT MAX(last_trade) FROM quotes WHERE symbol = 'EURUSD=X' and tstamp > '2015-08-17 00:00:00'";

    /* Create a non-transactional object. */
    pqxx::nontransaction N_MAX(C);

    /* Execute SQL query */
    pqxx::result R_MAX( N_MAX.exec( sql ));

    if (!R_MAX[0][0].to<float>(ft_max)) {
        throw std::runtime_error("Max user ID is not float. "
                                 "Maybe the users table is empty?");
    }
    
    /* Create SQL statement */
    sql = "SELECT MIN(last_trade) FROM quotes WHERE symbol = 'EURUSD=X' and tstamp > '2015-08-17 00:00:00'";

    N_MAX.commit();
    /* Create a non-transactional object. */
    pqxx::nontransaction N_MIN(C);

    /* Execute SQL query */
    pqxx::result R_MIN( N_MIN.exec( sql ));

    if (!R_MIN[0][0].to<float>(ft_min)) {
        throw std::runtime_error("Max user ID is not float. "
                                 "Maybe the users table is empty?");
    }
    
    /* Create SQL statement */
    sql = "SELECT COUNT(*) FROM quotes WHERE symbol = 'EURUSD=X' and tstamp > '2015-08-17 00:00:00'";

    N_MIN.commit();
    /* Create a non-transactional object. */
    pqxx::nontransaction N_CNT(C);

    /* Execute SQL query */
    pqxx::result R_CNT( N_CNT.exec( sql ));

    if (!R_CNT[0][0].to<int>(in_cnt)) {
        throw std::runtime_error("Max user ID is not float. "
                                 "Maybe the users table is empty?");
    }
    /* List down all the records */
    // SYMBOL         CHAR(10) NOT NULL,"
    // TSTAMP         TIMESTAMP NOT NULL,"
    // TLAST          TIMESTAMP,"
    // LAST_TRADE     FLOAT4,"
    // ASK            FLOAT4,"
    // BID            FLOAT4,"
    // VOLUME         INT4,"

    /* Create SQL statement */
    sql = "SELECT last_trade FROM quotes WHERE symbol = 'EURUSD=X' and tstamp > '2015-08-17 00:00:00'";

    N_CNT.commit();
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
    std::cout << ft_max << " " << ft_min << " " << in_cnt << std::endl;
    std::cout << "Operation done successfully" << std::endl;


    setBackgroundRole(QPalette::Base);					//Background color: this nice gradient of gray
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);	//allow for expanding of the widget, aka use the minimumSizeHint and sizeHint

    // For xy plot using KplotWidget
    this->xyPlot = xyPlot;						//set pointer
    xyPlot->setMinimumSize( 600, 600 );					//set minimum size in pixel
    xyData = new KPlotObject( Qt::blue, KPlotObject::Points, 10, KPlotObject::Star); //Bars are drawn in red with thickness 2
    xyData2 = new KPlotObject( Qt::red, KPlotObject::Lines, 1);//, KPlotObject::Square); //Bars are drawn in red with thickness 2
//      xyPlot->setLimits(0, 400, 0, 400);					//the data limits are 0 to 100 in the x-direction and 0 to 600 in the y-direction (initially we start at point 590)
    xyPlot->setLimits(0, in_cnt, ft_min, ft_max);				//the data limits are 0 to 100 in the x-direction and 0 to 600 in the y-direction (initially we start at point 590)
    xyPlot->addPlotObject(xyData);					//assign the data pointer to the graph. From now on, if the data pointer contains data, it is plotted in the graph
    xyPlot->addPlotObject(xyData2);					//assign the data pointer to the graph. From now on, if the data pointer contains data, it is plotted in the graph
    xyPlot->setBackgroundColor( QColor(240, 240, 240) );		//background: light shade of gray
    xyPlot->setForegroundColor( Qt::black );				//foreground: black
    xyPlot->axis( KPlotWidget::BottomAxis )->setLabel("Time [s]");	//set x-axis labeling
    xyPlot->axis( KPlotWidget::LeftAxis )->setLabel("Value [EUR/USD]");    //set y-axis labeling
    xyPlot->setShowGrid(true);
    xyData->setShowLines( true );
// //    xyData->setShowBars( true );
//     xyData->setLinePen( QPen( Qt::red, 1, Qt::SolidLine ) );
//     for(int x=1; x<200; x+=10) {
// //        xyData->addPoint(x,1);
//         xyData->addPoint(x,100);
//         xyPlot->update();
// //        xyData->setLinePen( QPen( Qt::red, 3.0, Qt::NoPen ) );
//     }
// //    update();
//     xyData2->setLinePen( QPen( Qt::blue, 2, Qt::SolidLine ) );
//     for(int x=1; x<200; x+=10) {
//         xyData2->addPoint(x,200);
//         xyData2->addPoint(x,300);
//         xyPlot->update();
// //        xyData->setLinePen( QPen( Qt::red, 3.0, Qt::NoPen ) );
//     }
// //    update();

    xyData2->setLinePen( QPen( Qt::blue, 2, Qt::SolidLine ) );
    i = 0;
    for (pqxx::result::const_iterator c = R_LT.begin(); c != R_LT.end(); ++c) {
        last = c[0].as<float>();
        xyData2->addPoint(i,last);
//        xyData->setLinePen( QPen( Qt::red, 3.0, Qt::NoPen ) );
	i += 1;
    }
    xyPlot->update();
    
}
