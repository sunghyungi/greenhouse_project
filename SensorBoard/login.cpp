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
    qry.prepare("select ID, pass=password(:pass), ISMANAGER, name from manager");
    qry.bindValue(":pass", ui->le_Password->text());
    qry.exec();

    while(qry.next()){
        if(ui->le_ID->text() == qry.value(0))
        {
            if(qry.value(1)==1){
                QMessageBox::information(this, "Login", "로그인에 성공하셨습니다.");
                this->close();
                MainWindow* w = new MainWindow(nullptr,qry.value(2).toInt(), qry.value(3).toString());
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
