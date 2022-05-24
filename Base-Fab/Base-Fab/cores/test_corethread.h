#ifndef TEST_CORETHREAD_H
#define TEST_CORETHREAD_H
#include "test_basefvt.h"
#include "basetokens.h"

class Test_CoreThread : public BaseThread
{
    Q_OBJECT
public:
    explicit Test_CoreThread(QObject *parent = nullptr);

protected:
    void run();
    bool fabFile();
    bool fabTokens();
    bool fabBootloader();
    bool workDown();
    bool factoryWork();
    void workResult(bool ret);
    bool printer();

protected slots:
    void initFunSlot();

private:
    Test_BaseFvt *mFvt;
    Test_Execute *mExe;
    BaseTokens *mTokens;
};

#endif // TEST_CORETHREAD_H
