#ifndef DEV_IMMRTU_H
#define DEV_IMMRTU_H

#include "dev_object.h"

class Dev_ImmRtu : public Dev_Object
{
    Q_OBJECT
    explicit Dev_ImmRtu(QObject *parent = nullptr);

public:
    bool readPduData();
    static Dev_ImmRtu *bulid(QObject *parent = nullptr);

protected:
    uchar *toItem(uchar *ptr, sItData &it);
    uchar *toLine(uchar *ptr, sLineData &line);
    uchar *getLines(uchar *ptr, sDevObj *obj);
    uchar *toAngles(uchar *ptr, sDevObj *obj);
    uchar *toBranchIt(uchar *ptr, sBranchIt &it);
    uchar *getBranchs(uchar *ptr, sDevObj *obj);

    void getDevData(uchar *ptr, int lines, sDevObj *obj);
    void initRtuItem(sRtuItem &it);

    int recvLine(int len);
    bool recvPacket(uchar *buf, int len);
};

#endif // DEV_IMMRTU_H
