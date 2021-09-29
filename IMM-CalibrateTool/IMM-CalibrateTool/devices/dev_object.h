#ifndef DEV_OBJECT_H
#define DEV_OBJECT_H

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

class Dev_Object : public BaseThread
{
    Q_OBJECT
public:
    explicit Dev_Object(QObject *parent = nullptr);
    sDevData *getDev() {return mDev;}
    virtual bool readPduData()=0;

    bool readVersion();
    bool requestAddr(int addr=1);

    bool masterWrite(sFrameFormat &it);
    bool masterWrite(uchar fc, uchar addr, uchar msb, uchar lsb);

    int masterRequest(sFrameFormat &it);
    std::tuple<int, uchar *> masterRequest(uchar fc);
    QByteArray masterRequest(uchar fc, uchar addr, uchar msb, uchar lsb);

protected:
    int toArray(sFrameFormat &it, uchar *cmd);
    bool checkCrc(uchar *recv, int len);
    int transmit(sFrameFormat &it, uchar *recv);
    int filterUpolData(sFrameFormat &it);

    uchar *toInt(uchar *ptr, uint &value);
    uchar *toShort(uchar *ptr, ushort &value);
    void reflush();
};

#endif // DEV_OBJECT_H
