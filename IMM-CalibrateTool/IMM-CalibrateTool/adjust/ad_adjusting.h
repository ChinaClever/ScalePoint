#ifndef AD_ADJUSTING_H
#define AD_ADJUSTING_H

#include "yc_obj.h"

class Ad_Adjusting : public BaseThread
{
    Q_OBJECT
    explicit Ad_Adjusting(QObject *parent = nullptr);
public:
    static Ad_Adjusting *bulid(QObject *parent = nullptr);
    bool startAdjust();

protected:
    bool sentCmd();
    bool writeCmd(uchar fn, uchar line);
    bool transmit(uchar *buf, int len);

    bool readData();
    bool waitDcRecv();
    int readSerial(uchar *recv, int sec=25);
    bool recvStatus(uchar *recv, int len);
    bool updateStatus(ushort status);
    bool overWork(const QString &str);
};

#endif // AD_ADJUSTING_H
