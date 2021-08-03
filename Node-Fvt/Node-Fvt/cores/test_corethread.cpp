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
    mNetWork = Test_NetWork::bulid(this);
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
    return ret;
}

void Test_CoreThread::workDown()
{

}

void Test_CoreThread::run()
{
    if(isRun) return; else isRun = true;
    bool ret = initFun();
    if(ret) {
        switch (mPro->step) {
        case Test_Start: workDown(); break;
        }
    } else mPro->result = Test_Fail;

    workResult();
    isRun = false;
}