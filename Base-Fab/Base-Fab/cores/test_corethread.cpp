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
    mFvt = Test_BaseFvt::bulid(this);
    mExe = Test_Execute::bulid(this);
    mTokens = BaseTokens::bulid(this);
    mModbusJ7 = Rtu_Modbus::bulid(this)->get(1);
    mModbusJ8 = Rtu_Modbus::bulid(this)->get(2);
    connect(mFvt, SIGNAL(closeSig()), this, SIGNAL(closeSig()));
    connect(mFvt, SIGNAL(openSig(QString,int)), this, SIGNAL(openSig(QString,int)));
}


bool Test_CoreThread::printer()
{
    bool ret = true;
    QString str = tr("标签打印 ");
    if(mPro->result != Test_Fail){
        sBarTend it;
        it.pn = mDt->pn;
        it.sn = mDt->sn;
        it.fw = mDt->fw;
        it.hw = mItem->hw;
        it.code = mDt->code;
        //it.code = mTokens->codeCrc();
        ret = Printer_BarTender::bulid(this)->printer(it);
        if(!ret) ret = Printer_BarTender::bulid(this)->printer(it);
        if(ret) str += tr("正常"); else str += tr("错误");
    } else str = tr("因测试未通过，标签未打印");
    return updatePro(str, ret);
}

bool Test_CoreThread::fabBootloader()
{
    QStringList ls{"flash"};
    ls << "--masserase" << "--device";
    ls << "EFR32MG12P332F1024GM48";
    ls << QString("%1").arg(mItem->bootloader);
    return mExe->startProcess(ls);
}

bool Test_CoreThread::fabFile()
{
    QStringList ls{"flash"};
    ls << "--device" << "EFR32MG12P332F1024GM48";
    ls << QString("%1").arg(mItem->firmware);
    return mExe->startProcess(ls);
}

bool Test_CoreThread::fabTokens()
{
    sToken *token = mTokens->bulidTokens();
    mDt->sn = token->custom_eui;
    mDt->code = token->install_code;

    QStringList ls{"flash"};
    ls << "--device" << "EFR32MG12P332F1024GM48";
    ls << "--tokengroup" << "znet" << "--tokenfile" << TOKEN_FN;
    return mExe->startProcess(ls);
}

bool Test_CoreThread::workDown()
{
    bool ret = fabBootloader();
    if(ret) {
        ret = fabFile();
        if(ret) fabTokens();
    }

    return ret;
}

void Test_CoreThread::workResult(bool ret)
{
    if(!ret) mPro->result = Test_Fail;
    BaseLogs::bulid()->start();
    mPro->step = Test_Over;
}

bool Test_CoreThread::factoryWork()
{

    bool ret = false;
    ret = mFvt->workDown();

    if(ret) ret = waitFor();
    if(ret) ret = mFvt->getToken();
    if(ret) ret = printer();
    return ret;
}

bool Test_CoreThread::test(RtuRw *modbus)
{
    bool ret = false;
    if(modbus == nullptr) return ret;
    static uchar sent[14] = {0xA1,0x30,0x00,0x04,0x01,0x00,0x00,0x10,0x00,0xAE,0x50,0x87,0xCD,0xA5};
    int len = 14;
    static uchar recv[128] = {0};
    len = modbus->transmit(sent, len, recv, 10);
    if(len == 0) len = modbus->transmit(sent, len, recv, 10);
    int count = 30;

    if( len >= count ) ret = true;
    qDebug()<<ret <<"   ccccc "<<len<<endl;
    return ret;
}

bool Test_CoreThread::test485()
{
    bool ret = false;
    ret = test(this->mModbusJ7);
    QString str = "test J7 pass---------";;
    if(!ret){
        str = "test J7 failed!!!!!!!!";
        updatePro(str ,ret);
    }
    emit mExe->msgSig(str);
    if(ret) {
        ret = test(mModbusJ8);
        if(!ret){
            str = "test J8 failed!!!!!!!!";
            updatePro(str ,ret);
        }
        else
        {
            str = "test J8 pass---------";
        }
    }
    emit mExe->msgSig(str);
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

void Test_CoreThread::run()
{
    if(isRun) return; else isRun = true;
    bool ret = false;
    switch (mPro->step) {
    case Test_Factory:{ ret = factoryWork(); mPro->step = Test_Factory;}break;
    case Test_Start: ret = workDown(); break;
    case Test_Bootloader: ret = fabBootloader(); break;
    case Test_Firmware: ret = fabFile(); break;
    case Test_Token: ret = fabTokens(); break;
    case Test_Fvt: ret = test485(); break;
    }

    workResult(ret);
    isRun = false;
}
