#ifndef AD_RESULTING_H
#define AD_RESULTING_H

#include "ad_adjusting.h"
#include "sp_immrtu.h"

class Ad_Resulting : public BaseThread
{
    Q_OBJECT
    explicit Ad_Resulting(QObject *parent = nullptr);
public:
    static Ad_Resulting *bulid(QObject *parent = nullptr);

    bool resEnter();
    bool initRtuThread(){return mCollect->readPduData();}
protected:
    bool powErrRange(int exValue, int cur);
    bool curErrRange(int exValue, int cur);

    bool curRangeByID(int i, int exValue, int cnt);
    bool powRangeByID(int i, int exValue, int cnt);
    bool volErrRangeByID(int i);
    bool volErrRange();
    bool workDown(int exValue);

    bool eachCurEnter(int exValue);
    bool eachCurCheck(int k, int exValue);

    bool noLoadEnter();
    bool noLoadCurFun();
    bool noLoadCurCheck(int k, int cnt);
    bool powerOn();

private:
    SP_ImmRtu *mCollect;
    YC_StandSource *mSource;
};

#endif // AD_RESULTING_H
