#ifndef DEV_SOCKETRTU_H
#define DEV_SOCKETRTU_H

#include "dev_scalepoint.h"

class Dev_SocketRtu : public Dev_ScalePoint
{
    Q_OBJECT
    explicit Dev_SocketRtu(QObject *parent = nullptr);
public:
    static Dev_SocketRtu *bulid(QObject *parent = nullptr);

    bool openAll();
    bool openOutput(uchar addr);

    bool closeAll();
    bool closeOutput(uchar addr);
    int readOutput(uchar addr);
    bool ctrlOutput(uchar addr, uchar status);

    bool measRot(uchar addr);
    bool storeValue(uchar addr);
};

#endif // DEV_SOCKETRTU_H
