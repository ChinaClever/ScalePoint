/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "pdu_mctrl.h"

Pdu_MCtrl::Pdu_MCtrl(QObject *parent) : Pdu_ObjCtrl(parent)
{

}

void Pdu_MCtrl::funSwitch(uchar *on, uchar *off)
{
    uchar cmd[] = {0x7B, 0xC1, 0x01, 0x15, 0xA1,
                   0xFF, 0xFF, 0xFF, 0xC7, 0xC8, 0xC9,
                   0x00, 0x00, 0x00, 0xD7, 0xD8, 0xD9,
                   0x00, 0x01, 0x00, 0xCC};

    for(int i=0; i<3; ++i) {
        cmd[5+i] = on[i];
        cmd[11+i] = off[i];
    }

    cmd[18] = cmd[2] = mPduData->addr;
    cmd[20] = mModbus->xorNum(cmd,sizeof(cmd)-1);
    mModbus->writeSerial(cmd, sizeof(cmd));
}

void Pdu_MCtrl::funClearEle(uchar *buf)
{
    static uchar cmd[] = {0x7B, 0xC1, 0x01, 0x15, 0xD1,
                          0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0xCC};

    for(int i=0; i<3; i++) cmd[5+i] = buf[i];
    cmd[2] = mPduData->addr;
    cmd[20] = mModbus->xorNum(cmd,sizeof(cmd)-1);
    mModbus->writeSerial(cmd, sizeof(cmd));
}

bool Pdu_MCtrl::factorySet()
{
    static uchar cmd[] = {0x7B, 0xA2, 0x17, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0xCC};

    cmd[22] = mModbus->xorNum(cmd,sizeof(cmd)-1);
    return mModbus->writeSerial(cmd, sizeof(cmd));
}
