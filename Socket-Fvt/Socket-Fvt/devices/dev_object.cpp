/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "dev_object.h"

Dev_Object::Dev_Object(QObject *parent) : BaseThread(parent)
{
}

uchar *Dev_Object::toInt(uchar *ptr, uint &value)
{
    value =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能高8位
    value <<= 16; // 左移8位
    value +=  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能底8位

    return ptr;
}

uchar *Dev_Object::toShort(uchar *ptr, ushort &value)
{
    value =  (*ptr) * 256 + *(ptr+1);  ptr += 2;
    return ptr;
}

int Dev_Object::toArray(sFrameFormat &it, uchar *cmd)
{
    int i = 0;
    cmd[i++] = it.fc;
    cmd[i++] = it.addr;
    cmd[i++] = it.msb;
    cmd[i++] = it.lsb;

    it.crc = mModbus->CRC16(cmd, 4);
    cmd[i++] = (uint8_t) (it.crc >> 8);
    cmd[i++] = (uint8_t) (it.crc & 0xFF);

    return i;
}

bool Dev_Object::checkCrc(uchar *recv, int len)
{
    bool ret = false;
    ushort crc = mModbus->CRC16(recv, len-2);
    ushort res = recv[len-2]*256 + recv[len-1];
    if(crc == res) {
        ret = true;
    } else {
        qDebug() << " Dev_Object CRC err" << crc << res;
    }

    return ret;
}

int Dev_Object::transmit(sFrameFormat &it, uchar *recv)
{
    uchar sent[10] = {0};
    int len = toArray(it, sent);
    len = mModbus->transmit(sent, len, recv, 1);
    if(len < 10 || len > 200) len = mModbus->transmit(sent, len, recv, 1);
    if(len < 6) { qDebug() << " Dev_Object read Data err" << len; return 0; }
    return len;
}

int Dev_Object::filterUpolData(sFrameFormat &it)
{
    static uchar recv[4096] = {0};
    int len = transmit(it, recv);

    uchar *ptr = recv + 6;
    if(/*(it.fc == ptr[0]) && */(it.addr == ptr[1]) && (len > 10)) {
        for(int i=0; i<len-6; ++i) recv[i] = recv[i+6]; len -= 6;
        if(checkCrc(recv, len)) it.reply = recv; else len = 0;
    } else {
        if(len!=6) qDebug() << " Dev_Object filter UPOL Data err" << it.fc << ptr[0] << it.addr << len;
        len = 0;
    }

    return len;
}

int Dev_Object::masterRequest(sFrameFormat &it)
{
    int ret = filterUpolData(it);
    if(!ret) ret = filterUpolData(it);
    return ret;
}

std::tuple<int, uchar *> Dev_Object::masterRequest(uchar fc)
{
    sFrameFormat it;
    it.fc = fc;
    it.addr = mItem->addr;
    it.msb = 0;
    it.lsb = 0;

    int ret = masterRequest(it);
    return std::make_tuple(ret, it.reply);
}

QByteArray Dev_Object::masterRequest(uchar fc, uchar addr, uchar msb, uchar lsb)
{
    sFrameFormat it;
    it.fc = fc;
    it.addr = addr;
    it.msb = msb;
    it.lsb = lsb;

    QByteArray res;
    int len = masterRequest(it);
    if(len) for(int i=0; i<len; ++i) res.append(it.reply[i]);

    return res;
}

bool Dev_Object::masterWrite(sFrameFormat &it)
{
    uchar sent[10] = {0};
    int len = toArray(it, sent);
    bool ret = mModbus->writeSerial(sent, len);
    if(ret) reflush();

    return ret;
}

bool Dev_Object::masterWrite(uchar fc, uchar addr, uchar msb, uchar lsb)
{
    sFrameFormat it;
    it.fc = fc;
    it.addr = addr;
    it.msb = msb;
    it.lsb = lsb;

    return masterWrite(it);
}

void Dev_Object::reflush()
{
    msleep(110); mModbus->reflush();
}

bool Dev_Object::requestAddr(int addr)
{
    bool ret = masterWrite(FC_REQUEST_ADDR, MASTER_ADDR, 0x01, addr);
    if(ret) {
        int len = masterRequest(FC_REQUEST_ADDR, MASTER_ADDR, 0x01, addr).size();
        if(len > 6) ret = false; else mItem->addr = addr;
    }

    return ret;
}

bool Dev_Object::readVersion()
{
    bool ret = true;
    QByteArray res = masterRequest(FC_FW_VERSION, mItem->addr, 0, 0);
    if(res.size()) {
        mDt->fw = tr("%1.%2").arg((uchar)res[2]).arg((uchar)res[3]);
    } else {
        ret = false;
    }

    return ret;
}
