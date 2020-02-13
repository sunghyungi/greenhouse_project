#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::makeTable()
{

    ui->Sensor_table->setRowCount(0);
    QSqlQuery qry = sqlData();


    while(qry.next()){

        int currentRow = ui->Sensor_table->rowCount();
        QString time = qry.value(5).toString() + "년 " + qry.value(6).toString() + "월 " + qry.value(7).toString()
                + "일 " + qry.value(8).toString() + "시 " + qry.value(9).toString() + "분";

        ui->Sensor_table->setRowCount(currentRow+1);
        ui->Sensor_table->setItem(currentRow, 0, new QTableWidgetItem(time));
        ui->Sensor_table->setItem(currentRow, 1, new QTableWidgetItem(qry.value(0).toString()));
        ui->Sensor_table->setItem(currentRow, 2, new QTableWidgetItem(qry.value(2).toString()));
        ui->Sensor_table->setItem(currentRow, 3, new QTableWidgetItem(qry.value(3).toString()));
        ui->Sensor_table->setItem(currentRow, 4, new QTableWidgetItem(qry.value(1).toString()));
   }
   setTable();
   curData(qry);

}

void MainWindow::curData(QSqlQuery qry)
{

    qry.last();

    ui->lbl_CurTime->setText(qry.value(5).toString() + "년 " + qry.value(6).toString() + "월 " + qry.value(7).toString()
                             + "일 " + qry.value(8).toString() + "시 " + qry.value(9).toString() + "분");

    ui->lbl_LightData->setText(qry.value(0).toString()+"lx");
    ui->lbl_TemData->setText(qry.value(2).toString()+"℃");
    ui->lbl_HumData->setText(qry.value(3).toString()+"%");
    ui->lbl_PreData->setText(qry.value(1).toString()+"Pa");
    ui->lbl_LightData->setStyleSheet("QLabel {color : black; }");
    ui->lbl_TemData->setStyleSheet("QLabel {color : red; }");
    ui->lbl_HumData->setStyleSheet("QLabel {color : blue; }");
    ui->lbl_PreData->setStyleSheet("QLabel {color : green; }");

}

void MainWindow::setTable()
{
    // tablewidget 설정
    ui->Sensor_table->setHorizontalHeaderLabels({"시간", "조도", "온도", "습도", "기압"}); // 열 이름 설정

    ui->Sensor_table->setColumnWidth(0, 200); // 열 너비 설정
    ui->Sensor_table->setColumnWidth(1, 80);
    ui->Sensor_table->setColumnWidth(2, 80);
    ui->Sensor_table->setColumnWidth(3, 80);
    ui->Sensor_table->setColumnWidth(4, 80);
    ui->Sensor_table->horizontalHeader()->setStretchLastSection(true);   // 마지막 열 너비 설정
    ui->Sensor_table->setEditTriggers(QAbstractItemView::NoEditTriggers);   // 테이블 수정 불가능
}
