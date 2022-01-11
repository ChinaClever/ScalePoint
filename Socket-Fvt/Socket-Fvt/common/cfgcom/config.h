#ifndef CONFIGBASH
#define CONFIGBASH
#include "cfgserial.h"


/**
 * RTU传输统计结构体
 */
struct sCount
{
    sCount() {all=ok=err=cnt=0;}
    int cnt;
    int all;
    int ok;
    int err;
};



struct sCfgItem
{
    QString hw;
    QString user;
    sCount cnts;
    sSerial coms;
    int logCount;
    uchar pcNum;
};


class Cfg : public CfgCom
{
    Cfg(QObject *parent = nullptr);
public:
    static Cfg *bulid(QObject *parent = nullptr);
    sCfgItem *item;

    void writeCnt();
    void writeCfgDev();

protected:
    void initCnt();
    void initCfgDev();
};

#endif // CONFIGBASH
