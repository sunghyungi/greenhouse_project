#include "devicemanager.h"
#include "ui_devicemanager.h"

DeviceManager::DeviceManager(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DeviceManager)
{
    ui->setupUi(this);
}

DeviceManager::~DeviceManager()
{
    delete ui;
}

void DeviceManager::on_btn_Exit_clicked()
{
    this->close();
}
