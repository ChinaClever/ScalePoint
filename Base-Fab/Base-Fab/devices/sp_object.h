#ifndef DEV_SCALEPOINT_H
#define DEV_SCALEPOINT_H

#include "baseobject.h"
#include "Function_Block.h"

struct sFrameFormat
{
    sFrameFormat() {addr=msb=0;}
    uchar fc;
    uchar addr;
    uchar msb;
    uchar lsb;
    ushort crc;

    uchar *reply;
};

class SP_Object : public BaseThread
{
    Q_OBJECT
public:
    explicit SP_Object(QObject *parent = nullptr);

    bool readSn();
    bool readVersion();
    bool enumDeviceType();
    bool onewireBus(int addr=1);
    bool requestAddr(int addr=1);
    int masterRequest(sFrameFormat &it);
    std::tuple<int, uchar *> masterRequest(uchar fc);
    bool writeSerial(uchar fc, uchar addr, uchar msb, uchar lsb);
    QByteArray masterRequest(uchar fc, uchar addr, uchar msb=0xFF, uchar lsb=0xFF);
    bool masterWrite(uchar fc, uchar addr, uchar msb=0xFF, uchar lsb=0xFF, bool ack=false);

protected:
    int toArray(sFrameFormat &it, uchar *cmd);
    int transmit(sFrameFormat &it, uchar *recv);
    int filterUpolData(sFrameFormat &it);
    bool checkCrc(uchar *recv, int len);
    void reflush();

protected slots:
    void initFunSlot();
};

#endif // DEV_SCALEPOINT_H
