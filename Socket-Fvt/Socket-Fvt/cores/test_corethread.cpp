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
    mExe = Test_Execute::bulid(this);
    mRtu = SP_SocketRtu::bulid(this);
    mPdu = Pdu_ZRtu::bulid(this);
    mBt = new Bt_Serial(this);
}

bool Test_CoreThread::enumDeviceType()
{
    QString str = tr("设备类型：");
    bool ret = mRtu->enumDeviceType();
    if(!ret) {
        ret = mExe->startProcess();
        if(ret) ret = mRtu->enumDeviceType();
    }

    if(ret) {
        str += tr("%1；输出位：%2").arg(mDt->dt).arg(mDt->outputs);
    } else str += tr("错误");
    return updatePro(str, ret);
}


bool Test_CoreThread::readDev()
{
    QString str = tr("请求地址 ");
    bool ret = mRtu->requestAddr();
    if(ret) str += tr("正常：Addr=%1").arg(mDt->addr); else str += tr("错误");
    updatePro(str, ret);

    if(ret) {
        ret = mRtu->readVersion();
        str = tr("Modbus-RTU通讯 版本读取 ");
        if(ret) str += tr("正常"); else str += tr("错误");
        updatePro(str, ret);
    }

    if(ret) {
        ret = mRtu->readSn(); str = tr("设备SN码读取 ");
        if(ret) str += tr("正常：SN %1").arg(mDt->sn); else str += tr("错误");
        updatePro(str, ret);
    }

    return ret;
}

bool Test_CoreThread::printer()
{
    bool ret = true;
    QString str = tr("标签打印 ");
    if(mPro->result != Test_Fail) {
        sBarTend it;
        it.pn = mDt->pn;
        it.sn = mDt->sn;
        it.fw = mDt->fw;
        it.hw = mItem->hw;
        ret = Printer_BarTender::bulid(this)->printer(it);
        if(!ret) ret = Printer_BarTender::bulid(this)->printer(it);
        if(ret) str += tr("正常"); else str += tr("错误");
    }

    return updatePro(str, ret);
}

void Test_CoreThread::workResult()
{
    bool res = true; printer();
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

void Test_CoreThread::outputCtrl()
{
    updatePro(tr("Socket 打开所有的输出位"));
    mRtu->openAll(); msleep(500);

    updatePro(tr("Socket 关闭所有的输出位"));
    mRtu->closeAll(); sleep(1);

    updatePro(tr("Socket 再次打开所有的输出位"));
    mRtu->openAll();
}

bool Test_CoreThread::btCurCheck()
{
    sBtIt bt; mBt->init(1);
    bool ret = mBt->readPacket(bt);
    if(ret) {
        QString str = tr("检测供电电压：%1V ").arg(bt.vol/100.0);
        if((bt.vol > 1100) && (bt.vol < 1300)) ret = true; else ret = false;
        if(ret) str += tr("正确"); else  str += tr("错误");
        updatePro(str, ret);

        str = tr("检测消耗电流：%1A ").arg(bt.cur/1000.0);
        if((bt.cur > 100) && (bt.cur < 1400)) ret = true; else ret = false;
        if(ret) str += tr("正确"); else  str += tr("错误");
        updatePro(str, ret);
    } else updatePro(tr("外购计量板通讯错误"), ret);

    return ret;
}

bool Test_CoreThread::relayCheck(int id)
{
    sPduData *pduData = mPdu->getPduData();
    mPdu->openOutputSwitch(id); mdelay(1);
    mPdu->openOnlySwitch(id); mdelay(1);
    mPdu->closeOtherSwitch(id); mdelay(1);

    bool ret = mRtu->closeOutput(id+1);
    QString str = tr("Socket 输出位%1 断开 ").arg(id+1);
    if(ret) str += tr("正确"); else str += tr("错误"); updatePro(str, ret, 1);

    mPdu->readPduData(); str = tr("PDU执行板 输出位 %1 打开 ").arg(id+1);
    if(1 == pduData->sw[id]) ret = true; else ret = false;
    if(ret) str += tr("正确"); else str += tr("错误"); updatePro(str, ret, 1);

    str = tr("Socket 输出位%1 空载电流 ").arg(id+1); mPdu->readPduData();
    if(pduData->cur.value[id]) ret = false; else ret = true;
    if(ret) str += tr("正常"); else str += tr("错误：%1").arg(pduData->cur.value[id]);
    updatePro(str, ret, 1);

    mRtu->openOutput(id+1);
    str = tr("Socket 输出位%1 闭合 ").arg(id+1);
    if(ret) str += tr("正确"); else str += tr("错误"); updatePro(str, ret, 4);

    str = tr("Socket 输出位%1 带载电流 ").arg(id+1); mPdu->readPduData();
    if(pduData->cur.value[id]) ret = true; else ret = false;
    if(ret) str += tr("正常：%1").arg(pduData->cur.value[id]); else str += tr("异常");
    updatePro(str, ret, 1);

    return ret;
}


bool Test_CoreThread::outputCheck()
{        
    mPdu->initData(3);
    bool ret = mPdu->readPduData();
    if(ret) {
        for(int i=0; i<mDt->outputs; ++i) relayCheck(i);
    } else updatePro(tr("PDU执行板通讯错误"), ret);

    return ret;
}

bool Test_CoreThread::initFun()
{    
    bool ret = updatePro(tr("即将开始"));
    if(ret) ret = mExe->startProcess();
    if(ret) ret = enumDeviceType();
    if(ret) ret = readDev();
    return ret;
}

bool Test_CoreThread::zeroMeasRot()
{
    uint value = 0;
    bool ret = true;  mPdu->closeAllSwitch(); delay(4);
    for(int i=1; i<=mDt->outputs; ++i) {
        bool res = mRtu->measRot(i, value);
        QString str = tr("Socket 输出位%1 ").arg(i);
        if(res) str += tr("继电器工作时间"); else str += tr("过零操作错误");
        updatePro(str+tr(" %1ms").arg(value/100.0), res, 1); if(!res) ret = res;
    }

    if(ret) {
        QString str = tr("将值存储到闪存中");
        QString res = mRtu->storeValue(); ret = res.contains("OK");
        if(ret) str += tr("正常 "); else str += tr("错误 ");
        updatePro(str+res, ret);
    }

    return ret;
}

void Test_CoreThread::workDown()
{
    btCurCheck();
    outputCheck();
    zeroMeasRot();
}

void Test_CoreThread::run()
{
    if(isRun) return; else isRun = true;
    bool ret = initFun();
    if(ret) {
        switch (mPro->step) {
        case Test_Start: workDown(); break;
        case Test_Ctrl: outputCtrl(); break;
        case Test_Relay: outputCheck(); break;
        case Test_Zero: zeroMeasRot(); break;
        }
    } else mPro->result = Test_Fail;

    workResult();
    isRun = false;
}
