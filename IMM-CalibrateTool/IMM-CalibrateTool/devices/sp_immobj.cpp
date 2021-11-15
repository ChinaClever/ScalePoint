/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "sp_immobj.h"

SP_ImmObj::SP_ImmObj(QObject *parent) : SP_Object(parent)
{

}

uchar *SP_ImmObj::toInt(uchar *ptr, uint &value)
{
    value =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能高8位
    value <<= 16; // 左移8位
    value +=  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能底8位
    return ptr;
}

uchar *SP_ImmObj::toShort(uchar *ptr, ushort &value)
{
    value =  (*ptr) * 256 + *(ptr+1);  ptr += 2;
    return ptr;
}

uchar *SP_ImmObj::toItem(uchar *ptr, sItData &it)
{
    ptr = toInt(ptr, it.active);
    ptr = toInt(ptr, it.reactive);
    ptr = toInt(ptr, it.apparen);
    return ptr;
}

uchar *SP_ImmObj::toLine(uchar *ptr, sLineData &line)
{
    ptr = toShort(ptr, line.vol_rms);
    ptr = toInt(ptr, line.cur_rms);
    ptr = toInt(ptr, line.cur_peak);
    ptr = toItem(ptr, line.pow);
    ptr = toInt(ptr, line.ele);
    return ptr;
}

uchar *SP_ImmObj::getLines(uchar *ptr, sDevObj *obj)
{
    for(int i=0; i<mDt->lines; ++i) {
        ptr = toLine(ptr, obj->lines[i]);
    }

    return ptr;
}

uchar *SP_ImmObj::toAngles(uchar *ptr, sDevObj *obj)
{
    for(int i=0; i<2; ++i) {
        ptr = toShort(ptr, obj->angles[i]);
    }

    return ptr;
}

uchar *SP_ImmObj::toBranchIt(uchar *ptr, sBranchIt &it)
{
    ptr = toInt(ptr, it.cur_rms);
    ptr = toInt(ptr, it.cur_peak);
    return ptr;
}

uchar *SP_ImmObj::getBranchs(uchar *ptr, sDevObj *obj)
{
    for(int i=0; i<6; ++i) { // OpSize
        ptr = toBranchIt(ptr, obj->branchs[i]);
    }

    return ptr;
}
