#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr, int manager=0);
    ~MainWindow();
    int manager;

public slots:
    void makePlot();
    void makeTable();
    void settingPlot(int i, double light, double minpre, double maxpre);
    void addMacAdress();
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

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
