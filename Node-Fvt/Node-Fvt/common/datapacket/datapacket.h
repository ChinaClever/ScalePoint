#ifndef DATAPACKET_H
#define DATAPACKET_H
#include <QtCore>
#include <QColor>


struct sDevType
{
    QString img;
    QString user;
    QString pwd;
    QString ip;
    bool aiFind;

    QString sn;
    QString model;
    QString manufacturer;
    QString hwRevision;
    QString fwRevision;
    QString ctrlBoardSerial;
    QString macAddress;
    QString serialNumber;
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
    Test_Start, // 开始
    Test_Set,
    Test_Secure,
    Test_Collect,
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
