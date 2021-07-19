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
    sCount() {all=ok=err=cnt=0;}

    int cnt;
    int all;
    int ok;
    int err;
};

struct sCfgItem
{    
    uchar modeId;
    uchar addr;

    ushort vol;
    ushort volErr, curErr, powErr; // 电流误差   

    QString user;
    sCount cnt;
    int logCount;
    uchar aiMode;
    uchar pcNum;
    ushort currentNum;

    SerialPort *com;
    SerialPort *source;
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
