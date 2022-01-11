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
    for(int i=0; i<mDt->outputs; ++i){ if(!openOutput(mDt->addr+i)) ret = false; } return ret;
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
    masterWrite(FC_WRITE_RELAY, addr, 0x00, status);
    bool ret = masterWrite(FC_WRITE_RELAY, addr, 0x00, status);
    // if(ret) if(status != readOutput(addr)) ret = false;
    return ret;
}

bool SP_SocketCtrl::openOutput(uchar addr)
{
    return ctrlOutput(addr, 0x01);
}

bool SP_SocketCtrl::closeAll()
{
    bool ret = true;
    //return  masterWrite(FC_WRITE_RELAY, BROADCAST_ADDR, 0x00, 0x01);
    for(int i=0; i<mDt->outputs; ++i){ if(!closeOutput(mDt->addr+i)) ret = false; } return ret;
}

bool SP_SocketCtrl::closeOutput(uchar addr)
{
    return ctrlOutput(addr, 0x00);
}

bool SP_SocketCtrl::measRot(uchar addr, uint &t)
{    
    bool ret = false; t = 0;
    for(int i=0; i<3; ++i) {
        QByteArray array = masterRequest(FC_MEAS_ROT, addr);
        if(array.size() && (array.at(0) == FC_MEAS_ROT)) {
            t =  array.at(2) * 256 + array.at(3);
            t = t * 100.0 / 20.0 ; // 转换为ms
            if((t>200)&&(t<1500)) {ret = true; break;}
        } mdelay(2);
    }

    return ret;
}

QString SP_SocketCtrl::storeValue()
{
    QString str ;
    QByteArray res = masterRequest(FC_SROT_TO_FLASH, mDt->addr);
    if(res.size() && (res.at(0) == FC_SROT_TO_FLASH)) {
        switch (res.at(3)) {
        case 0: str = "0: FLASH_PROG_OK"; break;
        case 1: str = "1: FLASH_ERASE_ERROR"; break;
        case 2: str = "2: FLASH_RELAY_NOT_MEASURED"; break;
        case 3: str = "3: FLASH_PROG_ERROR"; break;
        case 4: str = "4: FLASH_MAGIC_PROG_ERROR"; break;
        case 5: str = "5: FLASH_VERIFY_ERROR"; break;
        }
    }

    return str;
}
