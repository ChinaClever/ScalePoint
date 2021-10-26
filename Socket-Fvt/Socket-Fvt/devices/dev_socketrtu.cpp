/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "dev_socketrtu.h"

Dev_SocketRtu::Dev_SocketRtu(QObject *parent) : Dev_ScalePoint(parent)
{

}

Dev_SocketRtu *Dev_SocketRtu::bulid(QObject *parent)
{
    static Dev_SocketRtu* sington = nullptr;
    if(sington == nullptr)
        sington = new Dev_SocketRtu(parent);
    return sington;
}

bool Dev_SocketRtu::openAll()
{
    bool ret = true;
    // return  masterWrite(FC_WRITE_RELAY, BROADCAST_ADDR, 0x00, 0x00);
    for(int i=0; i<mDt->outputs; ++i){ if(!openOutput(mItem->addr+i)) ret = false; } return ret;
}

int Dev_SocketRtu::readOutput(uchar addr)
{
    int ret = -1;
    QByteArray res = masterRequest(FC_READ_RELAY, addr, 0x00, 0x00);
    if(res.size()) ret = res.at(3);
    return ret;
}

bool Dev_SocketRtu::ctrlOutput(uchar addr, uchar status)
{
    bool ret = masterWrite(FC_WRITE_RELAY, addr, 0x00, status);
    if(ret) if(status != readOutput(addr)) ret = false;
    return ret;
}

bool Dev_SocketRtu::openOutput(uchar addr)
{
    return ctrlOutput(addr, 0x00);
}

bool Dev_SocketRtu::closeAll()
{
    bool ret = true;
    for(int i=0; i<mDt->outputs; ++i){ if(!closeOutput(mItem->addr+i)) ret = false; } return ret;
    //return  masterWrite(FC_WRITE_RELAY, BROADCAST_ADDR, 0x00, 0x01);
}

bool Dev_SocketRtu::closeOutput(uchar addr)
{
    return ctrlOutput(addr, 0x01);
}

bool Dev_SocketRtu::measRot(uchar addr)
{
    bool ret = false;
    for(int i=0; i<3; ++i) {
        QByteArray array = masterRequest(FC_MEAS_ROT, addr);
        if(array.size() && (array.at(0) == FC_MEAS_ROT)) {
            uint t =  array.at(2) * 256 + array.at(3);
            t = t / 20000.0 * 100.0; // 转换为ms
            if((t > 200) && (t < 1500)) ret = true;
        }
    }

    return ret;
}

bool Dev_SocketRtu::storeValue(uchar addr)
{
    return masterWrite(FC_SROT_TO_FLASH, addr, 0xFF, 0xFF, true);
}
