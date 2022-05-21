/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_corethread.h"
#include "printer_bartender.h"

Test_CoreThread::Test_CoreThread(QObject *parent) : BaseThread(parent)
{

}

void Test_CoreThread::initFunSlot()
{
    BaseLogs::bulid(this);
    Printer_BarTender::bulid(this);
    mSn = Test_SerialNumber::bulid(this);
    mFab = test_FabPartition::bulid(this);
    mNetWork = Test_NetWork::bulid(this);
    connect(mFab, SIGNAL(fabSig(QString)), mNetWork, SIGNAL(msgSig(QString)));
}


bool Test_CoreThread::printer()
{
    bool ret = true;
    QString str = tr("标签打印 ");
    if(mPro->result != Test_Fail){
        sBarTend it;
        it.pn = "646130";
        it.sn = "00 04 74 " + mDt->sn;
        it.fw = mDt->fwRevision;
        it.hw = mDt->hwRevision;
        ret = Printer_BarTender::bulid(this)->printer(it);
        if(!ret) ret = Printer_BarTender::bulid(this)->printer(it);
        if(ret) str += tr("正常"); else str += tr("错误");
    } else str = tr("因测试未通过，标签未打印");
    return updatePro(str, ret);
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
        if(mDt->img.size()) {
            ret = mFab->programFull();
        }

        mSn->createSn();
        ret = mFab->workDown();
    }

    return ret;
}

bool Test_CoreThread::macSnCheck()
{
    bool ret = false;
    QString str = "serial Number" + tr("检查");
    if(mDt->ctrlBoardSerial != mDt->sn) {
        str += tr("错误：%1 %2").arg(mDt->ctrlBoardSerial).arg(mDt->sn);
    } else {
        str += tr("正确：%1 ").arg(mDt->ctrlBoardSerial); ret = true;
    }
    updatePro(str, ret);

    str = "Mac " + tr("检查");
    if(mDt->macAddress != mItem->macs.mac) {
        str += tr("错误：%1 %2").arg(mDt->macAddress).arg(mItem->macs.mac); ret = false;
    } else {
        str += tr("正确：%1 ").arg(mDt->macAddress); ret = true;
    }
    updatePro(str, ret);
    return ret;
}

bool Test_CoreThread::waitFor()
{
    emit waitSig();
    bool ret = false;
    isContinue = false;
    while(!isContinue) msleep(10);
    if(mPro->step < Test_Over) ret = true;
    else updatePro(tr("设备未正常启动.."), ret);

    return ret;
}

void Test_CoreThread::workDown()
{   
    bool ret = programFab();
    if(ret) {
        ret =  waitFor();
        if(ret) ret = mNetWork->startProcess();
        if(ret) ret = macSnCheck();
        if(ret) ret = printer();
    }
}

void Test_CoreThread::testNode()
{
    bool ret = mNetWork->startProcess();
    if(ret) ret = printer();
}


void Test_CoreThread::run()
{
    if(isRun) return; else isRun = true;
    bool ret = initFun();
    if(ret) {
        switch (mPro->step) {
        case Test_Start: workDown(); break;
        case Test_Set:  programFab(); break;
        case Test_Secure: mFab->secure_boot_prov(); break;
        case Test_Collect:  testNode(); break;
        }
    } else mPro->result = Test_Fail;

    workResult();
    isRun = false;
}
