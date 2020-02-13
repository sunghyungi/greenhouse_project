#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::analysis()
{
    if(ui->cbb_Mac->currentIndex()==0)
    {
         QMessageBox::information(this, "Error", "디바이스를 선택해주세요.");
         return;
    }
    else if(ui->cbb_Sensor->currentIndex()==0)
    {
        QMessageBox::information(this, "Error", "센서를 선택해주세요.");
        return;
    }

    QSqlQuery qry = sqlData();
    qry.exec();
    int i;
    if(ui->cbb_Sensor->currentIndex()==1) i = 0;
    else if(ui->cbb_Sensor->currentIndex()==2) i = 1;
    else if(ui->cbb_Sensor->currentIndex()==3) i = 2;
    else i = 3;

    qry.next();
    double min = qry.value(i).toDouble();
    double max = qry.value(i).toDouble();
    double sum = qry.value(i).toDouble();
    double count = 1;
    while(qry.next()){
        if(qry.value(i).toDouble() > max) max = qry.value(i).toDouble();
        if(qry.value(i).toDouble() < min) min = qry.value(i).toDouble();
        sum += qry.value(i).toDouble();
        count++;
    }
    QString maxs = QString::number(max);
    QString mins = QString::number(min);
    QString means = QString::number(sum/count);


    QSqlQuery qry2;
    qry2.prepare("select name, pro_light, pro_pre, pro_tem, pro_hum from crops "
                 "where name = (select crops from device where macid=(select macid from device where name=:name))");
    qry2.bindValue(":name", ui->cbb_Mac->currentText());
    qry2.exec();
    qry2.next();


    // 적정값 그리기(점선)
    QCPItemStraightLine *infLine = new QCPItemStraightLine(ui->customPlot);
    QPen DotPen;
    DotPen.setStyle(Qt::DotLine);
    DotPen.setWidthF(2);
    infLine->point1->setAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);
    infLine->point2->setAxes(ui->customPlot->xAxis, ui->customPlot->yAxis);


    if(i==0){      // 조도
        ui->le_Mean->setText(means + " lx");
        ui->le_Min->setText(mins + " lx");
        ui->le_Max->setText(maxs + " lx");
        ui->le_Pro->setText(qry2.value(1).toString() + " lx");

        ui->cb_Light->setCheckState(Qt::CheckState::Checked);
        ui->cb_Pre->setCheckState(Qt::CheckState::Unchecked);
        ui->cb_Tem->setCheckState(Qt::CheckState::Unchecked);
        ui->cb_Hum->setCheckState(Qt::CheckState::Unchecked);

        DotPen.setColor(Qt::black);
        infLine->setPen(DotPen);
        infLine->point1->setCoords(0, qry2.value(1).toDouble());
        infLine->point2->setCoords(count, qry2.value(1).toDouble());

    }
    else if(i==1){   // 기압
        ui->le_Mean->setText(means + " Pa");
        ui->le_Min->setText(mins + " Pa");
        ui->le_Max->setText(maxs + " Pa");
        ui->le_Pro->setText(qry2.value(2).toString() + " Pa");

        ui->cb_Light->setCheckState(Qt::CheckState::Unchecked);
        ui->cb_Pre->setCheckState(Qt::CheckState::Checked);
        ui->cb_Tem->setCheckState(Qt::CheckState::Unchecked);
        ui->cb_Hum->setCheckState(Qt::CheckState::Unchecked);

        DotPen.setColor(Qt::green);
        infLine->setPen(DotPen);
        infLine->point1->setAxes(ui->customPlot->xAxis, ui->customPlot->yAxis2);
        infLine->point2->setAxes(ui->customPlot->xAxis, ui->customPlot->yAxis2);
        infLine->point1->setCoords(0, qry2.value(2).toDouble());
        infLine->point2->setCoords(count, qry2.value(2).toDouble());
    }
    else if(i==2){   // 온도
        ui->le_Mean->setText(means + " ℃");
        ui->le_Min->setText(mins + " ℃");
        ui->le_Max->setText(maxs + " ℃");
        ui->le_Pro->setText(qry2.value(3).toString() + " ℃");

        ui->cb_Light->setCheckState(Qt::CheckState::Unchecked);
        ui->cb_Pre->setCheckState(Qt::CheckState::Unchecked);
        ui->cb_Tem->setCheckState(Qt::CheckState::Checked);
        ui->cb_Hum->setCheckState(Qt::CheckState::Unchecked);

        DotPen.setColor(Qt::red);
        infLine->setPen(DotPen);
        infLine->point1->setCoords(0, qry2.value(3).toDouble());
        infLine->point2->setCoords(count, qry2.value(3).toDouble());
    }
    else if(i==3){   // 습도
        ui->le_Mean->setText(means + " %");
        ui->le_Min->setText(mins + " %");
        ui->le_Max->setText(maxs + " %");
        ui->le_Pro->setText(qry2.value(4).toString() + " %");

        ui->cb_Light->setCheckState(Qt::CheckState::Unchecked);
        ui->cb_Pre->setCheckState(Qt::CheckState::Unchecked);
        ui->cb_Tem->setCheckState(Qt::CheckState::Unchecked);
        ui->cb_Hum->setCheckState(Qt::CheckState::Checked);

        DotPen.setColor(Qt::blue);
        infLine->setPen(DotPen);
        infLine->point1->setCoords(0, qry2.value(4).toDouble());
        infLine->point2->setCoords(count, qry2.value(4).toDouble());
    }
    makePlot();
    ui->customPlot->removeItem(infLine);
}
