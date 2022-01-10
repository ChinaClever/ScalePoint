#ifndef TEST_CORETHREAD_H
#define TEST_CORETHREAD_H
#include "test_execute.h"
#include "bt_serial.h"
#include "pdu_zrtu.h"

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
    void outputCtrl();

    bool zeroMeasRot();
    void workDown();
    void workResult();

    bool printer();
    bool noCurLoad();
    bool hasCurLoad();
    bool relayCheck(int id);
    bool btCurCheck();
    bool outputCheck();

protected slots:
    void initFunSlot();

private:
    Test_Execute *mExe;
    SP_SocketRtu *mRtu;
    Bt_Serial *mBt;
    Pdu_ZRtu *mPdu;
    bool mPr;
};

#endif // TEST_CORETHREAD_H
