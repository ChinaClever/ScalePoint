/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "sp_object.h"
#include "common.h"

SP_Object::SP_Object(QObject *parent) : BaseThread(parent)
{

}

void SP_Object::initFunSlot()
{
    mModbus = Rtu_Modbus::bulid(this)->get(0);
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
    // qDebug () << cm_ByteArrayToHexStr(QByteArray((char *)cmd, 6));
    return i;
}

bool SP_Object::checkCrc(uchar *recv, int len)
{
    bool ret = false;
    ushort crc = mModbus->CRC16(recv, len-2);
    ushort res = recv[len-2]*256 + recv[len-1];
    if((crc == res) || (len%6 == 0)) {
        ret = true;
    } else {
        qDebug() << " Dev_ScalePoint CRC err" << len << recv[0] << crc << res;
        qDebug() << cm_ByteArrayToHexStr(QByteArray((char *)recv, len));
    }

    return ret;
}

int SP_Object::transmit(sFrameFormat &it, uchar *recv)
{
    uchar sent[10] = {0};
    int len = toArray(it, sent); reflush();
    len = mModbus->transmit(sent, len, recv, 2);
    if(len < 6 || len > 2048) len = mModbus->transmit(sent, len, recv, 2);
    if(len < 6) { qDebug() << "Err: Dev_ScalePoint read Data err" << len; return 0; }
    return len;
}

int SP_Object::filterUpolData(sFrameFormat &it)
{
    static uchar recv[4096] = {0};
    int len = transmit(it, recv);
    uchar *ptr = recv + 6; //if(recv[0] == 0x9F) len -=6; else
    //    if((recv[0]&ERROR_MASK_BIT) == it.fc) ptr = recv; else len -=6;
    if(len > 11) len -= 6; else  ptr = recv;
    if(((ptr[1]&ERROR_MASK_BIT) == it.addr) && (len > 5)) {
        if(checkCrc(ptr, len)) it.reply = ptr; else len = 0;
    } else {
        qDebug() << "Err: Dev_ScalePoint filter UPOL Data err" << it.fc << ptr[0] << it.addr << len;
        //qDebug() << cm_ByteArrayToHexStr(QByteArray((char *)recv, 6+len));
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
    it.addr = mDt->addr;
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
    return mModbus->writeSerial(sent, len);
}

bool SP_Object::masterWrite(uchar fc, uchar addr, uchar msb, uchar lsb, bool ack)
{
    bool ret = false;
    QByteArray array = masterRequest(fc, addr, msb, lsb).right(6);
    if(array.size()) {
        if(ack) {
            if(array.at(0) == fc) ret = true;
        } else {
            if((array.at(0)&ERROR_MASK_BIT) == fc) ret = true;
        }
    } else qDebug() << "master Write No Request";

    return ret;
}

bool SP_Object::enumSocketType()
{
    bool ret = true;
    switch (mDt->devType) {
    case DEVICE_TYPE_A: mDt->dt = "Standard Socket"; break;
    case DEVICE_TYPE_B: mDt->dt = "Socket with Relay"; break;
    case DEVICE_TYPE_C: mDt->dt = "Socket Metered"; break;
    case DEVICE_TYPE_D: mDt->dt = "Socket Metered with Relay"; break;
    default: ret = false; break;
    }

    if(ret) writeSerial(FC_REQUEST_ADDR, MASTER_ADDR, 2, 3); // mDt->outputs
    return ret;
}

bool SP_Object::enumImmType()
{
    bool ret = true;
    switch (mDt->devType) {
    case DEVICE_TYPE_IMM_1L:
        mDt->lines = 1; mDt->outputs = 3; mDt->neutral = 0;
        mDt->dt = "IMM single line with 3 branch current"; break;
    case DEVICE_TYPE_IMM_3L:
        mDt->lines = 3; mDt->outputs = 6; mDt->neutral = 1; /////======
        mDt->dt = "IMM three lines with 6 branch current"; break;
    case DEVICE_TYPE_IMM_3L_N:
        mDt->lines = 3; mDt->outputs = 6; mDt->neutral = 1;
        mDt->dt = "IMM three lines with 6 branch current + neutral"; break;
    default: qDebug() << "Err: enumImmType " << mDt->devType << mDt->lines; ret = false;
    }

    if(ret) writeSerial(FC_REQUEST_ADDR, MASTER_ADDR, 1, 1);
    return ret;
}

bool SP_Object::enumDeviceType()
{
    int cnt = 0;
    bool ret = false; reflush();
    for(int i=0; i<10; ++i) {
        QByteArray array = mModbus->readSerial(230);
        if(array.size() > 6) {
            if((array.at(0) == FC_REQUEST_ADDR) && (array.at(1) == MASTER_ADDR)) {
                mDt->devType = array.at(2) >> 1; mDt->outputs = array.at(3);
                if(mDt->devType == DEVICE_TYPE_B) ret = enumSocketType();
                else if(mDt->devType < 8) ret = enumImmType();
            } reflush();
        } else if(cnt++ > 5) break;
    }

    return ret;
}

void SP_Object::resDev()
{
    writeSerial(FC_RESET, BROADCAST_ADDR, 0, 0);
}

bool SP_Object::requestAddr(int addr)
{
    mDt->addr = addr;
    //writeSerial(FC_REQUEST_ADDR, MASTER_ADDR, 1, addr);
    return masterWrite(FC_REQUEST_ADDR, MASTER_ADDR, 1, addr);
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


bool SP_Object::readSn()
{
    bool ret = true;
    QByteArray res = masterRequest(FC_SERIAL_NUMBER, mDt->addr, 0, 0);
    if(res.size() && (res.at(0) == FC_SERIAL_NUMBER)) {
        QByteArray array = res.mid(2,4);
        QDataStream stream(&array, QIODevice::ReadOnly);
        mDt->sn = cm_ByteArrayToHexStr(array);
    } else {
        ret = false;
    }

    return ret;
}
