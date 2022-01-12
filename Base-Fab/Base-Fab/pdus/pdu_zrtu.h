#ifndef DEV_RTUZPDU_H
#define DEV_RTUZPDU_H

#include "pdu_zctrl.h"

class Pdu_ZRtu : public Pdu_ZCtrl
{
    Q_OBJECT
    explicit Pdu_ZRtu(QObject *parent = nullptr);
public:
    static Pdu_ZRtu *bulid(QObject *parent = nullptr);

    bool readPduData();
protected:
    bool recvZpduVolCur(uchar *recv, int len);
    bool getZpduVolCur();
};

#endif // DEV_RTUZPDU_H
