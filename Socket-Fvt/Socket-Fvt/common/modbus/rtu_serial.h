#ifndef RTU_SERIAL_H
#define RTU_SERIAL_H

#include "config.h"
#include "datapacket.h"

class Rtu_Serial : public QThread
{
    Q_OBJECT
public:
    explicit Rtu_Serial(QObject *parent = nullptr);

    uchar xorNum(uchar *buf, int len);
    ushort rtu_crc(uchar *buf, int len);
    ushort CRC16(uchar *ptr, int len);

    bool changeBaudRate();
    bool setBaudRate(qint32 br);
    void init(SerialPort *s){mSerial=s;}
    int readSerial(quint8 *cmd, int secs);
    bool writeSerial(quint8 *cmd, int len);
    int transmit(uchar *sent, int len, uchar *recv, int secs);
    void reflush() {mSerial->reflush();}
    QByteArray readSerial(int msecs=100) {return mSerial->readSerial(msecs);}

protected:
    ushort calccrc (ushort crc, uchar crcbuf);

private:
    SerialPort *mSerial;
};

#endif // RTU_SERIAL_H
