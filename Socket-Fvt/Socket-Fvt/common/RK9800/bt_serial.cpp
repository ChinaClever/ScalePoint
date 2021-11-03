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
    mModbus->setBaudRate(9600);
}


sRtuItem Bt_Serial::initRtuItem()
{
    sRtuItem it;
    it.addr = 1;
    it.fn = 0x03;
    it.reg = 0;
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
        rxStream.setByteOrder(QDataStream::LittleEndian);
        rxStream >> bt.vol >> bt.cur;
    } else ret = false;
    return ret;
}
