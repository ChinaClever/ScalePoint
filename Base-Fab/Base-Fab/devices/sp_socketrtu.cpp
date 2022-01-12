/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "sp_socketrtu.h"

SP_SocketRtu::SP_SocketRtu(QObject *parent) : SP_SocketCtrl(parent)
{

}

SP_SocketRtu *SP_SocketRtu::bulid(QObject *parent)
{
    static SP_SocketRtu* sington = nullptr;
    if(sington == nullptr)
        sington = new SP_SocketRtu(parent);
    return sington;
}
