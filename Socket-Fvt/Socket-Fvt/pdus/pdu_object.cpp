/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "pdu_object.h"

Pdu_Object::Pdu_Object(QObject *parent) : BaseThread(parent)
{
    mPduData = new sPduData();
    initData(1); // // mItem->addr
}


void Pdu_Object::initData(int addr)
{
    memset(mPduData, 0, sizeof(sPduData));
    mPduData->addr = addr;
}


void Pdu_Object::initFunSlot()
{
    mModbus = Rtu_Modbus::bulid(this)->get(1);
}
