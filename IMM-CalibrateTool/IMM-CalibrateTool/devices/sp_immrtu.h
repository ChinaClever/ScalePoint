#ifndef DEV_IMMRTU_H
#define DEV_IMMRTU_H

#include "sp_immobj.h"

class SP_ImmRtu : public SP_ImmObj
{
    Q_OBJECT
    explicit SP_ImmRtu(QObject *parent = nullptr);

public:
    static SP_ImmRtu *bulid(QObject *parent = nullptr);
    bool readPduData();

protected:
    int recvLine(int len);
    bool recvPacket(uchar *ptr, int len, sDevObj *obj);
};

#endif // DEV_IMMRTU_H
