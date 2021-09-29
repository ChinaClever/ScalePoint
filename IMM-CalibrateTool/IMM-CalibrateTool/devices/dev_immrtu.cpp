/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "dev_immrtu.h"

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

int Dev_ImmRtu::recvLine(int len)
{
    int ret = 0;
    switch (len-4) {
    case LEN_READ_IMM_1L: ret = 1; break;
    case LEN_READ_IMM_3L: ret = 3; break;
    default: qDebug() << "IMM rtu recv len Err!!" << len; break;
    }

    return ret;
}


bool Dev_ImmRtu::recvPacket(uchar *buf, int len)
{
    bool ret = true;
    int line = 3; // int line = recvLine(len); //////========
    if(line) {
        getDevData(buf, line, mData);
    } else {
        ret = false;
    }

    return ret;
}


bool Dev_ImmRtu::readPduData()
{
    bool ret = false;
    auto t = masterRequest(FC_READ_IMM);
    int len = std::get<0>(t);
    if(len > 100) {
        uchar *recv = std::get<1>(t);
        ret = recvPacket(recv, len);
    } else {
        qDebug() << " Dev_ImmRtu read Pdu Data err" << len;
    }

    return ret;
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
    for(int i=0; i<4; ++i) { // OpSize
        ptr = toBranchIt(ptr, obj->branchs[i]);
    }
    return ptr;
}

void Dev_ImmRtu::getDevData(uchar *ptr, int lines, sDevObj *obj)
{
    obj->size = lines; ptr += 2;
    ptr = toShort(ptr, obj->status);
    ptr = getLines(ptr, obj);
    ptr = toShort(ptr, obj->hz);

    ptr = toAngles(ptr, obj);
    ptr = getBranchs(ptr, obj);
    // ptr = toBranchIt(ptr, obj->neutral);
}
