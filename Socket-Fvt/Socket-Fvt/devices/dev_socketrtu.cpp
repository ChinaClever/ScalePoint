/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
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

bool Dev_SocketRtu::openAll()
{
    int size = mDt->outputs ? 8 : mDt->outputs;
    for(int i=0; i<size; ++i) masterWrite(FC_RELAY_WRITE, i+1, 0x00, 0x00); return true;
    // return  masterWrite(FC_RELAY_WRITE, BROADCAST_ADDR, 0x00, 0x00);
}

int Dev_SocketRtu::readOutput(uchar addr)
{
    int ret = -1;
    QByteArray res = masterRequest(FC_RELAY_READ, addr, 0x00, 0x00);
    if(res.size()) ret = res.at(3);
    return ret;
}

bool Dev_SocketRtu::openOutput(uchar addr)
{
    bool ret = masterWrite(FC_RELAY_WRITE, addr, 0x00, 0x00);
    if(ret) if(readOutput(addr)) ret = false;
    return ret;
}

bool Dev_SocketRtu::closeAll()
{
    int size = mDt->outputs ? 8 : mDt->outputs;
    for(int i=0; i<size; ++i) masterWrite(FC_RELAY_WRITE, i+1, 0x00, 0x01); return true;
    //return  masterWrite(FC_RELAY_WRITE, BROADCAST_ADDR, 0x00, 0x01);
}

bool Dev_SocketRtu::closeOutput(uchar addr)
{
    bool ret = masterWrite(FC_RELAY_WRITE, addr, 0x00, 0x01);
    if(ret) if(!readOutput(addr)) ret = false;
    return ret;
}
