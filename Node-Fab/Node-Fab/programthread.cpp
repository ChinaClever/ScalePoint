#include "programthread.h"

ProgramThread::ProgramThread(int id, QObject *parent) : QThread(parent)
{
    mId = id;
    isRun = false;
}


ProgramThread::~ProgramThread()
{
    isRun = false;
    wait();
}

void ProgramThread::workDown()
{
    QStringList ls;
    QProcess pro(this);

    QString fn = "firmware/tmp/%1.img";
    QString dev = "/dev/ttyACM"+ QString::number(mId);
    ls << "-y" << dev << fn.arg(mId) << "0x00000";

    QString cmd = "echo \"123456\" | sudo -S chmod 777 " + dev;
    system(cmd.toLatin1().data()); msleep(100);

    QString exe = "./firmware/tmp/at91recovery_%1";
    int ret = pro.execute(exe.arg(mId), ls);
    emit fabSig(ret);
}

void ProgramThread::run()
{
    if(isRun) return;
    isRun = true;
    workDown();
    isRun = false;
}
