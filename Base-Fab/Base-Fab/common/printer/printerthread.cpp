/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "printerthread.h"

PrinterThread::PrinterThread(QObject *parent) : QObject(parent)
{
    mPixmap = nullptr;
    mPrinter = new PrinterWid();
    connect(this, SIGNAL(printerSig()), this, SLOT(printerSlot()));
}

PrinterThread *PrinterThread::bulid(QObject *parent)
{
    static PrinterThread* sington = nullptr;
    if(sington == nullptr)
        sington = new PrinterThread(parent);
    return sington;
}

void PrinterThread::printer(sPrIt &it)
{
    mIt = it; emit printerSig();
}

void PrinterThread::printerSlot()
{
    if(mPixmap) delete mPixmap;
    mPixmap = new PrinterPixmap;
    QPixmap pix = mPixmap->moduleGrab(mIt);
    mPrinter->printerWid(pix);
    mPrinter->on_printsBtn_clicked();
}
