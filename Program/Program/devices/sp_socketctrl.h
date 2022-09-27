#ifndef DEV_SOCKETRTU_H
#define DEV_SOCKETRTU_H

#include "sp_object.h"

class SP_SocketCtrl : public SP_Object
{
    Q_OBJECT
public:
    explicit SP_SocketCtrl(QObject *parent = nullptr);

    bool openAll();
    bool openOutput(uchar addr);

    bool closeAll();
    bool closeOutput(uchar addr);
    int readOutput(uchar addr);
    bool openAll_front4();
    bool closeAll_front4();
    bool openAll_back4();

    bool measRot(uchar addr, uint &t);
    QString storeValue();

protected:
    bool ctrlOutput(uchar addr, uchar status);
};

#endif // DEV_SOCKETRTU_H
