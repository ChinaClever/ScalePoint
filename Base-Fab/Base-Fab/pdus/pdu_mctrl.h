#ifndef DEV_CTRLMPDU_H
#define DEV_CTRLMPDU_H

#include "pdu_objctrl.h"

class Pdu_MCtrl : public Pdu_ObjCtrl
{
    Q_OBJECT
public:
    explicit Pdu_MCtrl(QObject *parent = nullptr);
    bool factorySet();

protected:
    void funSwitch(uchar *on, uchar *off);
    void funClearEle(uchar *buf);
};

#endif // DEV_CTRLMPDU_H
