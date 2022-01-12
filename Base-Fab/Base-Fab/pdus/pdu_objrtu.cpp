/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "pdu_objrtu.h"

Pdu_ObjRtu::Pdu_ObjRtu(QObject *parent) : Pdu_Object(parent)
{
}


uchar *Pdu_ObjRtu::toInt(uchar *ptr, int line, uint *value)
{
    for(int i=0; i<line; ++i) {
        value[i] =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能高8位
        value[i] <<= 16; // 左移8位
        value[i] +=  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能底8位
    }

    return ptr;
}

uchar *Pdu_ObjRtu::toOutputEle(uchar *ptr, int line, uint *value)
{
    for(int i=0; i<line; ++i) {
        value[i] = (*ptr++) << 16;
        value[i] += (*ptr++) << 8;
        value[i] += *ptr++;
    }

    return ptr;
}

uchar *Pdu_ObjRtu::toShort(uchar *ptr, int line, ushort *value)
{
    for(int i=0; i<line; ++i) {
        value[i] =  (*ptr) * 256 + *(ptr+1);  ptr += 2;
    }

    return ptr;
}

uchar *Pdu_ObjRtu::toChar(uchar *ptr, int line, uchar *value)
{
    for(int i=0; i<line; ++i) {
        value[i] = *(ptr++); // 读取电压
    }

    return ptr;
}

uchar *Pdu_ObjRtu::toThreshold(uchar *ptr, int line, sUnitTh &unit)
{
    for(int i=0; i<line; ++i) {
        ptr = toShort(ptr, 1, &unit.min[i]);
        ptr = toShort(ptr, 1, &unit.max[i]);
    }

    return ptr;
}

uchar *Pdu_ObjRtu::toEnvTh(uchar *ptr, sUnitTh &unit)
{
    unit.max[0] = *ptr++;
    unit.min[0] = *ptr++;
    return ptr;
}

ushort Pdu_ObjRtu::getShort(uchar *ptr)
{
    return  (*ptr) * 256 + *(ptr+1);
}

uchar *Pdu_ObjRtu::getShort(uchar *ptr, int line, uchar *value)
{
    for(int i=0; i<line; ++i) {
        value[i] = getShort(ptr); ptr += 2;
    }

    return  ptr;
}
