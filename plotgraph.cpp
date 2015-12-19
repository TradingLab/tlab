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
    string symbol, symbolVal, sql, tstamp;
    float th_buy, th_sell;
    int numBuy, numSell;
    int num_items;
    bool scanner = false;

    /* A week trading period */
//  EURUSD=X - US Dollar
//  EURGBP=X - British Pound
//  EURAUD=X - Australian Dollar
//  EURJPY=X - Japanese Yen
//  EURCHF=X - Swiss Franc
//  EURSEK=X - Swedish Krona
//  EURCAD=X - Canadian Dollar
//  EURNZD=X - New Zealand Dollar
//  EURSGD=X - Singapore Dollar
//  EURNOK=X - Norwegian Krone

    symbol = "'EURUSD=X'";
    double timeRangeLen = 4; //days with decimals
    int numBuySell = 40;
    
    /* Time range calculation */
    if (scanner) {
//        ini_tstamp = "'2015-08-30 21:00:00'"; //Week 36
//        end_tstamp = "'2015-09-04 21:00:00'";
   ini_tstamp = "'2015-09-06 21:00:00'"; //Week 37
   end_tstamp = "'2015-09-11 21:00:00'";
//    ini_tstamp = "'2015-09-13 21:00:00'"; //Week 38
//    end_tstamp = "'2015-09-18 21:00:00'";
//    ini_tstamp = "'2015-09-20 21:00:00'"; //Week 39
//    end_tstamp = "'2015-09-25 21:00:00'";
//    ini_tstamp = "'2015-09-27 21:00:00'"; //Week 40
//    end_tstamp = "'2015-10-02 21:00:00'";
    } else {
        plotTimeRange(timeRangeLen);
    }
    cout << "Symbol: " << symbol << endl;
    cout << "Quotes period: " << ini_tstamp << " to " << end_tstamp << endl;

    /* Create SQL statement */
    sql = "SELECT * FROM quotes WHERE symbol = " + \
          symbol + " AND tstamp >=" + ini_tstamp +\
          "AND  tstamp <=" + end_tstamp +\
          " ORDER BY tstamp";

    /* Create a non-transactional object. */
    pqxx::nontransaction N(C);

    /* Execute SQL query */
    pqxx::result R( N.exec( sql ));

    if (scanner) {
//        plotScanner(timeRangeLen);
    }

    plotArea(R, &num_items);
    cout.precision(10);
    
    cout << "Period in Julian time: " << jd_ini << " to " << jd_end << endl;
    cout <<  "Num. items: " << num_items << endl;
    cout << "Min/max time in Julian time: " << jd_min << "/" << jd_max << endl;
    std::cout << "Quote Value from " << val_min << " to " << val_max << std::endl;

    setBackgroundRole(QPalette::Base);					//Background color: this nice gradient of gray
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);	//allow for expanding of the widget, aka use the minimumSizeHint and sizeHint

    symbolVal = "Value " + symbol;
// For xy plot using KplotWidget
    this->xyPlot = xyPlot;						//set pointer
    xyPlot->setMinimumSize( 600, 600 );					//set minimum size in pixel
    xyPlot->setLimits(0, jd_end - jd_ini, val_min, val_max);			//
    xyPlot->setBackgroundColor( QColor(240, 240, 240) );		//background: light shade of gray
    xyPlot->setForegroundColor( Qt::black );				//foreground: black
    xyPlot->axis( KPlotWidget::BottomAxis )->setLabel("Time [Julian Day]");	//set x-axis labeling
    xyPlot->axis( KPlotWidget::LeftAxis )->setLabel(symbolVal.c_str());	//set y-axis labeling
    xyPlot->setShowGrid(true);
    xyPlot->antialiasing();

    plotCalcTh(R, numBuySell, &th_buy, &th_sell);
    /*  plotMinMaxPrices Parametes */
//    th_buy  = 0.0018;  //Buy profit price limit
//    th_sell = 0.0018;  //Sell profit price limit
    bool plot = true;
    plotMinMaxPrices(R, xyPlot, plot, th_buy, th_sell, &numBuy, &numSell);

    /* Plot Texts*/
    plotPutTextBoard(xyPlot, symbol, th_buy, val_min, val_max);
    /* Plot prices chart */
    plotPrices(R, xyPlot);

    xyPlot->update();
    xyPlot->show();
}

/* p l o t S c a n n e r */
// int PlotGraph::plotScanner(double &range_len) {
// 
// 
//     return 0;
// }


/* p l o t P u t T e x t B o a r d */
int PlotGraph::plotPutTextBoard(KPlotWidget* xyPlot, std::string& symbol, float & th_buy, float &val_min, float &val_max) {
    stringstream ss1 (stringstream::in | stringstream::out);
    string gd_ini, gd_end;
    string text = "Symbol: " + symbol;;
    int posx = 20, posy = 1;

    plotPutText(xyPlot, text, posx, posy, val_min, val_max);

    posy = 2;
    text = "Init. time: " + ts_ini;
    plotPutText(xyPlot, text, posx, posy, val_min, val_max);

    posy = 3;
    text = "End time: " + ts_end;
    plotPutText(xyPlot, text, posx, posy, val_min, val_max);

    posy = 4;
    ss1 << th_buy;
    text = ss1.str();
    text = "Bid/ask gain margin: " + text;
    plotPutText(xyPlot, text, posx, posy, val_min, val_max);

    return 0;
}

/* p l o t C a l c T h */
int PlotGraph::plotCalcTh(pqxx::result &R, int &numBuySell, float* th_buy, float* th_sell) {
    KPlotWidget xy;
    int numBuy = 0, numSell = 0, numTot;
    bool plot = false;
//     double time;
//     double t_min, t_th_min;
//     double t_max, t_th_max;
    float th, tht;

    th = (val_max - val_min);
    cout << "Num buy/sell: " << numBuySell << endl;
    tht = th;
//    th = th / 2;
    th = th / 40;
//    while(true) {
    for (tht= val_max - val_min; tht > 0; tht = tht - th) {
        plotMinMaxPrices(R, &xy, plot, tht, tht, &numBuy, &numSell);
        numTot = numBuy + numSell;
// 	if (numTot < numBuySell) {
// 	   tht -= th;
// 	}
// 	if (numTot > numBuySell) {
// 	   tht += th;
// 	}
        cout << "=> Th=" << tht << " " << numTot << "/" << numBuySell << endl;
        if (numTot >= numBuySell) {
            break;
        }
// 	th = th / 2;
    }
//
    *th_buy = *th_sell = tht;
//
    return 0;
}

/* p l o t M i n M a x P r i c e s */
int PlotGraph::plotMinMaxPrices(pqxx::result &R, KPlotWidget* xyPlot, bool &plot, float &th_buy, float &th_sell, int *numBuy, int *numSell) {
    double t_ini, time;
    double t_min, t_th_buy;
    double t_max, t_th_sell;
    int nBuy = 0, nSell = 0;
    float price, ini_price, first_price; //, last_price;
    float max_price, thMaxPrice;
    float min_price, thMinPrice;
    string tstamp;
//    double jd;
    bool firstMin = false;

    double x1, x2;
    float y1, y2;

    /* Search order, first min or max */
    if (jd_min < jd_max) {
        firstMin = true;
    }

    /* First price in selected data */
    pqxx::result::const_iterator c_ini = R.begin();
    first_price = c_ini[3].as<float>();

//     /* Last price in selected data */
//     for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
//         last_price = c[3].as<float>();
//     }

    t_ini     = jd_ini; // Begining time
    t_min     = jd_ini; // Time range t_ini to t_min
    t_max     = jd_ini; // Time range t_ini to t_min

    ini_price = first_price;
    min_price = first_price; //Value range ini_price to min_price
    max_price = first_price; //Value range ini_price to min_price


    while (true) {

        if (firstMin) {

            /* Evaluate buying price */
            thMinPrice = th_buy;
            t_th_buy = 0;
            min_price = ini_price;
            for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
                tstamp     = c[1].as<string>();
                time = julianTime(tstamp);
                if (time > t_ini) {
                    price = c[3].as<float>();
                    if (price <= min_price) {
                        t_min = time;
                        min_price = price;
                        thMinPrice = min_price + th_buy;
                    } else {
                        if (price >= thMinPrice) {
                            t_th_buy = time;
                            break;
                        }
                    }
                }
            }
            x1 = t_ini - jd_ini;
            x2 = t_min - jd_ini;
            y1 = ini_price;
            y2 = min_price;
            if (plot) plotLine(xyPlot,Qt::red, true, x1 , y1, x2, y2);
            nBuy += 1;
            if (t_th_buy > 0) {
                x1 = t_min - jd_ini;
                x2 = t_th_buy - jd_ini;
                y1 = min_price;
                y2 = thMinPrice;
                if (plot) plotLine(xyPlot,Qt::cyan,false,x1, y1, x2, y2);
            }

            t_ini     = t_min;
            ini_price = min_price;

            if (t_min >= jd_end) {
                break;
            }
        }


        /* Evaluate selling price */
        firstMin = true;

        thMaxPrice = - th_sell;
        t_th_sell = 0;
        max_price = ini_price;
        for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
            tstamp     = c[1].as<string>();
            time = julianTime(tstamp);
            if (time > t_ini) {
                price = c[3].as<float>();
                if (price >= max_price) {
                    t_max = time;
                    max_price = price;
                    thMaxPrice = max_price - th_sell;
                } else {
                    if (price <= thMaxPrice) {
                        t_th_sell = time;
                        break;
                    }
                }
            }
        }

        x1 = t_ini - jd_ini;
        x2 = t_max - jd_ini;
        y1 = ini_price;
        y2 = max_price;
        if (plot) plotLine(xyPlot,Qt::green,true, x1, y1, x2, y2);
        nSell += 1;
        if (t_th_sell > 0) {
            x1 = t_max - jd_ini;
            x2 = t_th_sell - jd_ini;
            y1 = max_price;
            y2 = thMaxPrice;
            if (plot) plotLine(xyPlot,Qt::cyan,false,x1,y1,x2,y2);
        }

        t_ini     = t_max;
        ini_price = max_price;

        cout.precision(15);
        if (t_max >= jd_end) {
            break;
        }
        *numBuy = nBuy;
        *numSell = nSell;
    }

    return 0;

}

/* p l o t T i m e R a n g e */
int PlotGraph::plotTimeRange(double &range_len) {
    /* Present time calc, ends in the present */
    char buf[80];
    string s1, s2;
    double julTime, ini_time;

    time_t t = time(0);
    struct tm *tm = localtime( &t );
    strftime(buf, sizeof(buf), "%Y-%m-%d %X", tm);
    s1 = buf;
    s2 = "'" + s1 + "'";
    end_tstamp = s2;

    /* Calculate initial time stamp */
    julTime = julianTime(s1);
    ini_time = julTime - range_len;
    s1 = GregorianTime(ini_time);
    s2 = "'" + s1 + "'";
    ini_tstamp = s2;

    return 0;
}

/* p l o t A r e a */
int PlotGraph::plotArea(pqxx::result &R, int *num_items) {
    string tstamp_ini, tstamp_end;
    string tstamp_min, tstamp_max;
    float vmin = 0, vmax = 0, val;
//    double julTime;
    int n_items = 0;

    /* First price in selected data */
    pqxx::result::const_iterator c_ini = R.begin();
    tstamp_ini = c_ini[1].as<string>();
    vmin = vmax = c_ini[3].as<float>();

    /* Last price in selected data */
    for (pqxx::result::const_iterator c = R.begin(); c != R.end(); ++c) {
//        cout << c[1].as<string>() << endl;
        val = c[3].as<float>();
        tstamp_end = c[1].as<string>();
        if (val < vmin) {
            tstamp_min = c[1].as<string>();
            vmin = val;
        }
        if (val > vmax) {
            tstamp_max = c[1].as<string>();
            vmax = val;
        }
        n_items += 1;
    }

    ts_ini = tstamp_ini;
    ts_end = tstamp_end;

    /* Convert time stamp to Julian Date */
    jd_ini = julianTime(tstamp_ini);
    jd_end  = julianTime(tstamp_end);
    jd_min = julianTime(tstamp_min);
    jd_max = julianTime(tstamp_max);

    val_min = vmin;
    val_max = vmax;

    *num_items = n_items;

    return 0;
}

/* j u l i a n T i m e */
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

/* G r e g o r i a n T i m e */
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

/* p l o t P u t T e x t */
int PlotGraph::plotPutText(KPlotWidget* xyPlot, string & text, int &posx, int &posy, float &val_min, float &val_max) {
    KPlotObject *xyData;
    double ini_jd, end_jd, jd;
    double x;//>
    float y; //^

    ini_jd = julianTime(ts_ini);
    end_jd = julianTime(ts_end);

    jd = end_jd - ini_jd;
    x  = posx * jd/40;

    y = val_max - posy * (val_max - val_min)/40;
    xyData  = new KPlotObject( Qt::black, KPlotObject::Points, 0);
    xyData->addPoint(x,y, text.c_str());

    xyPlot->addPlotObject(xyData);

    return 0;
}

/* p l o t P r i c e s */
int PlotGraph::plotPrices(pqxx::result &R, KPlotWidget* xyPlot) {
    KPlotObject *xyData;
    double julTime, time;
    string hhmm;
    double x;
    float y;
    string tstamp, old_tstamp;
    float  last_trade;

    xyData  = new KPlotObject( Qt::blue, KPlotObject::Lines, 1);
    hhmm = "00:00";
    for (pqxx::result::const_iterator d = R.begin(); d != R.end(); ++d) {
//       sym        = d[0].as<string>();
        tstamp     = d[1].as<string>();
        /* Convert time stamp to Julian Date */
        julTime = julianTime(tstamp);
        if (tstamp.substr(11,5)==hhmm) {
            time = julTime - jd_min;
            plotLine(xyPlot, Qt::yellow, false, time, val_min, time, val_max);
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
        x = julTime - jd_ini;
        y = last_trade;
        xyData->addPoint(x, y);
    }
    xyPlot->addPlotObject(xyData);

    return 0;

}

/* p l o t L i n e */
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
