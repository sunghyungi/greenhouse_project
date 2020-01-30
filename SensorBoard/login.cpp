#include "login.h"
#include "ui_login.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include "mainwindow.h"

Login::Login(QWidget *parent, bool dbcon) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    // db 연결
    if(dbcon){
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("192.168.10.156");
        db.setUserName("node");
        db.setPassword("node");
        db.setDatabaseName("test");

        if(db.open()) {
            QMessageBox::information(this, "Connection", "Database Connected Sucess");
        }else{
            QMessageBox::information(this, "Not Connection", "Database is Not Connected");
        }
    }
}

Login::~Login()
{
    delete ui;
}

void Login::on_btn_ExitLogin_clicked()
{
    this->close();
}

void Login::on_btn_Login_clicked()
{
    QSqlQuery qry;
    qry.exec("select ID, PASSWORD, ISMANAGER from manager");

    while(qry.next()){
        if(ui->le_ID->text() == qry.value(0))
        {
            if(ui->le_Password->text() == qry.value(1)){
                QMessageBox::information(this, "Login", "로그인에 성공하셨습니다.");
                this->close();
                MainWindow* w = new MainWindow(nullptr,qry.value(2).toInt());
                w->setAttribute(Qt::WA_DeleteOnClose);
                w->show();
                return;
            }
            QMessageBox::information(this, "Login", "비밀번호가 맞지 않습니다.");
            return;
        }
    }
    QMessageBox::information(this, "Login", "일치하는 아이디가 존재하지 않습니다.");

}
