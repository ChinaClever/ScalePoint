#include "dev_socketrtu.h"

Dev_SocketRtu::Dev_SocketRtu(QObject *parent) : Dev_Object(parent)
{

}


Dev_SocketRtu *Dev_SocketRtu::bulid(QObject *parent)
{
    static Dev_SocketRtu* sington = nullptr;
    if(sington == nullptr)
        sington = new Dev_SocketRtu(parent);
    return sington;
}


int Dev_SocketRtu::initRtuItem(uchar *cmd, uchar fn)
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


bool Dev_SocketRtu::checkCrc(uchar *recv, int len)
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

int Dev_SocketRtu::filterUpolData(uchar *recv, uchar fn)
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


bool Dev_SocketRtu::requestAddr()
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


bool Dev_SocketRtu::readVersion()
{
    bool ret = true;
    static uchar recv[1024] = {0};
    int len = filterUpolData(recv, 5);
    if(len) {
        ushort version = recv[2]*256 + recv[3];
        if(version) mDt->fw = QString::number(version/100.0);
    } else {
        ret = false;
    }

    return ret;
}
