/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_corethread.h"
#include "baselogs.h"

Test_CoreThread::Test_CoreThread(QObject *parent) : BaseThread(parent)
{

}

void Test_CoreThread::initFunSlot()
{
    BaseLogs::bulid(this);
    Dev_ImmRtu::bulid(this);
    mYc = Yc_Obj::bulid(this);
    mAd = Ad_CoreThread::bulid(this);
    mSn = Sn_SerialNum::bulid(this);
}


bool Test_CoreThread::readOutput(QProcess &pro)
{
    bool ret, res = false;
    do {
        ret = pro.waitForFinished(1000);
        QByteArray bs = pro.readAllStandardOutput();
        bs +=  pro.readAllStandardError();
        QString str = QString::fromLocal8Bit(bs);
        if(str.contains("CONNECTED")) res = true; //else str = str.simplified();
        QStringList ls = str.split("\n" /*, QString::SkipEmptyParts*/);
        for(int i=0; i<ls.size(); ++i) updatePro(ls.at(i).simplified());
    } while(!ret);
    pro.close();

    return res;
}

bool Test_CoreThread::execute()
{
    bool ret = false;
    QString exe = "SimpleIO_UM_CSExampleCode.exe";
    QFile file(exe);
    if (file.exists()){
        QProcess pro(this);
        pro.start(exe);
        ret = readOutput(pro);
        if(!ret) updatePro(tr("串口未正常连接") , ret);
    } else {
        updatePro(exe+tr(" 文件不存在") , ret);
    }

    return ret;
}


bool Test_CoreThread::readDev()
{
    QString str = tr("请求地址 ");
    Dev_ImmRtu *dev = Dev_ImmRtu::bulid();
    bool ret = dev->requestAddr();
    if(ret) str += tr("正常"); else str += tr("错误");
    updatePro(str, ret);

    if(ret) {
        str = tr("Modbus RTU通讯 ");
        ret = dev->readVersion();
        if(ret) str += tr("正常"); else str += tr("错误");
        updatePro(str, ret);
    }

    return ret;
}

void Test_CoreThread::workResult()
{
    BaseLogs::bulid()->start();
    bool res = mYc->powerDown();
    QString str = tr("最终结果 ");
    if(mPro->result != Test_Fail) {
        str += tr("通过");
    } else {
        res = false;
        str += tr("失败");
    }

    updatePro(str, res, 1);
    mPro->step = Test_Over;
}

bool Test_CoreThread::initFun()
{
    updatePro(tr("即将开始"));
    bool ret = mYc->powerOn();
    if(ret) ret = execute();
    if(ret) ret = readDev();
    if(ret) ret = mSn->snEnter();
    return ret;
}

void Test_CoreThread::workDown()
{
    bool ret = mAd->startAdjust(); if(!ret) return;
    Ad_Resulting::bulid(this)->initRtuThread();
    for(int i=0; i<LINE_NUM; ++i) {
        sLineData *line = &(mData->lines[i]);
        if(line->ele.active) {
            QString e = QString::number(line->ele.active/COM_RATE_ELE)+"Wh";
            QString str = tr("L%1 存在电能 %2").arg(i+1).arg(e);
            updatePro(str, false);
        }
    }
}

void Test_CoreThread::collectData()
{
    int cnt = 0;
    Dev_Object *dev = Dev_ImmRtu::bulid();
    while(mPro->step == Test_Collect) {
        bool ret = dev->readPduData();
        if(ret && (++cnt%30)) continue;
        QString str = tr("正在读取设备数据 %1").arg(cnt);
        if(!ret && (cnt>5)) str= tr("读取设备数据错误！");
        updatePro(str, ret, 2);
    }

    QString str = tr("读取设备数据停止！");
    updatePro(str);
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
