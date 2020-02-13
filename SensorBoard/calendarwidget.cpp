#include "calendarwidget.h"
#include "ui_calendarwidget.h"


CalendarWidget::CalendarWidget(QWidget *parent, QString mac) :
    QWidget(parent), mac(mac),
    ui(new Ui::CalendarWidget)
{
    ui->setupUi(this);
    calRange();
}

CalendarWidget::~CalendarWidget()
{
    delete ui;
}

void CalendarWidget::Link(MainWindow *w2){
    w = w2;
}

void CalendarWidget::calRange(){


    QSqlQuery qry;
    qry.prepare("select distinct year, month, day from sensor where mac=(select macid from device where name=:name)");
    qry.bindValue(":name",  mac);
    qry.exec();
    QTextCharFormat fm;
    fm.setBackground(Qt::yellow);
    fm.setFontWeight(QFont::Bold);

    while(qry.next())
    {
        ui->cld_Data->setDateTextFormat(QDate(qry.value(0).toInt(), qry.value(1).toInt(), qry.value(2).toInt()), fm);
    }

}

void CalendarWidget::on_cld_Data_clicked(const QDate &date)
{
    w->calPlot(date);
}
