#ifndef TEST_CORETHREAD_H
#define TEST_CORETHREAD_H
#include "test_execute.h"

class Test_CoreThread : public BaseThread
{
    Q_OBJECT
public:
    explicit Test_CoreThread(QObject *parent = nullptr);

protected:
    void run();
    bool initFun();
    bool readDev();
    bool enumDeviceType();

    void workDown();
    void collectData();
    void workResult();

protected slots:
    void initFunSlot();

private:
    Yc_Obj *mYc;
    SP_ImmRtu *mDev;
    Sn_SerialNum *mSn;
    Ad_CoreThread *mAd;
    Test_Execute *mExe;
};

#endif // TEST_CORETHREAD_H
