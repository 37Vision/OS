#include "mem.h"
#include <QFile>
#include <QTextStream>

Mem::Mem()
{

}

QString Mem::MemTotal="";
QString Mem::MemFree="";
QString Mem::MemAvailable="";
QString Mem::SwapTotal="";
QString Mem::SwapFree="";

void Mem::getMemTotal()
{
    QFile file("/proc/meminfo");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return ;
    }
    QTextStream in(&file);
    QString line = in.readLine();

    while (!line.isNull())
    {
        if(line.contains("MemTotal"))
        {
            for(int i = 0;i<line.length();i++)
            {
                if(line[i] >= '0' && line[i] <= '9')
                    Mem::MemTotal.append(line[i]);
            }
        }
        line = in.readLine();
    }

}

void Mem::getSwapTotal()
{
    QFile file("/proc/meminfo");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return ;
    }
    QTextStream in(&file);
    QString line = in.readLine();

    while (!line.isNull())
    {
        if(line.contains("SwapTotal"))
        {
            for(int i = 0;i<line.length();i++)
            {
                if(line[i] >= '0' && line[i] <= '9')
                    Mem::SwapTotal.append(line[i]);
            }
        }
        line = in.readLine();
    }
}
