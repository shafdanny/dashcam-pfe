#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    ipAdress = Settings::getInstance()->ipAddress;
    portClient = Settings::getInstance()->portClient;
    portServer = Settings::getInstance()->portServer;
    ui->lineEdit->setText(ipAdress);
    ui->spinBoxPortClient->setValue(portClient);
    ui->spinBoxPortServer->setValue(portServer);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept()
{
    ipAdress = ui->lineEdit->text();
    portClient = ui->spinBoxPortClient->value();
    portServer = ui->spinBoxPortServer->value();
    Settings::getInstance()->ipAddress = ipAdress;
    Settings::getInstance()->portClient = portClient;
    Settings::getInstance()->portServer = portServer;
    QDialog::accept();
}

void SettingsDialog::reject()
{
    ipAdress = Settings::getInstance()->ipAddress;
    portClient = Settings::getInstance()->portClient;
    portServer = Settings::getInstance()->portServer;
    ui->lineEdit->setText(ipAdress);
    ui->spinBoxPortClient->setValue(portClient);
    ui->spinBoxPortServer->setValue(portServer);
    QDialog::reject();

}
