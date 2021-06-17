/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "dev_immrtu.h"

#define IMM_RTU_DC_LEN  5
#define IMM_RTU_THREE_LEN 10

Dev_ImmRtu::Dev_ImmRtu(QObject *parent) : Dev_Object(parent)
{   
}

Dev_ImmRtu *Dev_ImmRtu::bulid(QObject *parent)
{
    static Dev_ImmRtu* sington = nullptr;
    if(sington == nullptr)
        sington = new Dev_ImmRtu(parent);
    return sington;
}

void Dev_ImmRtu::initRtuItem(sRtuItem &it)
{
    it.addr = mItem->addr;  /////========
    it.fn = 0x03;
    it.reg = 0;
    it.num = 10;
}

int Dev_ImmRtu::recvLine(int len)
{
    int ret = 0;
    switch (len) {
    case IMM_RTU_DC_LEN: ret = 1; break;
    case IMM_RTU_THREE_LEN: ret = 3; break;
    default: qDebug() << "SI rtu recv len Err!!" << len; break;
    }

    return ret;
}


bool Dev_ImmRtu::recvPacket(uchar *buf, int len)
{
    bool ret = true;
    int line = recvLine(len);

    ///////////===========
    //if(line) getDevData(buf, line);
    //else ret = false;

    return ret;
}

bool Dev_ImmRtu::readPduData()
{
    sRtuItem it;
    uchar recv[MODBUS_RTU_SIZE] = {0};

    initRtuItem(it);
    int len = mModbus->read(it, recv);
    return recvPacket(recv, len);
}



uchar *Dev_ImmRtu::toItem(uchar *ptr, sItData &it)
{
    ptr = toInt(ptr, it.active);
    ptr = toInt(ptr, it.reactive);
    ptr = toInt(ptr, it.apparen);

    return ptr;
}

uchar *Dev_ImmRtu::toLine(uchar *ptr, sLineData &line)
{
    ptr = toShort(ptr, line.vol_rms);
    ptr = toInt(ptr, line.cur_rms);
    ptr = toInt(ptr, line.cur_peak);
    ptr = toItem(ptr, line.pow);
    ptr = toItem(ptr, line.ele);
    ptr = toShort(ptr, line.angle);
    return ptr;
}

uchar *Dev_ImmRtu::getLines(uchar *ptr, sDevObj *obj)
{
    for(int i=0; i<LINE_NUM; ++i) {
        ptr = toLine(ptr, obj->lines[i]);
    }

    return ptr;
}

uchar *Dev_ImmRtu::toAngles(uchar *ptr, sDevObj *obj)
{
    for(int i=0; i<2; ++i) {
        ptr = toShort(ptr, obj->angles[i]);
    }

    return ptr;
}

uchar *Dev_ImmRtu::toBranchIt(uchar *ptr, sBranchIt &it)
{
    ptr = toInt(ptr, it.cur_rms);
    ptr = toInt(ptr, it.cur_peak);
    return ptr;
}

uchar *Dev_ImmRtu::getBranchs(uchar *ptr, sDevObj *obj)
{
    for(int i=0; i<OpSize; ++i) {
        ptr = toBranchIt(ptr, obj->branchs[i]);
    }
    return ptr;
}

void Dev_ImmRtu::getDevData(uchar *ptr, int lines, sDevObj *obj)
{
    ptr = toShort(ptr, obj->version);
    ptr = toShort(ptr, obj->status);

    obj->size = lines;
    ptr = getLines(ptr, obj);
    ptr = toAngles(ptr, obj);
    ptr = getBranchs(ptr, obj);
    ptr = toBranchIt(ptr, obj->neutral);
}
