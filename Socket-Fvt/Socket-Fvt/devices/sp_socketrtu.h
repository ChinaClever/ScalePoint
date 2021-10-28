#ifndef SP_SOCKETRTU_H
#define SP_SOCKETRTU_H

#include "sp_socketctrl.h"

class SP_SocketRtu : public SP_SocketCtrl
{
    Q_OBJECT
    explicit SP_SocketRtu(QObject *parent = nullptr);
public:
    static SP_SocketRtu *bulid(QObject *parent = nullptr);



};

#endif // SP_SOCKETRTU_H
