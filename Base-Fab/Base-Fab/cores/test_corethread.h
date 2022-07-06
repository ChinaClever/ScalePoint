#ifndef TEST_CORETHREAD_H
#define TEST_CORETHREAD_H
#include "test_basefvt.h"
#include "basetokens.h"

class Test_CoreThread : public BaseThread
{
    Q_OBJECT
public:
    explicit Test_CoreThread(QObject *parent = nullptr);
    bool isContinue;

signals:
    void waitSig();
    void closeSig();
    void openSig(QString str ,int baud);

protected:
    void run();
    bool fabFile();
    bool fabTokens();
    bool fabBootloader();
    bool workDown();
    bool factoryWork();
    void workResult(bool ret);
    bool printer();
    bool test(RtuRw *modbus);
    bool test485();
    bool waitFor();

protected slots:
    void initFunSlot();

private:
    Test_BaseFvt *mFvt;
    Test_Execute *mExe;
    BaseTokens *mTokens;
    RtuRw *mModbusJ7;
    RtuRw *mModbusJ8;
};

#endif // TEST_CORETHREAD_H
