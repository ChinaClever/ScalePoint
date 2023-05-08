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
    mBt->init(1);
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
    } else str += tr("识别错误"); updatePro(str, ret);

    if(ret) {
        bool res = mRtu->onewireBus();
        str = "OneWire Bus " + tr("检测 ");
        if(res) str += tr("正常"); else str += tr("错误");
        updatePro(str, res);
    }

    return ret;
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
    if((mPro->result != Test_Fail) && (Test_Start == mPro->step)){
        sBarTend it;
        it.pn = mDt->pn; it.sn = mDt->sn;
        it.fw = mDt->fw; it.hw = mItem->hw;
        ret = Printer_BarTender::bulid(this)->printer(it);
        if(!ret) ret = Printer_BarTender::bulid(this)->printer(it);
        if(ret) str += tr("正常"); else str += tr("错误");
    } else str = tr("因测试未通过，标签未打印");

    return updatePro(str, ret);
}

void Test_CoreThread::workResult()
{
    bool res = true;  if(mPr)printer();  ///////============
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

//    updatePro(tr("Socket 打开所有的输出位"));
//    mRtu->openAll(); msleep(500);

//    if(mDt->outputs == 8){
//        updatePro(tr("Socket 打开前4个输出位"));
//        mRtu->openAll_front4(); sleep(20);
//        updatePro(tr("Socket 关闭前4个输出位"));
//        mRtu->closeAll_front4();
//        updatePro(tr("Socket 打开后4个输出位"));
//        mRtu->openAll_back4(); sleep(20);
//    }
//    else{
        updatePro(tr("Socket 打开所有的输出位"));
        mRtu->openAll();
//    }

//    updatePro(tr("Socket 再次打开所有的输出位"));
//    mRtu->openAll();
}

bool Test_CoreThread::btCurCheck()
{
    sBtIt bt;
    bool ret = mBt->readPacket(bt);
    for(int i = 0 ; i < 10 ; i++) {
        if(!ret){ ret = mBt->readPacket(bt); msleep(100);}
        else break;
    }
    if(ret) {
        QString str = tr("检测供电电压：%1V ").arg(bt.vol/100.0);
        if((bt.vol > 1100) && (bt.vol < 1300)) ret = true; else ret = false;
        if(ret) str += tr("正确"); else str += tr("错误"); updatePro(str, ret);

        str = tr("检测消耗电流：%1A ").arg(bt.cur/1000.0);
        if((bt.cur >= 1) && (bt.cur < 90)) ret = true; else ret = false;
        if(ret) str += tr("正确"); else str += tr("错误");  updatePro(str, ret);

        str = tr("检测消耗功率：%1W ").arg(bt.pow/100.0);
        if((bt.pow > 1) && (bt.pow < 50)) ret = true; else ret = false;
        if(ret) str += tr("正确"); else str += tr("错误");  updatePro(str, ret);
    } else updatePro(tr("外购计量板通讯错误"), ret);

    return ret;
}

bool Test_CoreThread::hasCurLoad()
{
    bool ret = false;
    sPduData *pduData = mPdu->getPduData();
    for(int i=0; i<pduData->size; ++i) {
        if(pduData->cur.value[i]) ret = true;
    }
    return ret;
}

bool Test_CoreThread::noCurLoad()
{
    bool ret = true;
    sPduData *pduData = mPdu->getPduData();
    for(int i=0; i<pduData->size; ++i) {
        if(pduData->cur.value[i]) ret = false;
    }
    return ret;
}

bool Test_CoreThread::relayCheck(int id)
{
    sPduData *pduData = mPdu->getPduData();
    mPdu->openAllSwitch(); mdelay(1);

    bool ret = mPdu->readPduData(); QString str = tr("PDU执行板 输出位 %1 打开 ").arg(id+1);
    if(1 == pduData->sw[id]) ret = true; else ret = false;
    if(ret) str += tr("正确"); else str += tr("错误"); updatePro(str, ret, 1);

    ret = mRtu->closeOutput(id+1); str = tr("Socket 输出位%1 闭合 ").arg(id+1);
    if(ret) str += tr("正确"); else str += tr("错误"); updatePro(str, ret, 5);

    str = tr("Socket 输出位%1 带载电流 ").arg(id+1); mPdu->readPduData();
    if(hasCurLoad()) ret = true; else ret = false;
    if(ret) str += tr("正常"); else str += tr("异常"); updatePro(str, ret);

    ret = mRtu->openOutput(id+1); str = tr("Socket 输出位%1 断开 ").arg(id+1);
    if(ret) str += tr("正确"); else str += tr("错误"); updatePro(str, ret, 3);

    str = tr("Socket 输出位%1 空载电流 ").arg(id+1); mPdu->readPduData();
    if(noCurLoad()) ret = true; else ret = false;
    if(ret) str += tr("正常"); else str += tr("错误");
    return updatePro(str, ret);
}

bool Test_CoreThread::outputCheck()
{        
    mPdu->initData(3);
    bool ret = mPdu->readPduData();
    if(!ret) ret = mPdu->readPduData();
    if(ret) {
        for(int i=0; i<mDt->outputs; ++i) relayCheck(i);
    } else updatePro(tr("PDU执行板 通讯错误"), ret);

    return ret;
}

bool Test_CoreThread::initFun()
{    
    mPr = false;
    bool ret = updatePro(tr("即将开始，正在复位串口板"));
    if(ret) ret = mExe->startProcess();
    QThread::msleep(5000);
    mExe->startProcess();
    QThread::msleep(5000);
    mExe->startProcess();
    if(ret) ret = enumDeviceType();
    if(ret) ret = readDev();
    return ret;
}

bool Test_CoreThread::zeroMeasRot()
{
    mPdu->closeAllSwitch();
    uint value = 0; bool ret = true;
    updatePro(tr("过零检测即将开始"),ret, 7);
    for(int i=1; i<=mDt->outputs; ++i) {
        bool res = mRtu->measRot(i, value);
        QString str = tr("Socket 输出位%1 ").arg(i);
        if(res) str += tr("继电器工作时间"); else str += tr("过零操作错误");
        updatePro(str+tr(" %1ms").arg(value/100.0), res, 5); if(!res) ret = res;
    }

    if(ret) {
        QString str = tr("将值存储到闪存中"); delay(2);
        QString res = mRtu->storeValue(); ret = res.contains("OK");
        if(ret) str += tr("正常 "); else str += tr("错误 ");
        updatePro(str+res, ret);
    }

    return ret;
}

void Test_CoreThread::workDown()
{

    mPr = true;
    //btCurCheck();
    ////outputCheck();
    zeroMeasRot();
    outputCtrl();
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
