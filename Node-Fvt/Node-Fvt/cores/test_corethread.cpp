/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_corethread.h"

Test_CoreThread::Test_CoreThread(QObject *parent) : BaseThread(parent)
{

}

void Test_CoreThread::initFunSlot()
{
    BaseLogs::bulid(this);
    mSn = Test_SerialNumber::bulid(this);
    mFab = test_FabPartition::bulid(this);
    mNetWork = Test_NetWork::bulid(this);
    connect(mFab, SIGNAL(fabSig(QString)), mNetWork, SIGNAL(msgSig(QString)));
}

void Test_CoreThread::workResult()
{
    bool res = true;
    BaseLogs::bulid()->start();
    QString str = tr("最终结果 ");
    if(mPro->result != Test_Fail) {
        str += tr("通过");
    } else {
        res = false;
        str += tr("失败");
    }

    updatePro(str, res, 2);
    mPro->step = Test_Over;
}

bool Test_CoreThread::initFun()
{    
    bool ret = updatePro(tr("即将开始"));
#if defined(Q_OS_WIN32)
    mPro->step = Test_Collect;
#endif
    return ret;
}


bool Test_CoreThread::programFab()
{
    bool ret = mFab->check();
    if(ret) {
        mSn->createSn();
        ret = mFab->workDown();
    }

    return ret;
}

void Test_CoreThread::macSnCheck()
{
    bool ret = false;
    QString str = "serial Number" + tr("检查");
    if(mDt->serialNumber != mDt->sn) {
        str += tr("错误：%1 %2").arg(mDt->serialNumber).arg(mDt->sn);
    } else {
        str += tr("正确：%1 ").arg(mDt->serialNumber); ret = true;
    }
    updatePro(str, ret);

    str = "Mac " + tr("检查");
    if(mDt->macAddress != mItem->macs.mac) {
        str += tr("错误：%1 %2").arg(mDt->macAddress).arg(mItem->macs.mac); ret = false;
    } else {
        str += tr("正确：%1 ").arg(mDt->macAddress); ret = true;
    }
    updatePro(str, ret);
}

void Test_CoreThread::workDown()
{   
    bool ret = programFab();
    if(ret) {
        ret = mNetWork->startProcess();
        if(ret) macSnCheck();
    }
}

void Test_CoreThread::run()
{
    if(isRun) return; else isRun = true;
    bool ret = initFun();
    if(ret) {
        switch (mPro->step) {
        case Test_Start: workDown(); break;
        case Test_Set:  programFab(); break;
        case Test_Collect:  mNetWork->startProcess(); break;
        }
    } else mPro->result = Test_Fail;

    workResult();
    isRun = false;
}
