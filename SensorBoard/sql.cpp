#include "mainwindow.h"
#include "ui_mainwindow.h"


// 쿼리문 작성
QSqlQuery MainWindow::sqlData()
{
    QSqlQuery qry;
    int yearidx = ui->cbb_Year->currentIndex();
    int monthidx = ui->cbb_Month->currentIndex();
    int dayidx = ui->cbb_Day->currentIndex();

    if(yearidx==0){
        qry.prepare("select light, pre, tem, hum, time, year, month, day, hour, min, sec from sensor where mac=(select macid from device where name=:name)");
    }
    if(yearidx!=0){
        qry.prepare("select light, pre, tem, hum, time, year, month, day, hour, min, sec from sensor where mac=(select macid from device where name=:name) and year=:year");
    }
    if(monthidx!=0){
        qry.prepare("select light, pre, tem, hum, time, year, month, day, hour, min, sec from sensor where mac=(select macid from device where name=:name) and year=:year and month=:month");
    }
    if(dayidx!=0){
        qry.prepare("select light, pre, tem, hum, time, year, month, day, hour, min, sec from sensor where mac=(select macid from device where name=:name) and year=:year and month=:month and day=:day");
    }

    qry.bindValue(":name", ui->cbb_Mac->currentText());
    qry.bindValue(":year", ui->cbb_Year->currentText());
    qry.bindValue(":month", ui->cbb_Month->currentText());
    qry.bindValue(":day", ui->cbb_Day->currentText());
    qry.exec();

    return qry;
}

void MainWindow::addYear()
{
    QSqlQuery qry;
    qry.prepare("select distinct year from sensor where mac=(select macid from device where name=:name)");
    qry.bindValue(":name", ui->cbb_Mac->currentText());
    qry.exec();
    while(qry.next()){
        ui->cbb_Year->addItem(qry.value(0).toString());
    }
}

void MainWindow::addMonth()
{
    QSqlQuery qry;
    qry.prepare("select distinct month from sensor where mac=(select macid from device where name=:name) and year=:year");
    qry.bindValue(":name", ui->cbb_Mac->currentText());
    qry.bindValue(":year", ui->cbb_Year->currentText());
    qry.exec();
    while(qry.next()){
        ui->cbb_Month->addItem(qry.value(0).toString());
    }
}

void MainWindow::addDay()
{
    QSqlQuery qry;
    qry.prepare("select distinct day from sensor where mac=(select macid from device where name=:name) and year=:year and month=:month");
    qry.bindValue(":name", ui->cbb_Mac->currentText());
    qry.bindValue(":year", ui->cbb_Year->currentText());
    qry.bindValue(":month", ui->cbb_Month->currentText());
    qry.exec();
    while(qry.next()){
        ui->cbb_Day->addItem(qry.value(0).toString());
    }
}
