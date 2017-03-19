#include "settings.h"

Settings::Settings(QObject *parent) : QObject(parent)
{
     settings = new QSettings(QSettings::NativeFormat, QSettings::UserScope, "SecureCam", "SecureCam");
     ipAddress = settings->value("ipAdress", "").toString();
     portClient = settings->value("portClient", 8000).toInt();
     portServer = settings->value("portServer", 4000).toInt();

}

Settings* Settings::getInstance()
{
    if (!instance)
        instance = new Settings();

    return instance;
}

Settings *Settings::instance = 0;

void Settings::saveSettings()
{
    settings->setValue("ipAdress", ipAddress);
    settings->setValue("portClient", portClient);
    settings->setValue("portServer", portServer);
}
