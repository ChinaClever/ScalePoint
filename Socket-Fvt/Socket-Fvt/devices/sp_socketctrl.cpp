/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "sp_socketctrl.h"

SP_SocketCtrl::SP_SocketCtrl(QObject *parent) : SP_Object(parent)
{

}

bool SP_SocketCtrl::openAll()
{
    bool ret = true;
    // return  masterWrite(FC_WRITE_RELAY, BROADCAST_ADDR, 0x00, 0x00);
    for(int i=0; i<mDt->size; ++i){ if(!openOutput(mDt->addr+i)) ret = false; } return ret;
}

int SP_SocketCtrl::readOutput(uchar addr)
{
    int ret = -1;
    QByteArray res = masterRequest(FC_READ_RELAY, addr, 0x00, 0x00);
    if(res.size()) ret = res.at(3);
    return ret;
}

bool SP_SocketCtrl::ctrlOutput(uchar addr, uchar status)
{
    bool ret = masterWrite(FC_WRITE_RELAY, addr, 0x00, status);
    if(ret) if(status != readOutput(addr)) ret = false;
    return ret;
}

bool SP_SocketCtrl::openOutput(uchar addr)
{
    return ctrlOutput(addr, 0x00);
}

bool SP_SocketCtrl::closeAll()
{
    bool ret = true;
    for(int i=0; i<mDt->size; ++i){ if(!closeOutput(mDt->addr+i)) ret = false; } return ret;
    //return  masterWrite(FC_WRITE_RELAY, BROADCAST_ADDR, 0x00, 0x01);
}

bool SP_SocketCtrl::closeOutput(uchar addr)
{
    return ctrlOutput(addr, 0x01);
}

bool SP_SocketCtrl::measRot(uchar addr)
{
    bool ret = false;
    for(int i=0; i<3; ++i) {
        QByteArray array = masterRequest(FC_MEAS_ROT, addr);
        if(array.size() && (array.at(0) == FC_MEAS_ROT)) {
            uint t =  array.at(2) * 256 + array.at(3);
            t = t / 20000.0 * 100.0; // 转换为ms
            if((t>200)&&(t<1500)){ret = true;break;}
        }
    }

    return ret;
}

bool SP_SocketCtrl::storeValue(uchar addr)
{
    return masterWrite(FC_SROT_TO_FLASH, addr, 0xFF, 0xFF, true);
}
