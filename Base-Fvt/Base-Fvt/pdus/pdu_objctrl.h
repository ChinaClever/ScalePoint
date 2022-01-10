#ifndef DEV_CTRLOBJ_H
#define DEV_CTRLOBJ_H

#include "pdu_objrtu.h"

class Pdu_ObjCtrl : public Pdu_ObjRtu
{
    Q_OBJECT
public:
    explicit Pdu_ObjCtrl(QObject *parent = nullptr);

    virtual void openAllSwitch();
    virtual void closeAllSwitch();
    virtual void openOutputSwitch(int id);
    virtual void closeOutputSwitch(int id);
    virtual void openOnlySwSlot(int id);
    virtual void closeOtherSwitch(int id);
    virtual void openOnlySwitch(int id);

    virtual void clearAllEle();
    virtual void setClearEle(int id);
    virtual bool factorySet(){return true;}

protected:
    void setBitControl(int id, uchar *buf);
    // bool delay(int s) {return  delay(s);}

    virtual void funClearEle(uchar *buf)=0;
    virtual void funSwitch(uchar *on, uchar *off)=0;

protected slots:
    void closeOtherSwitchSlot();

protected:
    int mIdClosed;

};

#endif // DEV_CTRLOBJ_H
