#ifndef PRINTERTHREAD_H
#define PRINTERTHREAD_H

#include "printerpixmap.h"

class PrinterThread : public QObject
{
    Q_OBJECT
    explicit PrinterThread(QObject *parent = nullptr);
public:
    static PrinterThread *bulid(QObject *parent = nullptr);
    void printer(sPrIt &it);

signals:
    void printerSig();

protected slots:
    void printerSlot();

private:
    sPrIt mIt;
    PrinterWid *mPrinter;
    PrinterPixmap *mPixmap;
};

#endif // PRINTERTHREAD_H
