#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QHostAddress>
#include <QMessageBox>
#include "settings.h"

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT
    QString ipAdress;
    int portClient, portServer;
public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    void accept();
    void reject();

private:
    Ui::SettingsDialog *ui;
};

#endif // SETTINGSDIALOG_H
