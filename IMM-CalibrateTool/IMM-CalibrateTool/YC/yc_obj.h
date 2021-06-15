#ifndef TEST_YC_H
#define TEST_YC_H

#include "yc_dc107.h"

class Yc_Obj : public BaseThread
{
    Q_OBJECT
    explicit Yc_Obj(QObject *parent = nullptr);
public:
    static Yc_Obj *bulid(QObject *parent = nullptr);
    YC_StandSource *get();

    bool powerOn();
    bool powerDown();

    bool setCur(int v, int sec=0);
    bool setVol(int v, int sec=0);

private:
    YC_Ac92b *mAc;
    YC_Dc107 *mDc;
};

#endif // TEST_YC_H
