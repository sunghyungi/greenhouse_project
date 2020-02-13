#include "mainwindow.h"
#include "ui_mainwindow.h"


void MainWindow::settingPlot(double max, double minpre, double maxpre){


    ui->customPlot->yAxis->setVisible(false);
    ui->customPlot->yAxis2->setVisible(false);

    // x축
    ui->customPlot->xAxis->setLabel("time");

    QSqlQuery qry = sqlData();
    qry.exec();
    qry.first();
    int first_year = qry.value(5).toInt();
    int first_month = qry.value(6).toInt();
    int first_day = qry.value(7).toInt();
    int first_hour = qry.value(8).toInt();
    int first_min = qry.value(9).toInt();
    int first_sec = qry.value(10).toInt();
    qry.last();
    int last_year = qry.value(5).toInt();
    int last_month = qry.value(6).toInt();
    int last_day = qry.value(7).toInt();
    int last_hour = qry.value(8).toInt();
    int last_min = qry.value(9).toInt();
    int last_sec = qry.value(10).toInt();

    QSharedPointer<QCPAxisTickerDateTime> dateTicker(new QCPAxisTickerDateTime);


    if(ui->cbb_Day->currentIndex()!=0){
        dateTicker->setDateTimeFormat("hh:mm:ss");
    }
    else if(ui->cbb_Month->currentIndex()!=0){
        dateTicker->setDateTimeFormat("dd일");
    }
    else if(ui->cbb_Year->currentIndex()!=0){
        dateTicker->setDateTimeFormat("MM월");
    }
    else{
        dateTicker->setDateTimeFormat("yyyy년");
    }


    ui->customPlot->xAxis->setTickLabelRotation(45);
    ui->customPlot->xAxis->setTicker(dateTicker);
    ui->customPlot->xAxis->setRange(QCPAxisTickerDateTime::dateTimeToKey(QDateTime(QDate(first_year, first_month, first_day),QTime(first_hour,first_min,first_sec)))
                                    , QCPAxisTickerDateTime::dateTimeToKey(QDateTime(QDate(last_year, last_month, last_day),QTime(last_hour, last_min, last_sec))) );

    QDateTime(QDate(2020, 1, 1));
    // light, hum, tem 축
    if(ui->cb_Light->isChecked() || ui->cb_Hum->isChecked() || ui->cb_Tem->isChecked()){
        ui->customPlot->yAxis->setVisible(true);
        ui->customPlot->yAxis->setRange(0, max+10);
        ui->customPlot->yAxis->setLabel("light, tem, hum");
    }

    // pre 축
    if(ui->cb_Pre->isChecked()){
        ui->customPlot->yAxis2->setRange(minpre-200, maxpre+200);
        ui->customPlot->yAxis2->setVisible(true);
        ui->customPlot->yAxis2->setLabel("pre");
    }

    // 범례
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    QFont legendFont = font();
    legendFont.setPointSize(9);
    ui->customPlot->legend->setFont(legendFont);
    ui->customPlot->legend->setBrush(QBrush(QColor(255,255,255,100)));
    ui->customPlot->replot();

}

void MainWindow::makePlot()
{

    QSqlQuery qry = sqlData();

    ui->customPlot->clearGraphs();
    if(ui->cbb_Mac->currentIndex()!=0){


        // generate some data:
        QVector<double>x, y, y1, y2, y3; // initialize with entries 0..100
        int i=0;
        double max = 0;
        double maxpre= 0;
        double minpre= -1;

        while(qry.next()){

            x.push_back(QCPAxisTickerDateTime::dateTimeToKey(QDateTime(QDate(qry.value(5).toInt(),qry.value(6).toInt(),qry.value(7).toInt())
                                                                     , QTime(qry.value(8).toInt(),qry.value(9).toInt(),qry.value(10).toInt()))));

            y.push_back(qry.value(0).toDouble());
            y1.push_back(qry.value(1).toDouble());
            y2.push_back(qry.value(2).toDouble());
            y3.push_back(qry.value(3).toDouble());

            i++;

            if(minpre < 0){
                maxpre = qry.value(1).toDouble();
                minpre = qry.value(1).toDouble();
            }
            else if(ui->cb_Pre->isChecked()){
                if(qry.value(1)>maxpre)
                    maxpre = qry.value(1).toDouble();

                if(qry.value(1)<minpre)
                    minpre = qry.value(1).toDouble();
            }

            if(ui->cb_Light->isChecked()){
                max = (qry.value(0).toDouble() > max) ? qry.value(0).toDouble() : max;
            }
            if(ui->cb_Tem->isChecked()){
                max = (qry.value(2).toDouble() > max) ? qry.value(2).toDouble() : max;
            }
            if(ui->cb_Hum->isChecked()){
                max = (qry.value(3).toDouble() > max) ? qry.value(3).toDouble() : max;
            }

        }
        int idx=0;

        // light
        if(ui->cb_Light->isChecked()){
            ui->customPlot->addGraph();
            ui->customPlot->graph(idx)->setData(x,y);
            ui->customPlot->graph(idx)->setPen(QPen(Qt::black));
            ui->customPlot->graph(idx)->setName("light");
            idx++;
        }

        // temp
        if(ui->cb_Tem->isChecked()){
            ui->customPlot->addGraph();
            ui->customPlot->graph(idx)->setPen(QPen(Qt::red));
            ui->customPlot->graph(idx)->setData(x,y2);
            ui->customPlot->graph(idx)->setName("tem");
            idx++;
        }

        // hum
        if(ui->cb_Hum->isChecked()){
            ui->customPlot->addGraph();
            ui->customPlot->graph(idx)->setPen(QPen(Qt::blue));
            ui->customPlot->graph(idx)->setData(x,y3);
            ui->customPlot->graph(idx)->setName("hum");
            idx++;
        }
        // pre
        if(ui->cb_Pre->isChecked()){
            ui->customPlot->addGraph(ui->customPlot->xAxis, ui->customPlot->yAxis2);
            ui->customPlot->graph(idx)->setPen(QPen(Qt::green));
            ui->customPlot->graph(idx)->setData(x,y1);
            ui->customPlot->graph(idx)->setName("pre");
        }



        settingPlot(max, minpre, maxpre);
        makeTable();
    }

}


void MainWindow::calPlot(QDate date){

    QString year = QString::number(date.year());
    QString month = QString::number(date.month());
    QString day = QString::number(date.day());

    int yidx = ui->cbb_Year->findText(year);
    ui->cbb_Year->setCurrentIndex(yidx);

    int midx = ui->cbb_Month->findText(month);
    ui->cbb_Month->setCurrentIndex(midx);

    int didx = ui->cbb_Day->findText(day);
    ui->cbb_Day->setCurrentIndex(didx);
}

