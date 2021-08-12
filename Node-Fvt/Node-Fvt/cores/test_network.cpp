/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_network.h"

Test_NetWork::Test_NetWork(QObject *parent) : BaseThread(parent)
{

}

Test_NetWork *Test_NetWork::bulid(QObject *parent)
{
    static Test_NetWork* sington = nullptr;
    if(sington == nullptr)
        sington = new Test_NetWork(parent);
    return sington;
}

void Test_NetWork::initFunSlot()
{    
    mProcess = new QProcess(this);
    mProcess->setProcessChannelMode(QProcess::SeparateChannels);
    mProcess->setReadChannel(QProcess::StandardOutput);

    mUdp = new UdpRecvSocket(this);
    mUdp->initSocket(12306);
    this->start();
}

bool Test_NetWork::checkNet()
{    
    bool ret = true;
    QString ip = mDt->ip;
    for(int i=0; i<3; ++i) {
        ret = cm_pingNet(ip);
        if(ret) break; else delay(6);
    }

    QString str = tr("检测设备网络通讯 ") +ip;
    if(ret) str += tr(" 正常"); else str += tr(" 错误");
    return updatePro(str, ret);
}

QStringList Test_NetWork::getCmd()
{
    QStringList arguments;
    arguments << mDt->user << mDt->pwd;
    if(!mDt->aiFind) arguments << mDt->ip;

    return arguments;
}

bool Test_NetWork::startProcess()
{
    bool ret = mDt->aiFind;
    if(!ret) ret = checkNet();
    if(ret) {
        QString fn = "py_fvt_node.exe";
        QStringList cmd = getCmd();
        mProcess->startDetached(fn, cmd); // , QIODevice::ReadWrite

        mProcess->waitForStarted();
        updatePro(tr("正在启动测试程序"));
        mProcess->waitForFinished();



        mProcess->close();
    }

    return ret;
}


QString Test_NetWork::updateMacAddr(int step)
{
    sMac *it = &(mItem->macs);
    if(it->mac.size() > 5) {
        BaseLogs::bulid()->writeMac(it->mac);
        MacAddr *mac = MacAddr::bulid();
        it->mac = mac->macAdd(it->mac, step);
        Cfg::bulid()->write("mac", it->mac, "Mac");
    }

    return it->mac;
}

void Test_NetWork::readOutput()
{
    bool ret = mProcess->isReadable();
    if(ret) {
        QString str = mProcess->readAllStandardOutput();
        str += mProcess->readAllStandardError();
        qDebug() << "readOutput" << str;

        //QString strResult = QString::fromLocal8Bit(mProcess->readAllStandardOutput());
    }
}

void Test_NetWork::workDown()
{
    UdpBaseData *res = mUdp->getData();
    if(res) {
        QStringList list = QString(res->datagram).split(";");
        if(list.size() == 3) {
            QString str = list.first();
            bool pass = list.at(1).toInt();
            int fn = list.last().toInt();
            if(fn) {

            } else {
                updatePro(str, pass, 0);
            }
        } else {
            if(QString(res->datagram).contains("MAC-1")) mac = false; else
                qDebug() <<"Test_NetWork workDown err" << list.size();
        }
        delete res;
    } else {
        msleep(1);
    }
}

void Test_NetWork::run()
{
    isRun = true;
    while (isRun) {
        workDown();
        readOutput();
    }
}
