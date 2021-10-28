#ifndef DEV_CTRLZPDU_H
#define DEV_CTRLZPDU_H

#include "pdu_mctrl.h"

class Pdu_ZCtrl : public Pdu_ObjCtrl
{
    Q_OBJECT
public:
    explicit Pdu_ZCtrl(QObject *parent = nullptr);
    bool factorySet();

protected:
    void funSwitch(uchar *on, uchar *off);
    void funClearEle(uchar *buf);
};

#endif // DEV_CTRLZPDU_H
