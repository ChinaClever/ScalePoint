#include "test_bsthread.h"

Test_BsThread::Test_BsThread(QObject *parent) : BaseThread(parent)
{

}

Test_BsThread *Test_BsThread::bulid(QObject *parent)
{
    static Test_BsThread* sington = nullptr;
    if(sington == nullptr)
        sington = new Test_BsThread(parent);
    return sington;
}

void Test_BsThread::initFunSlot()
{
    mDev = SP_ImmRtu::bulid(this);
}


bool Test_BsThread::volErrRangeByID(int i)
{
    bool ret = true;
    sLineData *line = &(mData->lines[i]);
    int vol = line->vol_rms / COM_RATE_VOL;
    QString str = tr("L%1电压 期望值220V，实际电压%2V ").arg(i+1).arg(vol);
    if((vol >= 170) && (vol <= 280)) {
        str += tr("正常"); updatePro(str);
        line->status = Test_Pass;
    } else {
        ret = false;
    }

    return ret;
}


bool Test_BsThread::volErrRange()
{
    bool ret = true; int k = 0;
    for(int i=0; i<mData->size; ++i) {
        ret = volErrRangeByID(i);
        if(!ret) {
            if(k++ < 5){
                i = -1; delay(2);
                mDev->readPduData();
            } else {
                ret = false;
                mData->lines[i].status = Test_Fail;
                QString str = tr("L%1电压 检测到错误 电压=%2V").arg(i+1)
                        .arg(mData->lines[i].vol_rms/ COM_RATE_VOL);
                updatePro(str, ret, 1); break;
            }
        }
    }

    return ret;
}


bool Test_BsThread::curErrRange(int cur)
{
    bool ret = false;
    if(cur > 0) ret =  true;
    return ret;
}

bool Test_BsThread::curRangeByID(int i,int cnt)
{
    sLineData *line = &(mData->lines[i]); int cur = line->cur_rms / 10;
    QString str = tr("L%1电流 实际电流%2A ").arg(i+1).arg(line->cur_rms/COM_RATE_CUR);
    bool ret = curErrRange(cur);
    if(ret) {
        line->cur_ed = line->cur_rms;
        if(ret){str += tr("正常"); updatePro(str);}
    } else {
        if(cnt > 3) {
            str += tr("错误");
            updatePro(str, ret);
            line->status = Test_Fail;
        }
    }

    return ret;
}


bool Test_BsThread::eachCurCheck(int k)
{
    bool ret = true;
    QString str = tr("L%1电流校验").arg(k+1);
    updatePro(str); for(int i=0; i<5; ++i) {
        if(i) str += tr(" 第%1次").arg(i+1); else delay(4);
        ret = curRangeByID(k, i);
        if(ret) break; else if(!delay(i+5)) break;
        mDev->readPduData();
    }

    return ret;
}

bool Test_BsThread::eachCurEnter()
{
    bool res = mDev->readPduData();
    for(int k=0; k<mData->size; ++k) {
        bool ret = eachCurCheck(k);
        if(!ret) res = false;
    }

    return res;
}

bool Test_BsThread::loopCurCheck()
{
    bool res = true;
    for(int i=0; i<mDt->outputs; ++i) {
        sBranchIt *it = &(mData->branchs[i]); int cur = it->cur_rms / 10;
        QString str = tr("C%1电流 电流%2A ").arg(i+1).arg(it->cur_rms/COM_RATE_CUR);
        bool ret = curErrRange(cur);
        if(ret) {str += tr("正常"); ret = true;}
        else {str += tr("错误").arg(cur/COM_RATE_CUR); res = false;}
        updatePro(str, ret);
    }

    return res;
}

bool Test_BsThread::neutralCheck()
{
    bool ret = true;
    if(mDt->neutral) {
        sBranchIt *it = &(mData->neutral); int cur = it->cur_rms / 10;
        QString str = tr("零线电流%1A ").arg(it->cur_rms/COM_RATE_CUR);
        for(int i=0; i<3; ++i) {
            ret = curErrRange(cur);
            if(ret) break; else mDev->readPduData();
        }
        if(ret) str += tr("正常"); else str += tr("错误");
        updatePro(str, ret);
    }

    return ret;
}


bool Test_BsThread::workDown()
{
    mDev->readPduData();
    bool ret = volErrRange();
    if(ret) ret = eachCurEnter();
    if(ret) ret = neutralCheck();
    if(ret) ret = loopCurCheck();
    return ret;
}


