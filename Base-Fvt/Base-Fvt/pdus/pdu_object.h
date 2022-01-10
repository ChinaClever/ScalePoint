#ifndef PDU_OBJECT_H
#define PDU_OBJECT_H

#include "baseobject.h"

#define PACK_ARRAY_SIZE 16

struct sUnitTh {
    ushort size;
    ushort value[PACK_ARRAY_SIZE]; // 值
    ushort min[PACK_ARRAY_SIZE]; // 最小值
    ushort max[PACK_ARRAY_SIZE]; // 最大值
    uchar alarm[PACK_ARRAY_SIZE]; // 报警值 0表示未报警  1表示已报警 2表示已记录
};


/**
 * 数据对象：包括电流，电压，功率，电能，开关状态，插接位名称
 */
struct sPduData
{
    int addr;
    int size;
    double rate;
    sUnitTh vol; // 电压
    sUnitTh cur; // 电流
    sUnitTh tem; // 温度
    sUnitTh hum; // 湿度

    ushort pow[PACK_ARRAY_SIZE]; // 功率
    uint ele[PACK_ARRAY_SIZE]; // 电能
    uchar pf[PACK_ARRAY_SIZE]; // 功率因数
    uchar sw[PACK_ARRAY_SIZE]; // 开关状态  0 表示未启用 1 表示开

    uchar br; //// 波特率
    uchar hz;  // 电压频率
    uchar version;
    uchar chipStatus;
    ushort activePow[PACK_ARRAY_SIZE]; // 有功功率值
    uchar status[PACK_ARRAY_SIZE];
    ushort reserve;
    uchar ip[18];
};

class Pdu_Object : public BaseThread
{
    Q_OBJECT
public:
    explicit Pdu_Object(QObject *parent = nullptr);

    void initData(int addr);
    sPduData* getPduData() {return mPduData;}
    void changeBaudRate() {mModbus->changeBaudRate();}

protected slots:
    void initFunSlot();

protected:
     sPduData *mPduData;
};

#endif // DEV_OBJECT_H
