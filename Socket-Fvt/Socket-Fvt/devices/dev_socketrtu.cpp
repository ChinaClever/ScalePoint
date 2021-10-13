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
    return  masterWrite(FC_RELAY_WRITE, BROADCAST_ADDR, 0x00, 0x00);
}

bool Dev_SocketRtu::openOutput(uchar addr)
{
    return  masterWrite(FC_RELAY_WRITE, addr, 0x00, 0x00);
}

bool Dev_SocketRtu::closeAll()
{
    return  masterWrite(FC_RELAY_WRITE, BROADCAST_ADDR, 0x00, 0x01);
}

bool Dev_SocketRtu::closeOutput(uchar addr)
{
    return  masterWrite(FC_RELAY_WRITE, addr, 0x00, 0x01);
}
