/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "dev_object.h"

Dev_Object::Dev_Object(QObject *parent) : BaseThread(parent)
{
}

uchar *Dev_Object::toInt(uchar *ptr, uint &value)
{
    value =  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能高8位
    value <<= 16; // 左移8位
    value +=  (*ptr) * 256 + *(ptr+1);  ptr += 2; // 读取电能底8位

    return ptr;
}

uchar *Dev_Object::toShort(uchar *ptr, ushort &value)
{
    value =  (*ptr) * 256 + *(ptr+1);  ptr += 2;
    return ptr;
}
