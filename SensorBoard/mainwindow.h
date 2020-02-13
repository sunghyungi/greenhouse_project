#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTime>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, int manager=0, QString name="");
    ~MainWindow();
    int manager;


public slots:
    void makePlot();
    void makeTable();
    void settingPlot(double light, double minpre, double maxpre);
    void addMacAdress();
    QSqlQuery sqlData();
    void setTable();
    void curData(QSqlQuery qry);
    void calPlot(QDate date);
    void analysis();
    void addYear();
    void addMonth();
    void addDay();
private slots:
    void on_btn_Exit_clicked();

    void on_btn_Refresh_clicked();



    void on_cb_Light_clicked();

    void on_cb_Pre_clicked();

    void on_cb_Tem_clicked();

    void on_cb_Hum_clicked();

    void on_cbb_Mac_currentIndexChanged(int index);

    void on_btn_manager_clicked();

    void on_btn_Logout_clicked();

    void on_cbb_Year_currentIndexChanged(int index);

    void on_cbb_Day_currentIndexChanged(int index);

    void on_cbb_Month_currentIndexChanged(int index);

    void on_btn_Data_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
