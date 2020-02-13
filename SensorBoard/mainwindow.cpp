#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "devicemanager.h"
#include "login.h"
#include "widget.h"
#include "qcustomplot.h"
#include "calendarwidget.h"

MainWindow::MainWindow(QWidget *parent, int manager, QString name)
    : QMainWindow(parent)
    , manager(manager)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    if(manager == 0){
        ui->btn_manager->hide();
        ui->lbl_Manager->setText(name + "(일반사용자)");
    }
    else if(manager == 1 ){
        ui->lbl_Manager->setText(name + "(관리자)");
    }

    addMacAdress();
}

void MainWindow::addMacAdress()
{
    QSqlQuery qry;
    qry.exec("select name from device");
    while(qry.next()){
        ui->cbb_Mac->addItem(qry.value(0).toString());
    }
    ui->cbb_Mac->setCurrentIndex(1);


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

void MainWindow::on_btn_Data_clicked()
{
    analysis();
}

void MainWindow::on_cbb_Mac_currentIndexChanged(int index)
{
    ui->cbb_Year->setCurrentIndex(0);
    const int a = ui->cbb_Year->count();
    for(int i=a-1; i>0; i--){
        ui->cbb_Year->removeItem(i);
    }
    if(index!=0){
        makePlot();
        addYear();
    }

    // 작물 이미지 보여주기
    QSqlQuery qry;
    qry.prepare("select name from crops where name = (select crops from device where macid=(select macid from device where name=:name))");
    qry.bindValue(":name", ui->cbb_Mac->currentText());
    qry.exec();
    qry.next();

    if(qry.value(0).toString()=="딸기"){
        ui->img_Crop->setStyleSheet("border-image: url(:/new/image/strawberry.png);");
    }
    else if(qry.value(0).toString()=="토마토"){
        ui->img_Crop->setStyleSheet("border-image: url(:/new/image/tomato.png);");
    }
    else if(qry.value(0).toString()=="바나나"){
        ui->img_Crop->setStyleSheet("border-image: url(:/new/image/banana.png);");
    }


}

void MainWindow::on_cbb_Year_currentIndexChanged(int index)
{
    ui->cbb_Month->setCurrentIndex(0);
    const int a = ui->cbb_Month->count();
    for(int i=a-1; i>0; i--){
        ui->cbb_Month->removeItem(i);
    }
    if(index!=0){
        addMonth();
    }
    makePlot();
}

void MainWindow::on_cbb_Month_currentIndexChanged(int index)
{
    ui->cbb_Day->setCurrentIndex(0);
    const int a = ui->cbb_Day->count();
    for(int i=a-1; i>0; i--){
        ui->cbb_Day->removeItem(i);
    }

    if(index!=0){
        addDay();
    }
    makePlot();
}

void MainWindow::on_cbb_Day_currentIndexChanged(int index)
{
    makePlot();
}

// 달력
void MainWindow::on_pushButton_clicked()
{
    CalendarWidget *cal = new CalendarWidget(nullptr, ui->cbb_Mac->currentText());
    cal->setAttribute(Qt::WA_DeleteOnClose);
    cal->Link(this);
    cal->show();
}

// 기기등록
void MainWindow::on_btn_manager_clicked()
{
    Widget* device= new Widget;
    device->setAttribute(Qt::WA_DeleteOnClose); // close 할때 동적할당 delete
    device->show();
}

// 로그아웃
void MainWindow::on_btn_Logout_clicked()
{
    this->close();
    Login* login = new Login(nullptr, false);
    login->setAttribute(Qt::WA_DeleteOnClose);
    login->show();
}
