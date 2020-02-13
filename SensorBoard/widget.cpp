#include "widget.h"
#include "ui_widget.h"
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    showTable();

    ui->tb_list->setHorizontalHeaderLabels({"mac", "id", "locate", "crops"});
    ui->tb_list->resizeColumnToContents(ui->tb_list->columnCount());
    ui->tb_list->horizontalHeader()->setStretchLastSection(true);

    connect(ui->tb_list, SIGNAL(clicked(const QModelIndex &)), this, SLOT(tableClicked(const QModelIndex &)));

    connect(ui->pb_ins, SIGNAL(pressed()), this, SLOT(insertButton()));
    connect(ui->pb_upd, SIGNAL(pressed()), this, SLOT(updateButton()));
    connect(ui->pb_del, SIGNAL(pressed()), this, SLOT(deleteButton()));
    connect(ui->pb_init, SIGNAL(pressed()), this, SLOT(initeditorButton()));

    connect(ui->pb_connect, SIGNAL(pressed()), this, SLOT(connectButton()));
    connect(ui->pb_exit, SIGNAL(pressed()), this, SLOT(exitButton()));

    m_serialSettings.baudRate = 9600;
    m_serialSettings.dataBits = QSerialPort::Data8;
    m_serialSettings.parity = QSerialPort::NoParity;
    m_serialSettings.stopBits = QSerialPort::OneStop;
    m_serialSettings.flowControl = QSerialPort::NoFlowControl;

    const auto infos = QSerialPortInfo::availablePorts();

    for(const QSerialPortInfo &info: infos) {
        ui->cb_device->addItem(info.portName());
    }

    m_serial = new QSerialPort(this);
    connect(m_serial, SIGNAL(error(QSerialPort::SerialPortError)),
            this, SLOT(error(QSerialPort::SerialPortError)));
    connect(m_serial, SIGNAL(readyRead()), this, SLOT(readData()));
}

void Widget::error(QSerialPort::SerialPortError err) {
    if(err == QSerialPort::ResourceError) {
        qDebug() << "Critical Error: " << m_serial->errorString();
        m_serial->close();
    }
}

void Widget::readData() {
    const QString data = m_serial->readAll();
    if(data == "\r" || data == "\n") return;
    ui->le_mac->insert(data);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::showTable() {
    QSqlQuery qry;
    ui->tb_list->setRowCount(0);
    qry.prepare("select macid, name, locate, crops from device");

    qry.exec();
    while(qry.next()) {
        int currentRow = ui->tb_list->rowCount();
        ui->tb_list->setRowCount(currentRow + 1);
        ui->tb_list->setItem(currentRow, 0, new QTableWidgetItem(qry.value(0).toString()));
        ui->tb_list->setItem(currentRow, 1, new QTableWidgetItem(qry.value(1).toString()));
        ui->tb_list->setItem(currentRow, 2, new QTableWidgetItem(qry.value(2).toString()));
        ui->tb_list->setItem(currentRow, 3, new QTableWidgetItem(qry.value(3).toString()));
    }
}

void Widget::tableClicked(const QModelIndex &index) {
    if(index.isValid()) {
        ui->le_mac->setText(ui->tb_list->item(index.row(), 0)->text());
        ui->le_id->setText(ui->tb_list->item(index.row(), 1)->text());
        ui->le_loc->setText(ui->tb_list->item(index.row(), 2)->text());
        ui->le_crop->setText(ui->tb_list->item(index.row(), 3)->text());
    }
}
