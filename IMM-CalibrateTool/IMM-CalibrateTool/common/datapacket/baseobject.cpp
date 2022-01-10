/*
 *
 *  Created on: 2022年1月1日
 *      Author: Lzy
 */
#include "baseobject.h"
#include "baselogs.h"

BaseObject::BaseObject()
{
    mModbus = nullptr;
    mPacket = sDataPacket::bulid();
    mItem = Cfg::bulid()->item;
    mPro = mPacket->getPro();
    mDev = mPacket->getDev();
    mData = &(mDev->data);
    mDt = &(mDev->dt);
}

BaseThread::BaseThread(QObject *parent) : QThread(parent)
{
    isRun = false;
    BaseLogs::bulid(this);
    int t = 650+rand()%150;
    QTimer::singleShot(t,this,SLOT(initFunSlot()));
}

BaseThread::~BaseThread()
{
    mPro->step = Test_Over;
    isRun = false;
    wait();
}

void BaseThread::initFunSlot()
{
    mModbus = Rtu_Modbus::bulid(this)->get();
    if(!mModbus) {
        int t = 100+rand()%50;
        QTimer::singleShot(t,this,SLOT(initFunSlot()));
    }
}

bool BaseThread::updatePro(const QString &str, bool pass, int sec)
{
    mPro->pass << pass;
    mPro->status << str;
    if(pass) pass = delay(sec);
    else mPro->result = Test_Fail;
    if(!pass) BaseLogs::bulid()->appendLogItem(str, pass);
    mPro->time = QTime::currentTime().toString("hh:mm:ss");

    return pass;
}

bool BaseThread::mdelay(int s)
{
    bool ret = true;
    for(int i=0; i<s; ++i) {
        if(mPro->step < Test_Over){
            QThread::msleep(100);
        } else {
            ret = false;
            break;
        }
    }

    return ret;
}
