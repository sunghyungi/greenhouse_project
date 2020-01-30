#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include "devicemanager.h"
#include "login.h"



MainWindow::MainWindow(QWidget *parent, int manager)
    : QMainWindow(parent)
    , manager(manager)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(manager == 0){
        ui->btn_manager->hide();
    }

    // tablewidget 설정
    ui->Sensor_table->setHorizontalHeaderLabels({"light", "pre", "tem", "hum", "time"}); // 열 이름 설정
    ui->Sensor_table->resizeColumnToContents(ui->Sensor_table->columnCount());    // 열 너비 설정
    ui->Sensor_table->horizontalHeader()->setStretchLastSection(true);   // 마지막 열 너비 설정



//    makePlot();
    addMacAdress();
}


void MainWindow::makePlot()
{

    QSqlQuery qry;
    qry.prepare("select light, pre, tem, hum, time from sensor where mac=:mac");
    qry.bindValue(":mac", ui->cbb_Mac->currentText());
    qry.exec();

    // generate some data:
    QVector<double> x, y, y1, y2, y3; // initialize with entries 0..100
    int i=1;
    double max = 0;
    double maxpre= 0;
    double minpre= 200000;
    while(qry.next()){
        x.push_back(i);
        y.push_back(qry.value(0).toDouble());
        y1.push_back(qry.value(1).toDouble());
        y2.push_back(qry.value(2).toDouble());
        y3.push_back(qry.value(3).toDouble());

        i++;

        if(ui->cb_Pre->isChecked()){
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

    ui->customPlot->clearGraphs();
    int idx=0;

    // light
    if(ui->cb_Light->isChecked()){
        ui->customPlot->addGraph();
        ui->customPlot->graph(idx)->setData(x, y);
        ui->customPlot->graph(idx)->setPen(QPen(Qt::black));
        ui->customPlot->graph(idx)->setName("light");
        idx++;
    }


    // temp
    if(ui->cb_Tem->isChecked()){
        ui->customPlot->addGraph();
        ui->customPlot->graph(idx)->setPen(QPen(Qt::red));
        ui->customPlot->graph(idx)->setData(x, y2);
        ui->customPlot->graph(idx)->setName("temp");
        idx++;
    }

    // hum
    if(ui->cb_Hum->isChecked()){
        ui->customPlot->addGraph();
        ui->customPlot->graph(idx)->setPen(QPen(Qt::blue));
        ui->customPlot->graph(idx)->setData(x, y3);
        ui->customPlot->graph(idx)->setName("hum");
        idx++;
    }
    // pre
    if(ui->cb_Pre->isChecked()){
        ui->customPlot->addGraph(ui->customPlot->xAxis, ui->customPlot->yAxis2);
        ui->customPlot->graph(idx)->setPen(QPen(Qt::green));
        ui->customPlot->graph(idx)->setData(x, y1);
        ui->customPlot->graph(idx)->setName("pre");
    }
    idx = 0;
    settingPlot(i, max, minpre, maxpre);
    makeTable();

}

void MainWindow::settingPlot(int i, double max, double minpre, double maxpre){

    // 축 이름
    ui->customPlot->xAxis->setLabel("time");
    ui->customPlot->xAxis->setTicks(false);



    // light, hum, tem 축
    if(ui->cb_Light->isChecked() || ui->cb_Hum->isChecked() || ui->cb_Tem->isChecked()){
        ui->customPlot->xAxis->setRange(0, i);
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

void MainWindow::makeTable()
{
    QSqlQuery qry;
    ui->Sensor_table->setRowCount(0);
    qry.prepare("select light, pre, tem, hum, time from sensor where mac =:mac");
    qry.bindValue(":mac", ui->cbb_Mac->currentText());

    qry.exec();
    while(qry.next()){
        int currentRow = ui->Sensor_table->rowCount();
        ui->Sensor_table->setRowCount(currentRow+1);
        ui->Sensor_table->setItem(currentRow, 0, new QTableWidgetItem(qry.value(0).toString()));
        ui->Sensor_table->setItem(currentRow, 1, new QTableWidgetItem(qry.value(1).toString()));
        ui->Sensor_table->setItem(currentRow, 2, new QTableWidgetItem(qry.value(2).toString()));
        ui->Sensor_table->setItem(currentRow, 3, new QTableWidgetItem(qry.value(3).toString()));
        ui->Sensor_table->setItem(currentRow, 4, new QTableWidgetItem(qry.value(4).toString()));
//        ui->Sensor_table->setItem(currentRow, 5, new QTableWidgetItem(qry.value(5).toString()));


    }
}

void MainWindow::addMacAdress()
{
    QSqlQuery qry;
    qry.exec("select distinct mac from sensor");
    while(qry.next()){
        ui->cbb_Mac->addItem(qry.value(0).toString());
   }

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_btn_Exit_clicked()
{
    this->close();
}

void MainWindow::on_btn_Refresh_clicked()
{
    makePlot();
}

void MainWindow::on_cb_Light_clicked()
{
    makePlot();
}

void MainWindow::on_cb_Pre_clicked()
{
    makePlot();
}

void MainWindow::on_cb_Tem_clicked()
{
    makePlot();
}

void MainWindow::on_cb_Hum_clicked()
{
    makePlot();
}

void MainWindow::on_cbb_Mac_currentIndexChanged(int index)
{
    if(index!=0)
        makePlot();
}

void MainWindow::on_btn_manager_clicked()
{
    DeviceManager* device = new DeviceManager;
    device->setAttribute(Qt::WA_DeleteOnClose); // close 할때 동적할당 delete
    device->show();
}

void MainWindow::on_btn_Logout_clicked()
{
    this->close();
    Login* login = new Login(nullptr, false);
    login->show();
}
