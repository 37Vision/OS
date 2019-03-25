#ifndef CONFIG_H
#define CONFIG_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
class Config
{

public:
    Config();
    virtual ~Config();
    static QString getHostname();
    static QString getStarttime();
    static QString getSysversion();
    static QString getCpuinfo();
};

#endif // CONFIG_H
