/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_resulting.h"
#define AD_CUR_RATE 100

Ad_Resulting::Ad_Resulting(QObject *parent) : BaseThread(parent)
{
}

Ad_Resulting *Ad_Resulting::bulid(QObject *parent)
{
    static Ad_Resulting* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_Resulting(parent);
    return sington;
}

bool Ad_Resulting::curErrRange(int exValue, int cur)
{
    bool ret = false;
    int min = exValue - mItem->curErr * 10;
    int max = exValue + mItem->curErr * 10;

    if((cur >= min) && (cur <= max )) {
        ret =  true;
    }

    return ret;
}

bool Ad_Resulting::powErrRange(int exValue, int pow)
{
    bool ret = false;
    int err = exValue * mItem->powErr/1000.0;
    int min = exValue - err;
    int max = exValue + err;

    if((pow >= min) && (pow <= max )) {
        ret =  true;
    } else {
        qDebug() << "pow Err Range" << pow << exValue << err;
    }

    return ret;
}

bool Ad_Resulting::powRangeByID(int i, int exValue, int cnt)
{
    exValue = mItem->vol * exValue/AD_CUR_RATE; //mData->cur.value[i]/COM_RATE_CUR;
    if(AC == mDt->ac) exValue *= 0.5;
    QString str = tr("期望功率%1kW 第%2位 功率 ").arg(exValue/1000.0).arg(i+1);
    bool ret = powErrRange(exValue, mData->pow[i]);
    mData->powed[i] = mData->pow[i];
    if(ret) {
        str += tr("正常");
        updatePro(str);
        mData->status[i] = Test_Pass;
    } else {
        ret = false;
        if(cnt > 3) {
            str += tr("错误");
            updatePro(str, ret, 1);
            mData->status[i] = Test_Fail;
        }
    }

    return ret;
}

bool Ad_Resulting::curRangeByID(int i, int exValue, int cnt)
{
    int cur = mData->cur.value[i] * 10;
    if(mDt->lines == 2 &&i == 0) exValue *= 2;
    QString str = tr("期望电流%1A 第%2位 电流 ").arg(exValue/AD_CUR_RATE).arg(i+1);
    bool ret = curErrRange(exValue, cur);
    mData->cured[i] = mData->cur.value[i];
    if(ret) {
        // if(mItem->si_led) return ret;   ////===========
        ret = powRangeByID(i, exValue, cnt);
        if(ret){str += tr("正常"); updatePro(str);}
    } else {
        ret = false;
        if(cnt > 3) {
            str += tr("错误");
            updatePro(str, ret, 0);
            mData->status[i] = Test_Fail;
        }
    }

    return ret;
}

bool Ad_Resulting::volErrRangeByID(int i)
{
    bool ret = true;
    int vol = mData->vol.value[i];
    int min = mItem->vol - mItem->volErr;
    int max = mItem->vol + mItem->volErr;
    QString str = tr("期望电压200V，实际电压%1V 第%2位 电压 ").arg(vol).arg(i+1);
    if((vol >= min) && (vol <= max)) {
        str += tr("正常");
        updatePro(str);
        mData->status[i] = Test_Pass;
    } else {
        ret = false;
    }

    return ret;
}

bool Ad_Resulting::volErrRange()
{
    int k = 0;
    bool ret = mCollect->readPduData();
    for(int i=0; i<mData->size; ++i) {
        ret = volErrRangeByID(i);
        if(!ret) {
            if(k++ < 5){
                i = -1; delay(2);
                mCollect->readPduData();
            } else {
                ret = false;
                mData->status[i] = Test_Fail;
                QString str = tr("检测到电压 %1 错误").arg(i+1);
                updatePro(str, ret, 1); break;
            }
        }
    }

    return ret;
}

bool Ad_Resulting::eachCurCheck(int k, int exValue)
{
    bool ret = true;
    double value = mItem->vol*exValue/AD_CUR_RATE/1000.0;
    if(AC == mDt->ac) value = value*0.5;
    QString str = tr("校验数据: 期望电流%1A 功率%2kW").arg(exValue/AD_CUR_RATE).arg(value);
    updatePro(str);
    for(int i=0; i<5; ++i) {
        if(i) str += tr(" 第%1次").arg(i+1); //else delay(4);
        ret = curRangeByID(k, exValue, i);
        if(ret) break; else if(!delay(i+5)) break;
        mCollect->readPduData();
    }

    return ret;
}

bool Ad_Resulting::eachCurEnter(int exValue)
{
    bool res = true;
    mCollect->readPduData();
    for(int k=0; k<mData->size; ++k) {
        bool ret = eachCurCheck(k, exValue);
        if(!ret) res = false;
    }

    return res;
}

bool Ad_Resulting::initRtuThread()
{
    switch (mItem->modeId) {
    case SI_PDU: mCollect = Dev_SiRtu::bulid(this); break;
    case IP_PDU: mCollect = Dev_IpRtu::bulid(this); break;
    default: mCollect = nullptr; break;
    }

    return mCollect->readPduData();
}

bool Ad_Resulting::workDown(int exValue)
{
    return eachCurEnter(exValue);
}

bool Ad_Resulting::noLoadCurCheck(int cnt)
{
    bool res = true;
    for(int k=0; k<mData->size; ++k) {
        mData->powed[k] = mData->pow[k];
        mData->cured[k] = mData->cur.value[k];
        QString str = tr("空载校验: 第%1相 ").arg(k+1);
        if(mData->cur.value[k] || mData->pow[k]) {
            res = false;
            if(cnt > 3) {
                mData->status[k] = Test_Fail;
                if(mData->cur.value[k]) str += tr("电流有底数");
                if(mData->pow[k]) str += tr("功率有底数");
                updatePro(str, res, 1);
            }
        } else {
            mData->status[k] = Test_Pass;
            str += tr("通过");
            updatePro(str);
        }
    }

    return res;
}

bool Ad_Resulting::noLoadCurFun()
{
    bool ret = true;
    for(int i=0; i<5; ++i) {
        QString str = tr("空载校验: 第%1次检查").arg(i+1);
        if(i)updatePro(str, true, 5); else delay(6);
        mCollect->readPduData();
        ret = noLoadCurCheck(i);
        if(ret) break; else if(!delay(i+4)) break;
    }

    return ret;
}

bool Ad_Resulting::noLoadEnter()
{
    bool ret = mSource->setCur(0, 0);
    if(ret) ret = volErrRange();

    QString str = tr("空载验证：设置空载电流");
    updatePro(str, ret, 5);
    if(ret) ret = noLoadCurFun();
    return ret;
}

bool Ad_Resulting::powerOn()
{
    initRtuThread();
    updatePro(tr("自动验证开始"),true, 2);
    mSource = Yc_Obj::bulid()->get();
    if(mItem->modeId == SI_PDU) {
        updatePro(tr("等待设备重启"));
        mSource->setVol(0,3);
        //mSource->setCur(0,0);
    } else if(mDt->version == 1){
        if(mDt->lines == 3) {
            updatePro(tr("等待设备稳定"));
            mSource->setVol(200, 15);
        }
    }

    mPro->step = Test_vert;
    mSource->setVol(200, 1);
    bool ret = mSource->setCur(40, 5);
    QString str = tr("验证电流：期望电流4A");
    return updatePro(str, ret, 3);
}

bool Ad_Resulting::resEnter()
{
    bool ret = powerOn();
    if(ret) {
        ret = workDown(4*AD_CUR_RATE);
        if(ret) ret = noLoadEnter();
    }

    return ret;
}

