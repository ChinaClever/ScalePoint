/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_corethread.h"
#include "baselogs.h"
#include "printer_bartender.h"

Test_CoreThread::Test_CoreThread(QObject *parent) : BaseThread(parent)
{

}

void Test_CoreThread::initFunSlot()
{
    BaseLogs::bulid(this);
    mYc = Yc_Obj::bulid(this);
    mDev = SP_ImmRtu::bulid(this);
    mAd = Ad_CoreThread::bulid(this);
    mExe = Test_Execute::bulid(this);
}

bool Test_CoreThread::enumDeviceType()
{    
    bool ret = mDev->enumDeviceType();
    if(!ret) {
        ret = mExe->startProcess();
        if(ret) ret = mDev->enumDeviceType();
    }

    QString str = tr("设备类型：");
    if(ret) str += mDt->dt; else str += tr("识别错误");
    return updatePro(str, ret);
}


bool Test_CoreThread::readDev()
{
    QString str = tr("请求地址 ");
    bool ret = mDev->requestAddr();
    if(ret) str += tr("正常：Addr=%1").arg(mDt->addr); else str += tr("错误");
    updatePro(str, ret, 1);

    if(ret) {
        ret = mDev->readVersion();
        str = tr("Modbus-RTU通讯 版本读取 ");
        if(ret) str += tr("正常"); else str += tr("错误");
        updatePro(str, ret);
    }

    if(ret) {
        ret = mDev->readSn(); str = tr("设备SN码读取 ");
        if(ret) str += tr("正常：SN %1").arg(mDt->sn); else str += tr("错误");
        updatePro(str, ret);
    }

    return ret;
}


bool Test_CoreThread::printer()
{
    bool ret = true;
    QString str = tr("标签打印 ");
    if((mPro->result != Test_Fail) && (Test_Start == mPro->step)){
        sBarTend it;
        it.pn = mDt->pn; it.sn = mDt->sn;
        it.fw = mDt->fw; it.hw = mDt->hw;
        ret = Printer_BarTender::bulid(this)->printer(it);
        if(!ret) ret = Printer_BarTender::bulid(this)->printer(it);
        if(ret) str += tr("正常"); else str += tr("错误");
    } else str = tr("因测试未通过，标签未打印");

    return updatePro(str, ret);
}


void Test_CoreThread::workResult()
{

    BaseLogs::bulid()->start();
    bool res = mYc->powerDown();
    QString str = tr("最终结果 ");
    if(mPro->result != Test_Fail) {
        str += tr("通过");
        if(mPr) printer();
    } else {
        res = false;
        str += tr("失败");
    }

    updatePro(str, res, 1);
    mPro->step = Test_Over;
}

bool Test_CoreThread::initFun()
{
    mPr = false;
    updatePro(tr("即将开始"));
    bool ret = mYc->powerOn();
    if(ret) ret = mExe->startProcess();
    if(ret) ret = enumDeviceType();
    if(ret) ret = readDev();

    return ret;
}

void Test_CoreThread::workDown()
{
    bool ret = mPr = mAd->startAdjust(); if(!ret) return;
    Ad_Resulting::bulid(this)->initRtuThread();
    for(int i=0; i<mData->size; ++i) {
        sLineData *line = &(mData->lines[i]);
        if(line->ele) {
            QString e = QString::number(line->ele/COM_RATE_ELE)+"Wh";
            QString str = tr("L%1 存在电能 %2").arg(i+1).arg(e);
            updatePro(str, false);
        }
    }
}

void Test_CoreThread::collectData()
{
    int cnt = 0;
    while(mPro->step == Test_Collect) {
        bool ret = mDev->readPduData();
        if(ret && (++cnt%30)) continue;
        QString str = tr("正在读取设备数据 %1").arg(cnt);
        if(!ret) str= tr("读取设备数据错误！");
        updatePro(str, ret, 2);
    }
    updatePro(tr("读取设备数据停止！"));
}


void Test_CoreThread::run()
{
    if(isRun) return; else isRun = true;
    bool ret = initFun();
    if(ret) {
        switch (mPro->step) {
        case Test_Start: workDown(); break;
        case Test_Collect: collectData(); break;
        case Test_Ading: mAd->startAdjust(); break;
        case Test_vert: mAd->verifyResult(); break;
        }
    } else mPro->result = Test_Fail;

    workResult();
    isRun = false;
}
