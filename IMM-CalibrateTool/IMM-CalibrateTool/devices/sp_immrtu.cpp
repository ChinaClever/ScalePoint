/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "sp_immrtu.h"

SP_ImmRtu::SP_ImmRtu(QObject *parent) : SP_ImmObj(parent)
{
}

SP_ImmRtu *SP_ImmRtu::bulid(QObject *parent)
{
    static SP_ImmRtu* sington = nullptr;
    if(sington == nullptr)
        sington = new SP_ImmRtu(parent);
    return sington;
}

bool SP_ImmRtu::recvPacket(uchar *ptr, int len, sDevObj *obj)
{
    bool ret = false; uchar *start = ptr;
    obj->size = mDt->lines; ptr += 2;

    ptr = toShort(ptr, obj->status);
    ptr = getLines(ptr, obj);
    ptr = toShort(ptr, obj->hz);

    ptr = toAngles(ptr, obj);
    ptr = getBranchs(ptr, obj);
    // ptr = toBranchIt(ptr, obj->neutral);

    if((start+len-2) == ptr) ret = true;
    else qDebug() << "SP_ImmRtu recvPacket Data  err" << len << ptr-start;

    return ret;
}


bool SP_ImmRtu::readPduData()
{
    bool ret = false;
    auto t = masterRequest(FC_READ_IMM);
    int len = std::get<0>(t);
    if(len > 142) {
        uchar *recv = std::get<1>(t);
        ret = recvPacket(recv, len, mData);
    } else {
        mData->size = 0;
        qDebug() << " Dev_ImmRtu read Pdu Data err" << len;
    }

    return ret;
}


