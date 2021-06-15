#ifndef AD_CORETHREAD_H
#define AD_CORETHREAD_H

#include "ad_resulting.h"

class Ad_CoreThread : public BaseThread
{
    Q_OBJECT
    explicit Ad_CoreThread(QObject *parent = nullptr);
public:
    static Ad_CoreThread *bulid(QObject *parent = nullptr);
    bool verifyResult() {return mResult->resEnter();}
    bool startAdjust() {return workDown();}

protected:
    bool workDown();

private:
    Ad_Adjusting *mAdjust;
    Ad_Resulting *mResult;
    Ad_LedSi *mLedSi;
};

#endif // AD_CORETHREAD_H
