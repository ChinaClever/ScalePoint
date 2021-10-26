#ifndef DEV_SCALEPOINT_H
#define DEV_SCALEPOINT_H

#include "dev_object.h"
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

class Dev_ScalePoint : public Dev_Object
{
    Q_OBJECT
public:
    explicit Dev_ScalePoint(QObject *parent = nullptr);

    bool readVersion();
    bool enumDeviceType();
    bool requestAddr(int addr=1);
    int masterRequest(sFrameFormat &it);
    std::tuple<int, uchar *> masterRequest(uchar fc);
    bool writeSerial(uchar fc, uchar addr, uchar msb, uchar lsb);
    QByteArray masterRequest(uchar fc, uchar addr, uchar msb=0xFF, uchar lsb=0xFF);
    bool masterWrite(uchar fc, uchar addr, uchar msb=0xFF, uchar lsb=0xFF, bool ack=false);

protected:
    int toArray(sFrameFormat &it, uchar *cmd);
    bool checkCrc(uchar *recv, int len);
    int transmit(sFrameFormat &it, uchar *recv);
    int filterUpolData(sFrameFormat &it);
};

#endif // DEV_SCALEPOINT_H
