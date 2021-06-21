#ifndef TEST_CORETHREAD_H
#define TEST_CORETHREAD_H
#include "yc_obj.h"
#include "sn_serialnum.h"
#include "ad_corethread.h"

class Test_CoreThread : public BaseThread
{
    Q_OBJECT
public:
    explicit Test_CoreThread(QObject *parent = nullptr);

protected:
    void run();
    bool initFun();
    bool readDev();

    void workDown();
    void collectData();
    void workResult();

protected slots:
    void initFunSlot();

private:
    Yc_Obj *mYc;
    Sn_SerialNum *mSn;
    Ad_CoreThread *mAd;
};

#endif // TEST_CORETHREAD_H
