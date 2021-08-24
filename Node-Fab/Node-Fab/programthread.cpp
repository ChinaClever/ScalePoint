/*
 *
 *  Created on: 2021年10月1日
 *      Author: Lzy
 */
#include "programthread.h"
static QReadWriteLock  *gRwLock = new QReadWriteLock();

ProgramThread::ProgramThread(int id, QObject *parent) : QThread(parent)
{
    mId = id;
    isRun = false;
    mRwLock = gRwLock;
    mDir = "./Firmware_Build/4.0.0.5-48035/";
}


ProgramThread::~ProgramThread()
{
    isRun = false;
    wait();
}

void ProgramThread::secure_boot_prov()
{
    QString cmd = "cd " + mDir +"secure_boot_prov-scalepoint-040000-48035/ \n"
                                "sh secure_boot_permanent_scalepoint.sh";
    QWriteLocker locker(mRwLock);
    system(cmd.toLocal8Bit().data());
}

bool ProgramThread::checkTime(const QTime &st)
{
    bool ret = false;
    QTime t(0,0,0,0);
    t = t.addSecs(st.secsTo(QTime::currentTime()));
    if(t.minute() >= 7 && t.second() > 25) ret = true;

    return ret;
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

    QTime st = QTime::currentTime();
    QString exe = mDir + "tmp/at91recovery_%1";
    pro.execute(exe.arg(mId), ls);
    pro.close();

    return checkTime(st);
}

bool ProgramThread::programFab()
{
    QStringList ls;
    QProcess pro(this);
    QString fn = mDir + "default_sn/%1.img";
    QString dev = "/dev/ttyACM"+ QString::number(mId);

    ls << "-y" << dev << fn.arg(mId) << "fab";
    QString exe = mDir + "tmp/at91recovery_%1";
    pro.start(exe.arg(mId), ls);

    bool ret = pro.waitForFinished();
    QByteArray bs = pro.readAllStandardOutput();
    bs +=  pro.readAllStandardError();
    QString res = QString::fromLocal8Bit(bs);
    if(res.contains("ERR")) ret = false;
    pro.close();

    return ret;
}

void ProgramThread::workDown()
{
    bool ret = programFull();
    if(ret) {
        //ret = programFab();
        // secure_boot_prov();
    }

    emit fabSig(ret);
}

void ProgramThread::run()
{
    if(isRun) return;
    isRun = true;
    workDown();
    isRun = false;
}
