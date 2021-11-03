#ifndef BT_SERIAL_H
#define BT_SERIAL_H

#include "rtu_modbus.h"

struct sBtIt
{
    sBtIt() {vol=cur=0;}
    ushort vol;// 电压
    ushort cur; //电流
};

class Bt_Serial : public QThread
{
    Q_OBJECT
public:
    explicit Bt_Serial(QObject *parent = nullptr);

    void init(int id=1);
    bool readPacket(sBtIt &bt);

protected:
    sRtuItem initRtuItem();

private:
    RtuRw *mModbus;
};

#endif // BT_SERIAL_H
