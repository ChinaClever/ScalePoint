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

int Dev_ImmRtu::initRtuItem(uchar *cmd, uchar fn)
{
    int i = 0;
    cmd[i++] = fn;
    cmd[i++] = mItem->addr;
    cmd[i++] = 0x02;
    cmd[i++] = 0x03;

    ushort crc = mModbus->CRC16(cmd, 4);
    cmd[i++] = (uint8_t) (crc >> 8);
    cmd[i++] = (uint8_t) (crc & 0xFF);

    return i;
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
    int line = 3; // int line = recvLine(len);

    getDevData(buf, line, mData);

    return ret;
}

bool Dev_ImmRtu::checkCrc(uchar *recv, int len)
{
    bool ret = false;
    ushort crc = mModbus->CRC16(recv, len-2);
    ushort res = recv[len-2]*256 + recv[len-1];
    if(crc == res) {
        ret = true;
    } else {
        qDebug() << " Dev_ImmRtu CRC err" << crc << res;
    }

    return ret;
}

int Dev_ImmRtu::filterUpolData(uchar *recv, uchar fn)
{
    uchar sent[10] = {0};
    int len = initRtuItem(sent, fn);
    len = mModbus->transmit(sent, len, recv, 2);
    if(len < 10 || len > 200) len = mModbus->transmit(sent, len, recv, 1);
    if(len < 10) { qDebug() << " Dev_ImmRtu read Pdu Data err" << len; return 0; }

    uchar *ptr = recv + 6;
    if((sent[0] == ptr[0]) && (sent[1] == ptr[1]) && (len > 10)) {
        for(int i=0; i<len-6; ++i) recv[i] = recv[i+6]; len -= 6;
        if(!checkCrc(recv, len)) len = 0;
    } else {
        qDebug() << " Dev_ImmRtu filter UPOL Data err" << sent[0] << sent[1] << len;
        len = 0;
    }

    return len;
}

bool Dev_ImmRtu::requestAddr()
{
    static uchar recv[4*MODBUS_RTU_SIZE] = {0};
    uchar sent[] = {0x01, 0x00, 0x01, 0x01, 0x5B, 0xB4};
    bool ret = mModbus->writeSerial(sent, 6);
    for(int i=0; i<3; ++i) mModbus->writeSerial(sent, 6);  //msleep(100);
    if(ret) {
        int len = mModbus->transmit(sent, 6, recv, 1);
        if(len > 6) len = mModbus->transmit(sent, 6, recv, 1);
        if(len > 6) len = mModbus->transmit(sent, 6, recv, 1);
        if(len > 6) ret = false; else mItem->addr = 1;
    }

    return ret;
}

bool Dev_ImmRtu::readPduData()
{
    bool ret = false;
    static uchar recv[4*MODBUS_RTU_SIZE] = {0};
    int len = filterUpolData(recv);
    if(!len) len = filterUpolData(recv);
    if(len > 100) {
        ret = recvPacket(recv, len);
    } else {
        qDebug() << " Dev_ImmRtu read Pdu Data err" << len;
    }

    return ret;
}

bool Dev_ImmRtu::readVersion()
{
    uchar recv[16] = {0};
    bool ret = readPduData();   
    if(ret) {
        int len = filterUpolData(recv, 5);
        if(len) {
            mData->version = recv[2]*256 + recv[3];
        } else {
            ret = false;
        }
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
