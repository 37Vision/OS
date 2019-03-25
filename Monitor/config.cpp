#include "config.h"

Config::Config()
{

}
Config::~Config()
{

}

QString Config::getHostname()
{
    QFile file("/etc/hostname");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug()<<"error";
        return "";
    }
    QTextStream in(&file);
    return in.readLine();
}

QString Config::getStarttime()
{
    int sec_now=QDateTime::currentDateTime().toTime_t();
    QFile file("/proc/uptime");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug()<<"error";
        return "";
    }
    QTextStream in(&file);
    QString line = in.readLine();
    QString str;
    for(int i = 0;i<line.length();i++)
    {
        if(line[i] != '.')
            str.append(line[i]);
        else
            break;
    }
    //qDebug()<<str;
    int res = sec_now - str.toInt();
    return QDateTime::fromTime_t(res).toString("yyyy-MM-dd hh:mm:ss");
}

QString Config::getSysversion()
{
    QFile file("/etc/issue");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        //qDebug()<<"error";
        return "";
    }
    QTextStream in(&file);
    return in.readLine();
}

QString Config::getCpuinfo()
{
    QFile file("/proc/cpuinfo");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"error";
        return "";
    }
    QTextStream in(&file);
    QString line = in.readLine();
    QString str;
    while (!line.isNull())
    {
        if(line.contains("model name"))
        {
            int flag = 0;
            for(int i = 0;i<line.length();i++)
            {
                if(flag == 1)
                {
                    str.append(line[i]);
                }
                if(line[i] == ':')
                {
                    i++;
                    flag = 1;
                }
            }
            break;
        }
        line = in.readLine();
    }
    return str;
}
