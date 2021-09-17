#ifndef DEV_SOCKETRTU_H
#define DEV_SOCKETRTU_H

#include "dev_object.h"

class Dev_SocketRtu : public Dev_Object
{
    Q_OBJECT
    explicit Dev_SocketRtu(QObject *parent = nullptr);
public:
    static Dev_SocketRtu *bulid(QObject *parent = nullptr);

    bool requestAddr();
    bool readVersion();

protected:
    int initRtuItem(uchar *cmd, uchar fn);
    bool checkCrc(uchar *recv, int len);
    int filterUpolData(uchar *recv, uchar fn);

};

#endif // DEV_SOCKETRTU_H
