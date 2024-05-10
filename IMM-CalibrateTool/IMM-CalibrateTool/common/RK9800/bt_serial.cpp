/*
 *
 *  Created on: 2022年1月1日
 *      Author: Lzy
 */
#include "bt_serial.h"

Bt_Serial::Bt_Serial(QObject *parent) : QThread(parent)
{

}

void Bt_Serial::init(int id)
{
    mModbus = Rtu_Modbus::bulid()->get(id);
    //mModbus->setBaudRate(9600);
    mModbus->setBaudRate(19200);
}

sRtuItem Bt_Serial::initRtuItem()
{
    sRtuItem it;
    it.addr = 1;
    it.fn = 0x03;
    it.reg = 0x0B;
    it.num = 3;
    return it;
}

bool Bt_Serial::readPacket(sBtIt &bt)
{
    bool ret = true;
    sRtuItem it = initRtuItem();
    uchar recv[MODBUS_RTU_SIZE] = {0};

    int len = mModbus->read(it, recv);
    if(len > 0) {
        QByteArray array((char *)recv, len);
        QDataStream rxStream(array);
        rxStream.setByteOrder(QDataStream::BigEndian);
        rxStream >> bt.vol >> bt.cur >> bt.pow;
    } else ret = false;
    return ret;
}

bool Bt_Serial::openSocket(int index)
{
    bool ret = true;
    uchar send[MODBUS_RTU_SIZE] = {0x7B, 0xA1, 0x10, 0xFF, 0xFF, 0xFF, 0xC7, 0xC8, 0xC9, 0xFF, 0xFF, 0xFF , 0xD7 , 0xD8, 0xD9, 0xCA};
    send[3] &= (0x80 >> (index - 1));
    send[4] = 0;
    send[5] = 0;
    send[9] = 0;
    send[10] = 0;
    send[11] = 0;
    uchar recv[MODBUS_RTU_SIZE] = {0};
    mModbus->rtuWriteRPDU(send , 16, recv);
    return ret;
}

bool Bt_Serial::closeSocket(int index)
{
    bool ret = true;
    uchar send[MODBUS_RTU_SIZE] = {0x7B, 0xA1, 0x10, 0xFF, 0xFF, 0xFF, 0xC7, 0xC8, 0xC9, 0xFF, 0xFF, 0xFF , 0xD7 , 0xD8, 0xD9, 0xCA};
    send[3] = 0;
    send[4] = 0;
    send[5] = 0;
    send[9] &= (0x80 >> (index - 1));
    send[10] = 0;
    send[11] = 0;
    uchar recv[MODBUS_RTU_SIZE] = {0};
    mModbus->rtuWriteRPDU(send , 16, recv );
    return ret;
}

bool Bt_Serial::readSocket(int index , int onOrOff)
{
    bool ret = false;
    uchar send[MODBUS_RTU_SIZE] = {0x7B, 0xC1, 0x01, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 , 0x00 , 0x00 , 0X00 , 0xCA};
    uchar recv[MODBUS_RTU_SIZE] = {0};
    ret = mModbus->rtuReadRPDU(send , 16, index , onOrOff ,  recv );

    return ret;
}

