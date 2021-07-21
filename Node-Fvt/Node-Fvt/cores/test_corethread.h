#ifndef TEST_CORETHREAD_H
#define TEST_CORETHREAD_H
#include "baselogs.h"

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
    void workResult();

protected slots:
    void initFunSlot();

private:


};

#endif // TEST_CORETHREAD_H
