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
    mExe = Test_Execute::bulid(this);
    mRtu = Dev_SocketRtu::bulid(this);
}

bool Test_CoreThread::readDev()
{
    QString str = tr("请求地址 ");
    bool ret = mRtu->requestAddr();
    if(ret) str += tr("正常"); else str += tr("错误");
    updatePro(str, ret);

    if(ret) {
        str = tr("Modbus RTU通讯 ");
        ret = mRtu->readVersion();
        if(ret) str += tr("正常"); else str += tr("错误");
        updatePro(str, ret);
    }

    return ret;
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

void Test_CoreThread::outputCtrl()
{
    updatePro(tr("Socket 打开所有的输出位"));
    mRtu->openAll(); msleep(500);

    updatePro(tr("Socket 关闭所有的输出位"));
    mRtu->closeAll(); sleep(1);

    updatePro(tr("Socket 再次打开所有的输出位"));
    mRtu->openAll();
}

bool Test_CoreThread::initFun()
{    
    bool ret = updatePro(tr("即将开始"));
    if(ret) ret = mExe->startProcess();
    if(ret) ret = readDev();

    return ret;
}

void Test_CoreThread::workDown()
{
    bool ret = mRtu->openOutput(1);
    QString str = tr("打开输出位 1 ");
    if(ret) str += tr("正常"); else str += tr("错误");
    updatePro(str, ret);

    ret = mRtu->closeOutput(1);
    str = tr("关闭输出位 1 ");
    if(ret) str += tr("正常"); else str += tr("错误");
    updatePro(str, ret);

}

void Test_CoreThread::run()
{
    if(isRun) return; else isRun = true;
    bool ret = initFun();
    if(ret) {
        switch (mPro->step) {
        case Test_Start: workDown(); break;
        case Test_Ctrl: outputCtrl(); break;
            // case Test_Zero: mFab->secure_boot_prov(); break;
        }
    } else mPro->result = Test_Fail;

    workResult();
    isRun = false;
}
