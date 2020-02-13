#include <widget.h>
#include <ui_widget.h>

void Widget::connectButton() {
    if(ui->pb_connect->text() == "장치 접속끊기") {
        ui->pb_connect->setText("장치 접속하기");
        m_serial->close();
        return;
    }

    if(ui->pb_connect->text() == "장치 접속하기") {
        if(m_serial->isOpen()) {
            return;
        }
        QString devName = ui->cb_device->currentText().trimmed();
        m_serialSettings.portname = devName;
        m_serial->setPortName(m_serialSettings.portname);
        m_serial->setBaudRate(m_serialSettings.baudRate);
        m_serial->setDataBits(m_serialSettings.dataBits);
        m_serial->setParity(m_serialSettings.parity);
        m_serial->setStopBits(m_serialSettings.stopBits);
        m_serial->setFlowControl(m_serialSettings.flowControl);

        if(!m_serial->open(QIODevice::ReadWrite)) {
            qDebug() << "error message: " << m_serial->errorString();
        }
        ui->pb_connect->setText("장치 접속끊기");
        return;
    }
}

void Widget::exitButton() {
    db.close();
    Widget::close();
}

void Widget::insertButton() {
    QSqlQuery qry_ins;
    qry_ins.prepare("insert into test.device (macid, name, locate, crops) values(:macid, :name, :locate, :crops)");
    qry_ins.bindValue(":macid", ui->le_mac->text());
    qry_ins.bindValue(":name", ui->le_id->text());
    qry_ins.bindValue(":locate", ui->le_loc->text());
    qry_ins.bindValue(":crops", ui->le_crop->text());
    qry_ins.exec();
    qmsg.information(this, "등록완료", "등록완료");
    showTable();
}

void Widget::updateButton() {
    QSqlQuery qry_upd;
    qry_upd.prepare("update test.device set name=:name, locate=:locate, crops=:crops where macid=:macid");
    qry_upd.bindValue(":name", ui->le_id->text());
    qry_upd.bindValue(":locate", ui->le_loc->text());
    qry_upd.bindValue(":crops", ui->le_crop->text());
    qry_upd.bindValue(":macid", ui->le_mac->text());
    qry_upd.exec();
    qmsg.information(this, "수정완료", "수정완료");
    showTable();
}

void Widget::deleteButton() {
    QSqlQuery qry_del;
    qry_del.prepare("delete from test.device where macid=:macid");
    qry_del.bindValue(":macid", ui->le_mac->text());
    qry_del.exec();
    qmsg.information(this, "삭제완료", "삭제완료");
    showTable();
}

void Widget::initeditorButton() {
    ui->le_mac->clear();
    ui->le_id->clear();
    ui->le_loc->clear();
    ui->le_crop->clear();
}
