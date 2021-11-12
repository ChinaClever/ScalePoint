#ifndef CONFIGBASH
#define CONFIGBASH
#include "cfgcom.h"
#include "serialstatuswid.h"

enum eDevTypes {
    IMM=0,

    AC = 1, // 交流
    DC,     // 直流

    Sum=0,
    Transformer, // 互感器
    Mn    // 锰铜
};


/**
 * RTU传输统计结构体
 */
struct sCount
{
    int cnt;
    int all;
    int ok;
    int err;
};

struct sErrData
{
    ushort vol;
    ushort volErr;
    ushort curErr;
    ushort powErr; // 电流误差
};

struct sSerial
{
    sSerial() {sp=src=ser=nullptr;}
    SerialPort *sp; // 标准源
    SerialPort *src; // 串口对象
    SerialPort *ser; // 串口对象
};

struct sCfgItem
{
    uchar addr;

    QString user;
    sErrData errs;
    sSerial coms;
    sCount cnts;

    int logCount;
    uchar aiMode;
    uchar pcNum;
    ushort currentNum;
};


class Cfg
{
    Cfg();
public:
    static Cfg *bulid();

    sCfgItem *item;

    QString getSerialBr(const QString &com);
    QString getSerialName(const QString &key);
    void setSerialBr(const QString &com, const QString &br);
    void setSerialName(const QString &key, const QString &v);

    QString getLoginName();
    void setLoginName(const QString &name);

    void writeCnt();
    void writeCfgDev();

    void writeErrData();
    void setCurrentNum();
    void write(const QString &key, const QVariant& v, const QString &g="cfg");
    QVariant read(const QString &key, const QVariant &v = QVariant(), const QString &g="cfg");

protected:
    void initCnt();
    bool getDate();
    void setDate();
    void initCfgDev();
    void initErrData();
    void initCurrentNum();

private:
    CfgCom *mCfg;
};

#endif // CONFIGBASH
