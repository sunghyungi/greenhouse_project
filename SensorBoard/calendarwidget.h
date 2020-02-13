#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QWidget>
#include "mainwindow.h"

namespace Ui {
class CalendarWidget;
}

class CalendarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarWidget(QWidget *parent = nullptr, QString mac = "");
    ~CalendarWidget();
    QString mac;

public slots:
    void calRange();

    void Link(MainWindow *w2);
private slots:
    void on_cld_Data_clicked(const QDate &date);

private:
    Ui::CalendarWidget *ui;
    MainWindow *w;
};

#endif // CALENDARWIDGET_H
