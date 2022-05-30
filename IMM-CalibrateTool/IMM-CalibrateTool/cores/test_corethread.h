#ifndef TEST_CORETHREAD_H
#define TEST_CORETHREAD_H
#include "test_bsthread.h"
#include "sp_socketrtu.h"
#include "bt_serial.h"

class Test_CoreThread : public BaseThread
{
    Q_OBJECT
public:
    explicit Test_CoreThread(QObject *parent = nullptr);

protected:
    void run();
    bool resDev();
    bool initFun();
    bool readDev();
    bool printer();

    bool cylinderDown();
    bool enumDeviceType();

    void workDown();
    void collectData();
    void workResult();
    bool btCurCheck();

protected slots:
    void initFunSlot();

private:
    SP_SocketRtu *mSocket;
    Ad_CoreThread *mAd;
    Test_Execute *mExe;
    SP_ImmRtu *mDev;
    Yc_Obj *mYc;
    bool mPr;
    Bt_Serial *mBt;
};

#endif // TEST_CORETHREAD_H
