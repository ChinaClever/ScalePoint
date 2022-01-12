#ifndef DEV_RTUMPDU_H
#define DEV_RTUMPDU_H

#include "pdu_zrtu.h"

class Pdu_MRtu : public Pdu_MCtrl
{
    Q_OBJECT
    explicit Pdu_MRtu(QObject *parent = nullptr);
public:
    static Pdu_MRtu *bulid(QObject *parent = nullptr);

    bool readPduData();
protected:
    bool recvMpduVolCur(uchar *recv, int);
    bool getMpduVolCur();

    bool recvMpduEle(uchar *recv, int len);
    int getMpduEle();
};

#endif // DEV_RTUMPDU_H
