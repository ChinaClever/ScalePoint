#ifndef DATAPACKET_H
#define DATAPACKET_H
#include <QtCore>
#include <QColor>

#define LINE_NUM  3
#define PACK_ARRAY_SIZE LINE_NUM
#define OpSize  6

// 倍率定义
#define COM_RATE_VOL	100.0    // 电压
#define COM_RATE_CUR	1000.0    // 电流
#define COM_RATE_POW	1000.0  // 功率
#define COM_RATE_ELE	1000.0    // 电能
#define COM_RATE_PF     100.0   // 功率因数
#define COM_RATE_TEM	1.0    // 温度
#define COM_RATE_HUM	1.0    // 湿度

struct sItData
{
    uint active; // 有功
    uint reactive; // 无功
    uint apparen; // 视在
};

struct sLineData
{
    ushort vol_ed;
    ushort vol_rms;

    uint cur_ed;
    uint cur_rms;
    uint cur_peak;    
    uchar status;

    uint ele;
    uint powed;
    sItData pow;
    ushort angle; //电流与电压的线角
};


struct sBranchIt
{
    uint cur_rms;
    uint cur_peak;
};

struct sDevObj
{
    sDevObj() {size=0;}

    ushort size;
    ushort version;
    ushort status;

    ushort hz;
    ushort angles[LINE_NUM];
    sLineData lines[LINE_NUM];
    sBranchIt branchs[OpSize];
    sBranchIt neutral;
};


struct sDevType
{
    sDevType() {devId=devType=lines=0;}

    uchar addr;
    uchar devId;
    uchar lines;
    uchar outputs;
    uchar devType; // 0 SI-PDU  1 IP-PDU
    uchar neutral;

    QString dt;
    QString sn;    
    QString hw;
    QString fw;
    QString pn;    
};



/**
 * 设备数据结构体：
 */
struct sDevData
{
    sDevType dt; //设备类型
    sDevObj data;
};


enum {
    Test_Fun, // 功能
    Test_Start, // 开始
    Test_Collect,
    Test_Ading,
    Test_vert, // 验证
    Test_Bs, // 验证
    Test_Over, // 终止
    Test_End, // 完成

    Test_Info=0,
    Test_Pass=1,
    Test_Fail=2,
};

struct sProgress
{
    sProgress() {step=0;}

    uchar step; // 步骤
    QString time;
    QList<bool> pass;
    QStringList status;

    uchar result;    // 最终结果
    QTime startTime;
};


/**
 * 数据包
 */
class sDataPacket
{
    sDataPacket();
public:
    static sDataPacket *bulid();

    void init();
    sDevData *getDev() {return dev;}
    sProgress *getPro() {return pro;}

private:
    sDevData *dev;
    sProgress *pro;
};



#endif // DATAPACKET_H
