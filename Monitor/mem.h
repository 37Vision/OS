#ifndef MEM_H
#define MEM_H
#include <QString>
class Mem
{
public:
    Mem();
    static QString MemTotal;
    static QString MemFree;
    static QString MemAvailable;
    static QString SwapTotal;
    static QString SwapFree;

    static void getMemTotal();
    static void getSwapTotal();
};

#endif // MEM_H
