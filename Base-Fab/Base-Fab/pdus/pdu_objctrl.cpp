/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "pdu_objctrl.h"

Pdu_ObjCtrl::Pdu_ObjCtrl(QObject *parent) : Pdu_ObjRtu(parent)
{

}

void Pdu_ObjCtrl::openAllSwitch()
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0xFF;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    funSwitch(on, off);
}

void Pdu_ObjCtrl::setBitControl(int id, uchar *buf)
{
    int addr = mPduData->addr; //  执行板地址
    int size = mPduData->size; // 执行板输出位数量
    int index = (size * (addr-1) +id) / 8;
    int key = (size * (addr-1) +id) % 8 ;
    buf[index] |= 0x80 >> key;
}

void Pdu_ObjCtrl::openOutputSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    setBitControl(id, on);
    funSwitch(on, off);
}

void Pdu_ObjCtrl::closeOtherSwitchSlot()
{
    closeOtherSwitch(mIdClosed);
}

void Pdu_ObjCtrl::openOnlySwSlot(int id)
{
    mIdClosed = id;
    openOutputSwitch(id);
    QTimer::singleShot(10,this,SLOT(closeOtherSwitchSlot()));
}

void Pdu_ObjCtrl::openOnlySwitch(int id)
{
    openOutputSwitch(id);
    closeOtherSwitch(id);
}

void Pdu_ObjCtrl::closeOtherSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    setBitControl(id, off);
    for(int i=0; i<6; i++)  off[i] = ~off[i];
    funSwitch(on, off);
}

void Pdu_ObjCtrl::closeAllSwitch()
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0xff;  //关闭有效位

    funSwitch(on, off);
}

void Pdu_ObjCtrl::closeOutputSwitch(int id)
{
    uchar on[8], off[8];
    for(int i=0; i<6; i++)  on[i] = 0x00;  //打开有效位
    for(int i=0; i<6; i++)  off[i] = 0x00;  //关闭有效位

    setBitControl(id, off);
    funSwitch(on, off);
}

void Pdu_ObjCtrl::clearAllEle()
{
    uchar cmd[8];
    for(int i=0; i<6; i++) cmd[i] = 0xFF;
    funClearEle(cmd);
}

void Pdu_ObjCtrl::setClearEle(int id)
{
    uchar cmd[8];
    for(int i=0; i<6; i++) cmd[i] = 0;

    setBitControl(id, cmd);
    funClearEle(cmd);
}
