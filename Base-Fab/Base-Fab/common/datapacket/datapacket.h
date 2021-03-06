#ifndef DATAPACKET_H
#define DATAPACKET_H
#include <QtCore>
#include <QColor>

struct sDevType
{
    sDevType() {outputs=2;}

    uchar addr;
    uchar devType;
    uchar outputs;

    QString id;
    QString sn; // IEEE
    QString fw;
    QString boot;
    QString protocol;
    QString zigbee;

    QString pn; // 981225
    QString dt;
    QString code;
};


/**
 * 设备数据结构体：
 */
struct sDevData
{
    sDevType dt; //设备类型
};


enum {
    Test_Fun, // 功能
    Test_Factory,
    Test_Start, // 开始
    Test_Bootloader,
    Test_Firmware,
    Test_Token,
    Test_Fvt,
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
