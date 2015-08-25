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
#include "plotwindow.h"
#include <QHBoxLayout>
 
PlotWindow::PlotWindow(pqxx::connection &C, QWidget *parent) : KXmlGuiWindow(parent)
{
  
    master = new QWidget();
    xyPlot = new KPlotWidget();
//    logic = new BallLogic();
    plotGraph = new PlotGraph(C, xyPlot);
    QHBoxLayout *layout = new QHBoxLayout;
//    layout->addWidget(plotGraph);
    layout->addWidget(xyPlot);
    master->setLayout(layout);
    setCentralWidget(master);
    setupGUI();
 
}
