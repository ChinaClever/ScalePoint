#ifndef DEV_SOCKETRTU_H
#define DEV_SOCKETRTU_H

#include "dev_object.h"

class Dev_SocketRtu : public Dev_Object
{
    Q_OBJECT
    explicit Dev_SocketRtu(QObject *parent = nullptr);
public:
    static Dev_SocketRtu *bulid(QObject *parent = nullptr);

    bool openAll();
    bool openOutput(uchar addr);

    bool closeAll();
    bool closeOutput(uchar addr);
};

#endif // DEV_SOCKETRTU_H
