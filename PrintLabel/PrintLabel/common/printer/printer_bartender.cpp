/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "printer_bartender.h"
#include "common.h"
#include <QDateTime>
#include <QCoreApplication>

Printer_BarTender::Printer_BarTender(QObject *parent , int port) : QObject(parent)
{
    mSocket = new QUdpSocket(this);
    //mSocket->bind(QHostAddress::LocalHost, 47755);
    mSocket->bind(QHostAddress::AnyIPv4, 47755);
    connect(mSocket,SIGNAL(readyRead()),this,SLOT(recvSlot()));
    mPort = port;
}

Printer_BarTender *Printer_BarTender::bulid(QObject *parent)
{
    static Printer_BarTender* sington = nullptr;
    if(sington == nullptr)
        sington = new Printer_BarTender(parent);
    return sington;
}

//QString Printer_BarTender::createOrder(sBarTend &it)
//{
//    QString str = "PN,HW,FW,Date,SN,QR\n";
//    str += it.pn + ","; str += it.hw + ","; str += it.fw + ",";
//    QString date = QDate::currentDate().toString("yy") + "W";
//    date += QString("%1").arg(QDate::currentDate().weekNumber(), 2, 10, QLatin1Char('0'));
//    str += date+","; str += "ETH " + it.sn + ","; it.sn = it.sn.remove(QRegExp("\\s"));
//    str += QString("G$K:%1%E$A:%2%M:%3$HW:%4$FW:%5").arg(it.pn).arg(it.sn).arg(date).arg(it.hw).arg(it.fw);
//    return str;
//}

QString Printer_BarTender::createBaseOrder(sBarTend &it)
{
    QString str = "PN,HW,FW,Date,SN,QR\n";
    str += it.pn + ","; str += it.hw + ","; str += it.fw + ",";

    str += it.date+","; str += "IEEE " + it.sn + ","; it.sn = it.sn.remove(QRegExp("\\s"));
    str += QString("G$K:%1$I:%2%Z$A:%3%M:%4$HW:%5$FW:%6").arg(it.pn).arg(it.code).arg(it.sn).arg(it.date).arg(it.hw).arg(it.fw);
    return str;
}

QString Printer_BarTender::createSocketOrder(sBarTend &it)
{
    QString str = "PN,HW,FW,Date,SN,QR\n";
    str += it.pn + ","; str += it.hw + ","; str += it.fw + ",";

    str += it.date+","; str +="SN " + it.sn + ","; it.sn = it.sn.remove(QRegExp("\\s"));
    str += QString("G$K:%1%$S:%2%M:%3$HW:%4$FW:%5").arg(it.pn).arg(it.sn).arg(it.date).arg(it.hw).arg(it.fw);
    return str;
}

QString Printer_BarTender::createNodeOrder(sBarTend &it)
{
    QString str = "PN,HW,FW,Date,SN,QR\n";
    str += it.pn + ","; str += it.hw + ","; str += it.fw + ",";

    str += it.date+","; str += "ETH " + it.sn + ","; it.sn = it.sn.remove(QRegExp("\\s"));
    str += QString("G$K:%1%E$A:%2%M:%3$HW:%4$FW:%5").arg(it.pn).arg(it.sn).arg(it.date).arg(it.hw).arg(it.fw);
    return str;
}


void Printer_BarTender::delay(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

bool Printer_BarTender::recvResponse(int sec)
{
    mRes = false;
    for(int i=0; i<10*sec; ++i) {
        if (mRes) break; else delay(100);
    }

    //mSocket->close();
    return mRes;
}

bool Printer_BarTender::printer(sBarTend &it , int port , int index)
{
    mPort = port;
    sendMsg("start", mPort, QHostAddress::Broadcast);

    QString order = "";
    if(index == 0)
        order = createBaseOrder(it);
    else if(index == 1)
        order = createSocketOrder(it);
    else
        order = createNodeOrder(it);
    sendMsg(order.toLocal8Bit(), mPort+1, QHostAddress::Broadcast);
    return recvResponse(10);
}

int Printer_BarTender::sendMsg(const QByteArray &msg, quint16 port, const QHostAddress &host)
{
    int ret = mSocket->writeDatagram(msg, host, port);
    if(ret > 0) mSocket->flush(); delay(100);
    return ret;
}

void Printer_BarTender::recvSlot()
{
    QByteArray array;
    while (mSocket->hasPendingDatagrams()) {
        array.resize(mSocket->bytesAvailable());
        mSocket->readDatagram(array.data() ,array.size());
        if(array.size()) mRes = true;
    }
}
