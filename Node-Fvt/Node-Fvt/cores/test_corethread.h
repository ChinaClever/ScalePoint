#ifndef TEST_CORETHREAD_H
#define TEST_CORETHREAD_H
#include "test_network.h"

class Test_CoreThread : public BaseThread
{
    Q_OBJECT
public:
    explicit Test_CoreThread(QObject *parent = nullptr);

protected:
    void run();
    bool initFun();
    void workDown();
    void workResult();

protected slots:
    void initFunSlot();

private:
    Test_NetWork *mNetWork;
    Test_SerialNumber *mSn;
};

#endif // TEST_CORETHREAD_H
