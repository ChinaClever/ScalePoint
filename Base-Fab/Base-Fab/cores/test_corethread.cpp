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
        it.code = mTokens->codeCrc();
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
        if(ret) ret = mFvt->workDown();
        if(ret) printer();
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
    bool ret = fabTokens();
    if(ret) ret = mFvt->workDown();
    return ret;
}

void Test_CoreThread::run()
{
    if(isRun) return; else isRun = true;
    bool ret = false;
    switch (mPro->step) {
    case Test_Factory: ret = factoryWork(); break;
    case Test_Start: ret = workDown(); break;
    case Test_Bootloader: ret = fabBootloader(); break;
    case Test_Firmware: ret = fabFile(); break;
    case Test_Token: ret = fabTokens(); break;
    case Test_Fvt: ret = mFvt->workDown(); break;
    }

    workResult(ret);
    isRun = false;
}
