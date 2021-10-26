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
    void outputCtrl();

    void workDown();
    void workResult();

protected slots:
    void initFunSlot();

private:
    Test_Execute *mExe;
    Dev_SocketRtu *mRtu;
};

#endif // TEST_CORETHREAD_H
