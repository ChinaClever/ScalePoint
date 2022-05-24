#ifndef TEST_CORETHREAD_H
#define TEST_CORETHREAD_H
#include "test_network.h"

class Test_CoreThread : public BaseThread
{
    Q_OBJECT
public:
    explicit Test_CoreThread(QObject *parent = nullptr);

    bool isContinue;

signals:
    void waitSig();

protected:
    void run();
    bool initFun();
    bool programFab();
    bool macSnCheck();
    void workDown();
    void workResult();
    bool waitFor();
    bool printer();
    void testNode();

protected slots:
    void initFunSlot();

private:
    Test_NetWork *mNetWork;
    Test_SerialNumber *mSn;
    test_FabPartition *mFab;
    int mSelect;
};

#endif // TEST_CORETHREAD_H
