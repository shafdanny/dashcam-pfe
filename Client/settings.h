#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>

class Settings : public QObject
{
    Q_OBJECT
    static Settings* instance;
    QSettings* settings;
public:
    QString ipAddress;
    int portClient, portServer;
    explicit Settings(QObject *parent = 0);
    static Settings* getInstance();
    void saveSettings();

signals:

public slots:
};

#endif // SETTINGS_H
