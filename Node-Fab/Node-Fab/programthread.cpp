/*
 *
 *  Created on: 2021年10月1日
 *      Author: Lzy
 */
#include "programthread.h"

ProgramThread::ProgramThread(int id, QObject *parent) : QThread(parent)
{
    mId = id;
    isRun = false;
    mDir = "./Firmware_Build/4.0.0.5-48035/";
}


ProgramThread::~ProgramThread()
{
    isRun = false;
    wait();
}


void ProgramThread::updatePro(QString &str)
{
    int id = 0;
    bool ok = false;
    if(str.contains("Flash Erase")) id = 1;
    if(str.contains("Flash Write")) id = 2;
    QString res = str.split("] [ ").last().split("%]").first().trimmed();
    int ret = res.toInt(&ok);
    if(ok && ret <= 100) emit proSig(id, ret);
}

bool ProgramThread::readOutput(QProcess &pro)
{
    bool ret, res = true;
    do {
        ret = pro.waitForFinished(500);
        QByteArray bs = pro.readAllStandardOutput();
        bs +=  pro.readAllStandardError();
        QString str = QString::fromLocal8Bit(bs);
        if(str.contains("ERR")) res = false; //else str = str.simplified();
        else updatePro(str);
    } while(!ret);

    pro.close();
    return res;
}

bool ProgramThread::programFull()
{
    QStringList ls;
    QProcess pro(this);

    QString fn = mDir + "tmp/%1.img";
    QString dev = "/dev/ttyACM"+ QString::number(mId);
    ls << "-y" << dev << fn.arg(mId) << "0x00000";

    QString cmd = "echo \"123456\" | sudo -S chmod 777 " + dev;
    system(cmd.toLatin1().data()); msleep(100);

    QString exe = mDir + "tmp/at91recovery_%1";
    pro.start(exe.arg(mId), ls);
    return readOutput(pro);
}

void ProgramThread::workDown()
{
    bool ret = programFull();
    emit fabSig(ret);
}

void ProgramThread::run()
{
    if(isRun) return;
    isRun = true;
    workDown();
    isRun = false;
}
