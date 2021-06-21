/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_corethread.h"

Ad_CoreThread::Ad_CoreThread(QObject *parent) : BaseThread(parent)
{
    mAdjust = Ad_Adjusting::bulid(this);
    mResult = Ad_Resulting::bulid(this);
}

Ad_CoreThread *Ad_CoreThread::bulid(QObject *parent)
{
    static Ad_CoreThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_CoreThread(parent);
    return sington;
}

bool Ad_CoreThread::workDown()
{
    bool ret = mAdjust->startAdjust();
    if(mPro->step == Test_vert) {
        ret = mResult->resEnter();
    } else if(mPro->step == Test_Over) {
        ret = mResult->initRtuThread();
    }

    return ret;
}



