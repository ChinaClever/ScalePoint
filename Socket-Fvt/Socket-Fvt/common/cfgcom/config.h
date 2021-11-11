#ifndef CONFIGBASH
#define CONFIGBASH
#include "cfgcom.h"
#include "serialstatuswid.h"

enum eDevTypes {
    IMM=0,
    Node=1,
    Socket,

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
    sCount() {all=ok=err=cnt=0;}
    int cnt;
    int all;
    int ok;
    int err;
};


struct sSerial
{
    sSerial() {sp=pdu=ser2=nullptr;}
    SerialPort *sp; // 标准源
    SerialPort *pdu; // 串口对象
    SerialPort *ser2; // 串口对象
};

struct sCfgItem
{
    QString user;
    sCount cnts;
    sSerial coms;
    int logCount;
    uchar pcNum;
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

    void write(const QString &key, const QVariant& v, const QString &g="cfg");
    QVariant read(const QString &key, const QVariant &v = QVariant(), const QString &g="cfg");
protected:
    void initCnt();
    void initCfgDev();

private:
    CfgCom *mCfg;
};

#endif // CONFIGBASH
