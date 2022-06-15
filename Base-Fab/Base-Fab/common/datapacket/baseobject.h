#ifndef OBJECTTHREAD_H
#define OBJECTTHREAD_H

#include "datapacket.h"
#include "rtu_modbus.h"

class BaseObject
{
public:
    BaseObject();
    void init() {mPacket->init();}

protected:
    sDevType *mDt;
    sDevData *mDev;
    RtuRw *mModbus;
    RtuRw *mModbusJ7;
    RtuRw *mModbusJ8;
    sCfgItem *mItem;
    sProgress *mPro;
    // sDevObj *mData;
    sDataPacket *mPacket;
};


class BaseThread : public QThread, public BaseObject
{
    Q_OBJECT
public:
    explicit BaseThread(QObject *parent = nullptr);
    ~BaseThread();

    bool updatePro(const QString &str, bool pass=true, int sec=0);
    bool delay(int s=1) {return mdelay(6*s);}
    bool mdelay(int s=1);

protected slots:
    virtual void initFunSlot();

protected:
    bool isRun;

};

#endif // OBJECTTHREAD_H
