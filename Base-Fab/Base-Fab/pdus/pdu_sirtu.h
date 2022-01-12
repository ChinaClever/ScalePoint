#ifndef PDU_SIRTU_H
#define PDU_SIRTU_H

#include "pdu_objrtu.h"

class Pdu_SiRtu : public Pdu_ObjRtu
{
    Q_OBJECT
    explicit Pdu_SiRtu(QObject *parent = nullptr);
public:
    static Pdu_SiRtu *bulid(QObject *parent = nullptr);
    bool readPduData();

protected:
    int recvLine(int len);
    void initRtuItem(sRtuItem &it);
    bool recvPacket(uchar *buf, int len);
    int recvAcData(uchar *ptr, int line);
    int recvDcData(uchar *ptr, int line);
};

#endif // PDU_SIRTU_H
