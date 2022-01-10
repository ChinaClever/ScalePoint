#ifndef PDU_IPRTU_H
#define PDU_IPRTU_H

#include "pdu_sirtu.h"

class Pdu_IpRtu : public Pdu_ObjRtu
{
    Q_OBJECT
    explicit Pdu_IpRtu(QObject *parent = nullptr);
public:
    static Pdu_IpRtu *bulid(QObject *parent = nullptr);
    bool readPduData();

protected:
    int recvLine(int len);
    void initRtuItem(sRtuItem &it);
    bool recvPacket(uchar *buf, int len);

    int recvDataV1(uchar *ptr);
    int recvDataV3(uchar *ptr);
    uchar *getSwitch(uchar *ptr, int line, uchar *value);
};

#endif // PDU_IPRTU_H
