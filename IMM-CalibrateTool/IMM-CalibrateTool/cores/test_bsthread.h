#ifndef TEST_BSTHREAD_H
#define TEST_BSTHREAD_H

#include "test_execute.h"

class Test_BsThread : public BaseThread
{
    Q_OBJECT
    explicit Test_BsThread(QObject *parent = nullptr);
public:
    static Test_BsThread *bulid(QObject *parent = nullptr);
    bool workDown();

protected slots:
    void initFunSlot();

private:
    bool volErrRangeByID(int i);
    bool volErrRange();

    bool curErrRange(int cur);
    bool curRangeByID(int i, int cnt);
    bool eachCurCheck(int k);
    bool eachCurEnter();

    bool loopCurCheck();
    bool neutralCheck();

private:
     SP_ImmRtu *mDev;

};

#endif // TEST_BSTHREAD_H
