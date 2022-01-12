/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "pdu_zctrl.h"

Pdu_ZCtrl::Pdu_ZCtrl(QObject *parent) : Pdu_ObjCtrl(parent)
{

}

void Pdu_ZCtrl::funSwitch(uchar *on, uchar *off)
{
    int k = 5, id = mPduData->addr-1; // mItem->addr
    static uchar cmd[68] = {0x7B, 0xC1, 0x00, 0xA2, 0xB2};

    for(int i=0; i<6; i++)  cmd[k++] = on[i];  //打开有效位
    for(int i=0; i<6; i++)  cmd[k++] = off[i];  //关闭有效位

    //预留位
    for(int i=0; i<3; i++)  cmd[k++] = 0xC7 + i;
    for(int i=0; i<3; i++)  cmd[k++] = 0xD7 + i;
    for(int i=0; i<39; i++)  cmd[k++] = 0x00;
    if((on[id] == 0xFF) || (off[id] == 0xFF)) cmd[k++] = 1; // 统一开关为1
    else cmd[k++] = 0;

    for(int i=0; i<3; i++)  cmd[k++] = 0x0E;
    cmd[k++] = 0x44;
    cmd[k] = mModbus->xorNum(cmd,sizeof(cmd)-1);
    mModbus->writeSerial(cmd, sizeof(cmd));
}

void Pdu_ZCtrl::funClearEle(uchar *buf)
{
    int k = 5;
    static uchar cmd[68] = {0x7B, 0xC1, 0x00, 0xA4, 0xB4};

    for(int i=0; i<6; i++) cmd[k++] = buf[i];
    for(int i=0; i<52; i++) cmd[k++] = 0x00;  //预留位
    for(int i=0; i<3; i++) cmd[k++] = 0x0E;
    cmd[k++] = 0x44;
    cmd[k] = mModbus->xorNum(cmd,sizeof(cmd)-1); //异或校验码
    mModbus->writeSerial(cmd, sizeof(cmd));
}

bool Pdu_ZCtrl::factorySet()
{
    int k = 5;
    static uchar cmd[68] = {0x7B, 0xC1, 0x00, 0xA3, 0xB3};

    for(int i=0; i<42; i++) cmd[k++] = 0;
    for(int i=0; i<16; i++) cmd[k++] = 0x00;  //预留位
    for(int i=0; i<3; i++) cmd[k++] = 0x0E;
    cmd[k++] = 0x44;
    cmd[k] = mModbus->xorNum(cmd,sizeof(cmd)-1); //异或校验码
    return mModbus->writeSerial(cmd, sizeof(cmd));
}
