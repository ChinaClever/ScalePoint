/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "sp_object.h"

SP_Object::SP_Object(QObject *parent) : BaseThread(parent)
{

}

void SP_Object::initFunSlot()
{
    mModbus = Rtu_Modbus::bulid(this)->get(1);
}

void SP_Object::reflush()
{
    msleep(50); mModbus->reflush();
}

int SP_Object::toArray(sFrameFormat &it, uchar *cmd)
{
    int i = 0;
    cmd[i++] = it.fc;
    cmd[i++] = it.addr;
    cmd[i++] = it.msb;
    cmd[i++] = it.lsb;

    it.crc = mModbus->CRC16(cmd, 4);
    cmd[i++] = (uint8_t) (it.crc >> 8);
    cmd[i++] = (uint8_t) (it.crc & 0xFF);
    // qDebug () << cm_ByteArrayToHexString(QByteArray((char *)cmd, 6));
    return i;
}

bool SP_Object::checkCrc(uchar *recv, int len)
{
    bool ret = false;
    ushort crc = mModbus->CRC16(recv, len-2);
    ushort res = recv[len-2]*256 + recv[len-1];
    if((crc == res) || (len ==6)) {
        ret = true;
    } else {
        qDebug() << " Dev_ScalePoint CRC err" << recv[0] <<crc << res;
    }

    return ret;
}

int SP_Object::transmit(sFrameFormat &it, uchar *recv)
{
    uchar sent[10] = {0};
    int len = toArray(it, sent); reflush();
    len = mModbus->transmit(sent, len, recv, 1);
    if(len < 6 || len > 2048) len = mModbus->transmit(sent, len, recv, 1);
    if(len < 6) { qDebug() << " Dev_ScalePoint read Data err" << len; return 0; }
    return len;
}

int SP_Object::filterUpolData(sFrameFormat &it)
{
    static uchar recv[4096] = {0};
    int len = transmit(it, recv);
    uchar *ptr = recv + 6;
    if((recv[0]&ERROR_MASK_BIT) == it.fc) ptr = recv; else len -=6;
    if(((ptr[1]&ERROR_MASK_BIT) == it.addr) && (len > 5)) {
        if(checkCrc(ptr, len)) it.reply = ptr; else len = 0;
    } else {
        qDebug() << " Dev_ScalePoint filter UPOL Data err" << it.fc << ptr[0] << it.addr << len;
        len = 0;
    }

    return len;
}

int SP_Object::masterRequest(sFrameFormat &it)
{
    int ret = filterUpolData(it);
    if(!ret) ret = filterUpolData(it);
    return ret;
}

std::tuple<int, uchar *> SP_Object::masterRequest(uchar fc)
{
    sFrameFormat it;
    it.fc = fc;
    it.addr = mItem->addr;
    it.msb = 0;
    it.lsb = 0;

    int ret = masterRequest(it);
    return std::make_tuple(ret, it.reply);
}

QByteArray SP_Object::masterRequest(uchar fc, uchar addr, uchar msb, uchar lsb)
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

bool SP_Object::writeSerial(uchar fc, uchar addr, uchar msb, uchar lsb)
{
    sFrameFormat it;
    it.fc = fc;
    it.addr = addr;
    it.msb = msb;
    it.lsb = lsb;

    uchar sent[10] = {0};
    int len = toArray(it, sent);
    bool ret = mModbus->writeSerial(sent, len);
    if(ret) reflush();

    return mModbus->writeSerial(sent, len);
}

bool SP_Object::masterWrite(uchar fc, uchar addr, uchar msb, uchar lsb, bool ack)
{
    bool ret = false;
    QByteArray array = masterRequest(fc, addr, msb, lsb);
    if(array.size()) {
        if(ack) {
            if(array.at(0) == fc) ret = true;
        } else {
            if((array.at(0)&ERROR_MASK_BIT) == fc) ret = true;
        }
    } else qDebug() << "master Write No Request";

    return ret;
}

bool SP_Object::enumDeviceType()
{
    bool ret = false; reflush();
    for(int i=0,k=0; i<9; ++i,k=0) {
        QByteArray array = mModbus->readSerial(250);
        if(array.size()) {
            if((array.at(k++) == FC_REQUEST_ADDR) && (array.at(k++) == MASTER_ADDR)) {
                mDt->id = array.at(k++) >> 1;
                mDt->size = array.at(k++);
                switch (mDt->id) {
                case DEVICE_TYPE_A: mDt->dt = "Standard Socket"; break;
                case DEVICE_TYPE_B: mDt->dt = "Socket with Relay"; break;
                case DEVICE_TYPE_C: mDt->dt = "Socket Metered"; break;
                case DEVICE_TYPE_D: mDt->dt = "Socket Metered with Relay"; break;
                case DEVICE_TYPE_IMM_1L: mDt->dt = "IMM single line with 3 branch current"; break;
                case DEVICE_TYPE_IMM_3L: mDt->dt = "IMM three lines with 6 branch current"; break;
                case DEVICE_TYPE_IMM_3L_N: mDt->dt = "IMM three lines with 6 branch current + neutral"; break;
                default: qDebug() << "enumDeviceType err" <<  mDt->id << mDt->size; continue;
                } ret = true; break;
            }
        } else writeSerial(FC_RESET, BROADCAST_ADDR, 0, i%2);
    }

    return ret;
}

bool SP_Object::requestAddr(int addr)
{
    mDt->addr = addr;
    writeSerial(FC_REQUEST_ADDR, MASTER_ADDR, mDt->size, addr);
    return masterWrite(FC_REQUEST_ADDR, MASTER_ADDR, mDt->size, addr);
}

bool SP_Object::readVersion()
{
    bool ret = true;
    QByteArray res = masterRequest(FC_FW_VERSION, mDt->addr, 0, 0);
    if(res.size() && (res.at(0) == FC_FW_VERSION)) {
        mDt->fw = tr("%1.%2").arg((uchar)res[2]).arg((uchar)res[3]);
    } else {
        ret = false;
    }

    return ret;
}
