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
    std::string symbol, symbolVal, sql, tstamp;
    float val_min, val_max;
    float th_min, th_max;
    double jd_min, jd_max;
    int num_items;

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

    symbol = "'EURUSD=X'";
    /* Time range calculation */
    std::string ini_tstamp, end_tstamp;
//  ini_tstamp = "'2015-08-17 00:00:00'"; //Week 34
//  end_tstamp = "'2015-08-21 21:00:00'";
//     ini_tstamp = "'2015-08-24 00:00:00'"; //Week 35
//     end_tstamp = "'2015-08-28 21:00:00'";

    double timeRangeLen = 2; //days with decimals
    plotTimeRange(timeRangeLen, &ini_tstamp, &end_tstamp);
    
    /*  plotMinMaxPrices Parametes */
//     th_min = 0.0018 + 0.0002;  //Ask profit price limit
//     th_max = 0.0018 + 0.0002;  //Bid profit price limit
    th_min = 0.004;            //Ask profit price limit
    th_max = 0.004;            //Bid profit price limit
//     th_min3 = 0.0018 - 0.0008; //Ask profit price limit
//     th_max3 = 0.0018 - 0.0008; //Bid profit price limit

    cout << "Symbol: " << symbol << endl;
    cout << "Quotes period: " << ini_tstamp << " to " << end_tstamp << endl;

    plotArea(C, symbol, ini_tstamp, end_tstamp, &jd_min, &jd_max, &val_min, &val_max, &num_items);
    cout.precision(10);
    cout << "period in Julian time: " << jd_min << " to " << jd_max << endl;
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
//    xyPlot->setLimits(0, num_items, val_min, val_max);
    xyPlot->setLimits(0, jd_max - jd_min, val_min, val_max);			//
    xyPlot->setBackgroundColor( QColor(240, 240, 240) );		//background: light shade of gray
    xyPlot->setForegroundColor( Qt::black );				//foreground: black
    xyPlot->axis( KPlotWidget::BottomAxis )->setLabel("Time [Julian Day]");	//set x-axis labeling
    xyPlot->axis( KPlotWidget::LeftAxis )->setLabel(symbolVal.c_str());	//set y-axis labeling
    xyPlot->setShowGrid(true);
    xyPlot->antialiasing();

//    N.commit();
      
    plotMinMaxPrices(R, xyPlot, jd_min, th_min, th_max);

    /* Plot Texts*/
    string text = "Symbol: " + symbol;
    int posx = 20, posy = 1;
    plotPutText(xyPlot, text, ini_tstamp, end_tstamp, posx, posy, val_min, val_max);
    posy = 2;
    text = "Init. time: " + ini_tstamp;
    plotPutText(xyPlot, text, ini_tstamp, end_tstamp, posx, posy, val_min, val_max);
    posy = 3;
    text = "End time: " + end_tstamp;
    plotPutText(xyPlot, text, ini_tstamp, end_tstamp, posx, posy, val_min, val_max);
    posy = 4;
    stringstream ss1 (stringstream::in | stringstream::out);
    ss1 << th_min;
    text = ss1.str();
    text = "Bid/ask gain margin:         " + text;
    plotPutText(xyPlot, text, ini_tstamp, end_tstamp, posx, posy, val_min, val_max);

    plotPrices(R, xyPlot, jd_min, val_min, val_max);

    xyPlot->update();
    xyPlot->show();
//    pqxx::internal::sleep_seconds(4);
}

int PlotGraph::plotTimeRange(double &range_len, std::string * ini_tstamp, std::string * end_tstamp) {
    /* Present time calc, ends in the present */
    char buf[80];
    string s1, s2;
    double julTime, ini_time;

    time_t t = time(0);
    struct tm *tm = localtime( &t );
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", tm);
    s1 = buf;
    s2 = "'" + s1 + "'";
    *end_tstamp = s2;

    /* Calculate initial time stamp */
    julTime = julianTime(s1);
    ini_time = julTime - range_len;
    s1 = GregorianTime(ini_time);
    s2 = "'" + s1 + "'";
    *ini_tstamp = s2;
   
    return 0;
}

// Plot Area
int PlotGraph::plotArea(pqxx::connection & C, std::string symbol, std::string ini_tstamp, std::string end_tstamp, double *jd_min, double *jd_max, float *val_min, float *val_max, int *num_items) {
    std::string sql, ts;
    float v_min, v_max;
    int n_items;
    double julTime;

    /* Convert time stamp to Julian Date, remove '' */
    ts = ini_tstamp.substr(1,19);
    julTime = julianTime(ts);
    *jd_min = julTime;

    ts = end_tstamp.substr(1,19);
    julTime = julianTime(ts);
    *jd_max = julTime;

    /* Create Max price SQL statement */
    sql = "SELECT MAX(last_trade) FROM quotes WHERE symbol = " + \
          symbol + " AND tstamp > " + ini_tstamp + \
          " AND tstamp <= " + end_tstamp;
    /* Create Max price SQL statement */

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

    /* Create Min Price SQL statement */
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
    /* Create record number QL statement */
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

double PlotGraph::julianTime(std::string tstamp) {
// tstamp format: YYYY-MM-DD HH:MM:SS
// Calculates the Reduced JD
// Name 		Epoch 			Calculation 		Notes
// Reduced JD 		12h Nov 16, 1858 	JD − 2400000
    string ts;
    struct tm  tm;
    int day, month, year, hour, min, sec;
    int at, yt, mt;
    double julTime, hms;

    strptime(tstamp.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
    day   = tm.tm_mday;
    month = tm.tm_mon+1;
    year  = tm.tm_year+1900;
    hour  = tm.tm_hour;
    min   = tm.tm_min;
    sec   = tm.tm_sec;
    at    = (14 - month) / 12;
    yt    = year + 4800 - at;
    mt    = month + 12 * at - 3;
    julTime = 0;
    if (year > 1582 || (year == 1582 && month > 10) || (year == 1582 && month == 10 && day >= 15))
        julTime = day + (153 * mt + 2) / 5 + 365 * yt + yt / 4 - yt / 100 + yt / 400 - 32045;
    else
        julTime = day + (153 * mt + 2) / 5 + 365 * yt + yt / 4 - 32083;

    julTime = julTime - 2440000; // REDUCED JD //
    hms = (sec + 60 * (min +  60 * hour));
    hms = hms / (24 * 60 * 60);
    julTime = julTime + hms;

    return julTime;

}

//http://robm.fastmail.fm/articles/date_class.html
string PlotGraph::GregorianTime(double jd) {
    long julian, a, b, c, d, e, m;
    int hms1;
    float hms2, hms3;
    string gd;
    struct tm tm;
    char buf[80];
    int day, month, year;
    int hour, min, sec;

    julian = jd + 2440000;
    hms1 = jd;
    hms2 = jd - hms1;
    
    hms3 = hms2 * 24 * 60 * 60;
    
    hour = hms3 / (60 * 60);
    min  = ( hms3 / 60 ) - ( hour * 60 );
    sec  = hms3 - ((hour * 60) + min) * 60;

    a = julian + 32044;
    b = (4*a+3)/146097;;
    c = a - (b*146097)/4;

    d = (4*c+3)/1461;
    e = c - (1461*d)/4;
    m = (5*e+2)/153;

    day = e - (153*m+2)/5 + 1;
    month = m + 3 - 12*(m/10);
    year = b*100 + d - 4800 + m/10;
    
    tm.tm_mday = day;
    tm.tm_mon = month-1;
    tm.tm_year = year -1900;
    tm.tm_hour = hour;
    tm.tm_min  = min;
    tm.tm_sec  = sec;

    strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tm);
    gd.append(buf);
    
//     cout.precision(20);
//     cout <<jd << " " << julian << " " << year << "-" << month << "-" << day << endl;
//     cout << hms3 << " " << hour << " " << " " << min << " " << sec << " " << gd << endl;

    return gd;
}

int PlotGraph::plotMinMaxPrices(pqxx::result &R, KPlotWidget* xyPlot, double &jd_min, float &th_min, float &th_max) {
    double t_ini, time;
    double t_min, t_th_min;
    double t_max, t_th_max;

    float first_price, last_price;
    float max_price, threshold_max_price;
    float min_price, threshold_min_price;
    string tstamp;
    double julTime;

    pqxx::result::const_iterator c_ini = R.begin();
    first_price = c_ini[3].as<float>();
    t_min = 0;
    t_max = 0;
    max_price = first_price;

    /* Evaluate buying price */
    while (true) {
        t_ini = t_max;
        first_price = max_price;
        min_price = max_price;
        threshold_min_price = th_min;//0.0018;
        t_th_min = 0;
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            tstamp     = c[1].as<string>();
            julTime = julianTime(tstamp);
            time = julTime - jd_min;
            if (time >= t_ini) {
                last_price = c[3].as<float>();
                if (last_price <= min_price) {
                    t_min = time;
                    min_price = last_price;
                    threshold_min_price = min_price + th_min;//0.0018;
                } else {
                    if (last_price >= threshold_min_price) {
                        t_th_min = time;
                        break;
                    }
                }
            }
        }
        plotLine(xyPlot,Qt::red, true, t_ini,first_price,t_min, min_price);
        if (t_th_min >0) plotLine(xyPlot,Qt::cyan,false,t_min, min_price,t_th_min,threshold_min_price);

        t_ini = t_min;
        first_price = min_price;
        max_price = 0;
        threshold_max_price = - th_max;//0.0022;
        t_th_max = 0;

        /* Evaluate selling price */
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            tstamp     = c[1].as<string>();
            julTime = julianTime(tstamp);
            time = julTime - jd_min;
            if (time >= t_ini) {
                last_price = c[3].as<float>();
                if (last_price >= max_price) {
                    t_max = time;
                    max_price = last_price;
                    threshold_max_price = max_price - th_max;//0.0022;
                } else {
                    if (last_price <= threshold_max_price) {
                        t_th_max = time;
                        break;
                    }
                }
            }
        }
        
        plotLine(xyPlot,Qt::green,true, t_ini,first_price,t_max, max_price);
        if (t_th_max > 0) plotLine(xyPlot,Qt::cyan,false,t_max,max_price,t_th_max,threshold_max_price);

        if (t_th_min == 0 || t_th_max == 0 ) {
            break;
        }
    }
    return 0;

}

int PlotGraph::plotPutText(KPlotWidget* xyPlot, string & text, std::string & ini_ts, std::string & end_ts, int &posx, int &posy, float &val_min, float &val_max) {
    KPlotObject *xyData;
    string ts;
    double ini_jd, end_jd, jd;
    double x;//>
    float y; //^
    
    ts = ini_ts.substr(1,19);
    ini_jd = julianTime(ts);
    ts = end_ts.substr(1,19);
    end_jd = julianTime(ts);
    jd = end_jd - ini_jd;
    x  = posx * jd/40;
    
    y = val_max - posy * (val_max - val_min)/40;
    xyData  = new KPlotObject( Qt::black, KPlotObject::Points, 0);
    xyData->addPoint(x,y, text.c_str());
   
//     plotLine(xyPlot, Qt::red, false, x, val_min, x, val_max);
    xyPlot->addPlotObject(xyData);
    
  return 0;
}

int PlotGraph::plotPrices(pqxx::result &R, KPlotWidget* xyPlot, double &jd_min, float &val_min, float &val_max ) {
    KPlotObject *xyData;
    string hhmm, mm1, mm2;
    double julTime, time;


    //     string sym;
    string tstamp, old_tstamp;
//     string tlast;
    float  last_trade;
//     float  ask;
//     float  bid;
//     int    volume;

    xyData  = new KPlotObject( Qt::blue, KPlotObject::Lines, 1);
    hhmm = "00:00";
    mm1   = "00";
    mm2   = "01";
    for (pqxx::result::const_iterator d = R.begin(); d != R.end(); ++d) {
//       sym        = d[0].as<string>();
        tstamp     = d[1].as<string>();
        /* Convert time stamp to Julian Date */
        julTime = julianTime(tstamp);
//	cout << tstamp << " " << julTime << " " << jd_min << " " << julTime - jd_min << endl;
        if (tstamp.substr(11,5)==hhmm) {
            time = julTime - jd_min;
            plotLine(xyPlot, Qt::blue, false, time, val_min, time, val_max);
        } else {
//             if (tstamp.substr(14,2)==mm1) {
//                 time = julTime - jd_min;
//                 plotLine(xyPlot, Qt::white, false, time, val_min, time, val_max);
//             }
       	}
//      tlast      = d[2].as<string>();
        last_trade = d[3].as<float>();
//      ask        = d[4].as<float>();
//      bid        = d[5].as<float>();
//      volume     = d[6].as<int>();
        xyData->addPoint(julTime - jd_min,last_trade);
    }
    xyPlot->addPlotObject(xyData);
    
    return 0;

}

int PlotGraph::plotLine(KPlotWidget* xyPlot, const QColor &Color, bool val, double &x1,float &y1, double &x2, float &y2) {

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
