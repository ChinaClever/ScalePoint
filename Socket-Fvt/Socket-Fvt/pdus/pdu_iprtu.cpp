/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "pdu_iprtu.h"

#define IP_LINE_NUM 3
#define IP_RTU_ONE_LEN 49  // 单相数据长度
#define IP_RTU_THREE_LEN 58  // 三相数据长度

Pdu_IpRtu::Pdu_IpRtu(QObject *parent) : Pdu_ObjRtu(parent)
{

}

Pdu_IpRtu *Pdu_IpRtu::bulid(QObject *parent)
{
    static Pdu_IpRtu* sington = nullptr;
    if(sington == nullptr)
        sington = new Pdu_IpRtu(parent);
    return sington;
}

int Pdu_IpRtu::recvLine(int len)
{
    int ret = 0;
    switch (len/2) {
    case IP_RTU_ONE_LEN: ret = 1; break;
    case IP_RTU_THREE_LEN: ret = 3; break;
    default: qDebug() << "IP rtu recv len Err!!" << len; break;
    }

    return ret;
}


void Pdu_IpRtu::initRtuItem(sRtuItem &it)
{
    it.addr = mItem->addr;
    it.fn = 0x03;
    it.reg = 0;
    it.num = IP_RTU_ONE_LEN;// V1
#if 0
    if(mCfg->ip_version == 3) {
        it.num = IP_RTU_THREE_LEN;  // V3
    }
#endif
}


uchar *Pdu_IpRtu::getSwitch(uchar *ptr, int line, uchar *value)
{
    ptr = getShort(ptr, line, value);
    for(int i=0; i<line; ++i) {
        if(value[i] == 2) value[i] = 1; else value[i] = 0;
    }

    return ptr;
}


/**
  * 功　能：读取电参数 数据
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：12 正确
  */
int Pdu_IpRtu::recvDataV1(uchar *ptr)
{
    uchar *ret = ptr;
    ushort hz[6] = {0};
    int line = IP_LINE_NUM;

    sPduData *obj = mPduData;
    ptr =  toShort(ptr, line, obj->vol.value);
    ptr =  toShort(ptr, line, obj->cur.value);
    ptr =  toShort(ptr, line, obj->pow);
    ptr =  toShort(ptr, line, hz); obj->hz = hz[0];
    ptr =  toInt(ptr, line, obj->ele);

    obj->tem.size = obj->hum.size = 1;
    ptr =  toShort(ptr, 1, obj->tem.value);
    ptr =  toShort(ptr, 1, obj->hum.value);
    obj->tem.max[0] = 40; obj->hum.max[0] = 99;

    ptr = toThreshold(ptr, line, obj->vol);
    ptr = toThreshold(ptr, line, obj->cur);
    ptr += 8 + (2*2*line + 4); // 报警标志位

    ptr = getSwitch(ptr, 2, obj->sw);
    obj->size = getShort(ptr); ptr +=2;
    obj->version = getShort(ptr); ptr +=2;
    obj->br = getShort(ptr); ptr +=2;
    obj->reserve = getShort(ptr); ptr +=2;

    if(obj->size == 2)  obj->size = 3;
    obj->vol.size = obj->cur.size = obj->size;

    return ptr-ret;
}


/**
  * 功　能：读取电参数 数据
  * 入口参数：ptr -> 缓冲区
  * 出口参数：pkt -> 结构体
  * 返回值：12 正确
  */
int Pdu_IpRtu::recvDataV3(uchar *ptr)
{
    uchar *ret = ptr;
    int line = IP_LINE_NUM;

    sPduData *obj = mPduData;
    ptr =  toShort(ptr, line, obj->vol.value);
    ptr =  toShort(ptr, line, obj->cur.value);
    ptr =  getShort(ptr, line, obj->pf);
    ptr =  toShort(ptr, line, obj->pow);
    ptr =  toShort(ptr, line, obj->activePow);
    ptr =  toInt(ptr, line, obj->ele);
    obj->hz = getShort(ptr); ptr +=2;
    ptr = getSwitch(ptr, line, obj->sw); // 开关状态

    obj->tem.size = obj->hum.size = 1;
    ptr =  toShort(ptr, 1, obj->tem.value);
    ptr =  toShort(ptr, 1, obj->hum.value);
    obj->tem.max[0] = 40; obj->hum.max[0] = 99;

    ptr = toThreshold(ptr, line, obj->vol);
    ptr = toThreshold(ptr, line, obj->cur);
    ptr += 8 + (2*2*line + 2 + 2); // 报警

    obj->size = getShort(ptr); ptr +=2;
    obj->version = getShort(ptr); ptr +=2;
    obj->br = getShort(ptr); ptr +=2;
    ptr = toChar(ptr, 8, obj->ip);
    obj->reserve = getShort(ptr); ptr +=2;

    if(obj->size == 2)  obj->size = 3;
    obj->vol.size = obj->cur.size = obj->size;

    return ptr-ret;
}


bool Pdu_IpRtu::recvPacket(uchar *buf, int len)
{
    bool ret = true;
    int line = recvLine(len);
    switch (line) {
    case 1: recvDataV1(buf); break;
    case 3: recvDataV3(buf); break;
    default: ret = false; break;
    }

    return ret;
}

bool Pdu_IpRtu::readPduData()
{
    sRtuItem it;
    uchar recv[MODBUS_RTU_SIZE] = {0};

    initRtuItem(it);
    int len = mModbus->read(it, recv);
    return recvPacket(recv, len);
}

