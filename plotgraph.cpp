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

PlotGraph::PlotGraph(KPlotWidget* xyPlot, QWidget* parent): QWidget(parent) {
//    this->logic = logic;
//    radius = 20;

    setBackgroundRole(QPalette::Base);					//Background color: this nice gradient of gray
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);	//allow for expanding of the widget, aka use the minimumSizeHint and sizeHint

    // For xy plot using KplotWidget
    this->xyPlot = xyPlot;						//set pointer
    xyPlot->setMinimumSize( 600, 600 );					//set minimum size in pixel
    xyData = new KPlotObject( Qt::blue, KPlotObject::Points, 10, KPlotObject::Star); //Bars are drawn in red with thickness 2
    xyData2 = new KPlotObject( Qt::red, KPlotObject::Lines, 1);//, KPlotObject::Square); //Bars are drawn in red with thickness 2
    xyPlot->setLimits(0, 400, 0, 400);					//the data limits are 0 to 100 in the x-direction and 0 to 600 in the y-direction (initially we start at point 590)
    xyPlot->addPlotObject(xyData);					//assign the data pointer to the graph. From now on, if the data pointer contains data, it is plotted in the graph
    xyPlot->addPlotObject(xyData2);					//assign the data pointer to the graph. From now on, if the data pointer contains data, it is plotted in the graph
    xyPlot->setBackgroundColor( QColor(240, 240, 240) );		//background: light shade of gray
    xyPlot->setForegroundColor( Qt::black );				//foreground: black
    xyPlot->axis( KPlotWidget::BottomAxis )->setLabel("Time [s]");	//set x-axis labeling
    xyPlot->axis( KPlotWidget::LeftAxis )->setLabel("Position [m]");    //set y-axis labeling
    xyPlot->setShowGrid(false);
    xyData->setShowLines( true );
//    xyData->setShowBars( true );
    xyData->setLinePen( QPen( Qt::red, 1, Qt::SolidLine ) );
    for(int x=1; x<200; x+=10) {
//        xyData->addPoint(x,1);
        xyData->addPoint(x,100);
        xyPlot->update();
//        xyData->setLinePen( QPen( Qt::red, 3.0, Qt::NoPen ) );
    }
//    update();
    xyData2->setLinePen( QPen( Qt::blue, 2, Qt::SolidLine ) );
    for(int x=1; x<200; x+=10) {
        xyData2->addPoint(x,200);
        xyData2->addPoint(x,300);
        xyPlot->update();
//        xyData->setLinePen( QPen( Qt::red, 3.0, Qt::NoPen ) );
    }
//    update();
//     QPainter painter(this);
//     painter.setRenderHint(QPainter::Antialiasing, true);
//     //draw ball
//     painter.setPen(QPen(Qt::blue, 4));
//     painter.drawEllipse(100, 100, 40, -40);
//     //draw floor
//     painter.fillRect( 10, 595, 180, -20 , QColor(40, 40, 40) );
// 
//     xyData->draw(&painter, xyPlot);
//     xyPlot->update();
//    update();

}
