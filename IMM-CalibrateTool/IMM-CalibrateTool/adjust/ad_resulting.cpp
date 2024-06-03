/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_resulting.h"
#define AD_CUR_RATE 100

Ad_Resulting::Ad_Resulting(QObject *parent) : BaseThread(parent)
{
    mCollect = SP_ImmRtu::bulid(this);
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
    int min = exValue - mItem->errs.curErr * 10;//400-1
    int max = exValue + mItem->errs.curErr * 10;//400+1
    if((cur >= min) && (cur <= max )) ret =  true;
    return ret;
}

bool Ad_Resulting::powErrRange(int exValue, int pow)
{
    bool ret = false; pow /= COM_RATE_POW;
    int err = exValue * mItem->errs.powErr/1000.0;
    int min = exValue - err;
    int max = exValue + err;
    if((pow >= min) && (pow <= max)) ret =  true;
    else qDebug() << "pow Err Range" << pow << exValue << err;

    return ret;
}

bool Ad_Resulting::powRangeByID(int i, int exValue, int cnt)
{
    exValue = mItem->errs.vol * exValue/AD_CUR_RATE; exValue *= 0.5;
    QString str = tr("L%1功率 期望值 %2W 实际功率").arg(i+1).arg(exValue);
    sLineData *line = &(mData->lines[i]);
    bool ret = powErrRange(exValue, line->pow.active);
    int a = exValue*COM_RATE_POW;
    int b = line->pow.active;
    float c = -1;
    if(a != 0)c = ((abs(a-b)*1.0)/a)*100.0;
    if(ret) {
        line->status = Test_Pass;
        line->powed = line->pow.active;
        str += tr(" %1W 误差=%2 % 正常").arg(QString::number(line->pow.active/COM_RATE_POW,'f',3)).arg(QString::number(c,'f',3));
        updatePro(str);
    } else {
        if(cnt > 3) {
            str += tr(" %1W 误差=%2 % 错误").arg(QString::number(line->pow.active/COM_RATE_POW,'f',3)).arg(QString::number(c,'f',3));
            updatePro(str, ret); line->status = Test_Fail;
        }
    }

    return ret;
}

bool Ad_Resulting::curRangeByID(int i, int exValue, int cnt)
{
    sLineData *line = &(mData->lines[i]); int cur = line->cur_rms / 10;
    int a = exValue*COM_RATE_CUR/AD_CUR_RATE;
    int b = line->cur_rms;
    float c = -1;
    if(a != 0)c = ((abs(a-b)*1.0)/a)*100.0;
    QString str1;
    QString str = tr("L%1电流 期望值%2A，实际电流%3A 误差=%4 %").arg(i+1)
            .arg(exValue/AD_CUR_RATE).arg(line->cur_rms/COM_RATE_CUR).arg(a==0?"---":QString::number(c,'f',3));
    bool ret = curErrRange(exValue, cur);
    if(ret) {
        line->cur_ed = line->cur_rms;
        ret = powRangeByID(i, exValue, cnt);
        if(ret){
            str += tr("正常");
            str1 = tr("L%1:Current Expect:%2A Current:%3A Error rate:%4 Pass").arg(i+1)
                    .arg(exValue/AD_CUR_RATE).arg(line->cur_rms/COM_RATE_CUR).arg(a==0?"---":QString::number(c,'f',3));
            mLog<<str1;
            updatePro(str);
        }
    } else {
        if(cnt > 3) {
            str += tr("错误");
            str1 = tr("L%1:Current Expect:%2A Current:%3A Error rate:%4 Fail").arg(i+1)
                    .arg(exValue/AD_CUR_RATE).arg(line->cur_rms/COM_RATE_CUR).arg(a==0?"---":QString::number(c,'f',3));
            mLog<<str1;
            updatePro(str, ret);
            line->status = Test_Fail;
        }
    }

    return ret;
}

bool Ad_Resulting::volErrRangeByID(int i)
{
    bool ret = true;
    sErrData *errs = &(mItem->errs);
    sLineData *line = &(mData->lines[i]);
    int vol = line->vol_rms / COM_RATE_VOL;
    int min = errs->vol - errs->volErr;
    int max = errs->vol + errs->volErr;

    int a = errs->vol*COM_RATE_VOL;
    int b = line->cur_rms;
    float c = -1;
    if(a != 0)c = ((abs(a-b)*1.0)/a)*100.0;
    QString str = tr("L%1电压 期望值200V，实际电压%2V 误差=%3 %").arg(i+1).arg(vol).arg(a==0?"---":QString::number(c,'f',3));
    if((vol >= min) && (vol <= max)) {
        str += tr("正常"); updatePro(str);
        mLog<<str;
        line->status = Test_Pass;
    } else {
        ret = false;
    }

    return ret;
}

bool Ad_Resulting::volErrRange()
{    
    bool ret = true; int k = 0;
    for(int i=0; i<mData->size; ++i) {
        ret = volErrRangeByID(i);
        if(!ret) {
            if(k++ < 5){
                i = -1; delay(2);
                mCollect->readPduData();
            } else {
                ret = false;
                mData->lines[i].status = Test_Fail;

                sErrData *errs = &(mItem->errs);
                sLineData *line = &(mData->lines[i]);
                int a = errs->vol*COM_RATE_VOL;
                int b = line->cur_rms;
                float c = -1;
                if(a != 0)c = ((abs(a-b)*1.0)/a)*100.0;
                QString str1 = tr("L%1电压 期望值200V，实际电压%2V 误差=%3 %").arg(i+1).arg(line->vol_rms / COM_RATE_VOL).arg(a==0?"---":QString::number(c,'f',3));

                updatePro(str1, ret, 1); break;
            }
        }
    }

    return ret;
}

bool Ad_Resulting::eachCurCheck(int k, int exValue)
{
    bool ret = true;
    double value = mItem->errs.vol*exValue/AD_CUR_RATE; value *= 0.5;
    QString str = tr("L%1校验数据: 期望电流%2A 功率%3W").arg(k+1).arg(exValue/AD_CUR_RATE).arg(value);
    updatePro(str); for(int i=0; i<5; ++i) {
        if(i) str += tr(" 第%1次").arg(i+1); else delay(4);
        ret = curRangeByID(k, exValue, i);
        if(ret) break; else if(!delay(i+5)) break;
        mCollect->readPduData();
    }

    return ret;
}

bool Ad_Resulting::eachCurEnter(int exValue)
{
    bool res = mCollect->readPduData();
    for(int k=0; k<mData->size; ++k) {
        bool ret = eachCurCheck(k, exValue);
        if(!ret) res = false;
    }

    return res;
}

bool Ad_Resulting::loopCurCheck(int exValue)
{
    bool res = true;
    for(int i=0; i<mDt->outputs; ++i) {
        sBranchIt *it = &(mData->branchs[i]); int cur = it->cur_rms / 10;
        int a = exValue*COM_RATE_CUR/AD_CUR_RATE;
        int b = it->cur_rms;
        float c = -1;
        if(a != 0)c = ((abs(a-b)*1.0)/a)*100.0;
        QString str = tr("C%1电流 期望值%2A, 实际电流%3A 误差=%4 %").arg(i+1)
                .arg(exValue/AD_CUR_RATE).arg(it->cur_rms/COM_RATE_CUR).arg(a==0?"---":QString::number(c,'f',3));
        QString str1;
        int err = (mItem->errs.curErr) * 10; bool ret = false;
        int min = exValue - err; int max = exValue + err;
        if((cur >= min) && (cur <= max )) {
            str += tr("正常");
            ret = true;
            str1 = tr("C%1:Current Expect:%2A Current:%3A Error rate:%4 Pass").arg(i+1)
                                        .arg(exValue/AD_CUR_RATE).arg(it->cur_rms/COM_RATE_CUR).arg(a==0?"---":QString::number(c,'f',3));
        }
        else {
            str += tr("错误").arg(cur/COM_RATE_CUR);
            res = false;
            str1 = tr("C%1:Current Expect:%2A Current:%3A Error rate:%4 Fail").arg(i+1)
                                        .arg(exValue/AD_CUR_RATE).arg(it->cur_rms/COM_RATE_CUR).arg(a==0?"---":QString::number(c,'f',3));
        }
        mLog<<str1;
        updatePro(str, ret);
    }

    return res;
}

bool Ad_Resulting::neutralCheck(int exValue)
{
    bool ret = true;
    if(mDt->neutral) {
        sBranchIt *it = &(mData->neutral); int cur = it->cur_rms / 10;
        int a = exValue*COM_RATE_CUR/AD_CUR_RATE;
        int b = it->cur_rms;
        float c = -1;
        if(a != 0)c = ((abs(a-b)*1.0)/a)*100.0;
        QString str = tr("期望值%1A 实际零线电流%2A 误差=%3 %")
                .arg(exValue/AD_CUR_RATE).arg(it->cur_rms/COM_RATE_CUR).arg(a==0?"---":QString::number(c,'f',3));
        for(int i=0; i<3; ++i) {
            ret = curErrRange(exValue, cur);
            if(ret) break; else mCollect->readPduData();
        }
        if(ret) str += tr("正常"); else str += tr("错误");
        updatePro(str, ret);
    }

    return ret;
}

bool Ad_Resulting::workDown(int exValue)
{
    bool ret = eachCurEnter(exValue);
    //if(ret) ret = neutralCheck(exValue);
    if(ret) ret = loopCurCheck(exValue);
    return ret;
}

bool Ad_Resulting::noLoadCurCheck(int k, int cnt)
{
    bool res = true;
    sLineData *line = &(mData->lines[k]);
    line->powed = line->pow.active;
    line->cur_ed = line->cur_rms;

    ushort pow = line->pow.active / 10;
    ushort cur = line->cur_rms / 10;
    QString str = tr("L%1相 空载校验 ").arg(k+1);
    if(cur || pow) {
        res = false;
        if(cnt > 3) {
            if(cur) str += tr("电流有底数");
            if(pow) str += tr("功率有底数");
            updatePro(str, res); line->status = Test_Fail;
        }
    } else {
        line->status = Test_Pass;
        str += tr("通过"); updatePro(str);
    }

    return res;
}

bool Ad_Resulting::noLoadCurFun()
{
    bool ret = true;
    for(int k=0; k<mData->size; ++k) {
        for(int i=0; i<5; ++i) {
            mCollect->readPduData(); ret = noLoadCurCheck(k, i);
            if(ret) break; else if(!delay(i+4)) break;
            // if(i) updatePro(tr("L%1相 空载校验: 第%2次检查").arg(k+1).arg(i+1));
        }
    }

    if(ret && mDt->neutral) {
        QString str = tr("零线 空载校验 ");
        int v = mData->neutral.cur_rms; if(!v) str += str += tr("通过");
        else {str += tr("电流有底数 %1A").arg(v/COM_RATE_CUR); ret = false;}
        updatePro(str, ret);
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
    mSource = Yc_Obj::bulid()->get();
    updatePro(tr("自动验证开始"));

    mPro->step = Test_vert;
    mSource->setVol(200);
    bool ret = mSource->setCur(40, 2);
    QString str = tr("验证电流：期望电流4A");
    return updatePro(str, ret);
}

bool Ad_Resulting::resEnter()
{
    mLog.clear();
    QString str = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    mLog<<str;
    mLog<<mDt->pn;
    mLog<<mDt->sn;
    mLog<<mDt->fw;

    bool ret = powerOn();
    if(ret) {
        ret = workDown(4*AD_CUR_RATE);
        writeLog();
        if(ret) ret = noLoadEnter();
    }

    return ret;
}

void Ad_Resulting::writeLog()
{
    QString tfile = QCoreApplication::applicationDirPath();
    tfile.remove(tfile.length()-7 , 7);
    QString fileName = tfile+"Log_imm.csv";
    QFile csvfile(fileName);
    csvfile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);

    QTextStream out(&csvfile);
    out.setCodec("GBK");
    for(int i=0; i<mLog.size(); ++i)
    {
        out<<mLog.at(i)<<",";
        msleep(30);
    }
    out<<"\n";
    csvfile.flush();
    csvfile.close();
    mLog.clear();
}


