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
    Test_BsThread::bulid(this);
    mYc = Yc_Obj::bulid(this);
    mDev = SP_ImmRtu::bulid(this);
    mAd = Ad_CoreThread::bulid(this);
    mExe = Test_Execute::bulid(this);
    mSocket = SP_SocketRtu::bulid(this);
    mBt = new Bt_Serial(this);
}

bool Test_CoreThread::resDev()
{    
    mDev->resDev(); msleep(120);
    return mExe->startProcess();
}

bool Test_CoreThread::enumDeviceType()
{
    bool ret = true;
    if(!mDt->dt.contains("IMM")) ret = mDev->enumDeviceType();
    if(!ret) ret = mDev->enumDeviceType();

    QString str = tr("设备类型：");
    if(ret) str += mDt->dt; else str += tr("识别错误");
    return updatePro(str, ret);
}

bool Test_CoreThread::readDev()
{
    QString str = tr("请求地址 ");
    bool ret = mDev->requestAddr();
    if(ret) str += tr("正常：Addr=%1").arg(mDt->addr); else str += tr("错误");
    updatePro(str, ret);

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
    if(mPro->result != Test_Fail){
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
    bool res = mYc->powerDown();
    QString str = tr("最终结果 ");
    if(mPro->result != Test_Fail) {
        if(mPr) res = printer();
        if(res)
            str += tr("通过");
        else
            str += tr("失败");
    } else {
        res = false;
        msleep(1650);
        str += tr("失败");
    }

    updatePro(str, res, 1); mPro->step = Test_Over;
    mSocket->closeOutput(3); BaseLogs::bulid()->start();
    sleep(1); mSocket->closeOutput(2);
}

bool Test_CoreThread::cylinderDown()
{
    bool ret = resDev();
    QString str = tr("气缸初始化");
    YC_Ac92b::bulid()->setVol(220);
    if(ret) ret = mSocket->enumDeviceType();
    if(ret) str += tr("正常"); else str += tr("失败"); updatePro(str, ret);
    if(ret) {mSocket->openOutput(2); mSocket->openOutput(3);}
    return ret;
}

bool Test_CoreThread::initFun()
{
    mPr = false;
    updatePro(tr("即将开始"));
    bool ret = cylinderDown();
    if(ret) ret = enumDeviceType();
    if(mPro->step != Test_Bs && mPro->step != Test_Print)
        if(ret) ret = mYc->powerOn();
    if(mPro->step == Test_Collect || mPro->step == Test_Print) ret = true;
    if(ret) ret = readDev();///
    return ret;
}

void Test_CoreThread::workDown()
{
    //bool ret = btCurCheck();
    bool ret = true;
    if(ret) {ret = mPr = mAd->startAdjust(); if(!ret) return;}
    Ad_Resulting::bulid(this)->initRtuThread();
    for(int i=0; i<mData->size; ++i) {
        sLineData *line = &(mData->lines[i]);
        if(line->ele) {
            QString e = QString::number(line->ele/COM_RATE_ELE , 'f' , 1)+"kWh";
            QString str = tr("L%1 存在电能 %2").arg(i+1).arg(e); updatePro(str, false);
        }
    }
}

bool Test_CoreThread::btCurCheck()
{
    sBtIt bt; mBt->init(2);
    bool ret = mBt->readPacket(bt);
    if(ret) {
        QString str = tr("检测供电电压：%1V ").arg(bt.vol/100.0);
        if((bt.vol > 1100) && (bt.vol < 1300)) ret = true; else ret = false;
        if(ret) str += tr("正确"); else str += tr("错误"); updatePro(str, ret);

        str = tr("检测消耗电流：%1A ").arg(bt.cur/1000.0);
        if((bt.cur > 1) && (bt.cur < 90)) ret = true; else ret = false;
        if(ret) str += tr("正确"); else str += tr("错误");  updatePro(str, ret);

        str = tr("检测消耗功率：%1W ").arg(bt.pow/100.0);
        if((bt.pow > 1) && (bt.pow < 50)) ret = true; else ret = false;
        if(ret) str += tr("正确"); else str += tr("错误");  updatePro(str, ret);
    } else updatePro(tr("外购计量板通讯错误"), ret);

    return ret;
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
    if(ret){
        switch (mPro->step) {
        case Test_Start: workDown(); break;
        case Test_Collect: collectData(); break;
        case Test_Ading: mAd->startAdjust(); break;
        case Test_vert: mAd->verifyResult(); break;
        case Test_Bs:  Test_BsThread::bulid()->workDown(); break;
        case Test_Print:  mPr = true; break;
        }
    }
    workResult();

    isRun = false;
}
