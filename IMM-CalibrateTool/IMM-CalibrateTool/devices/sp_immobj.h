#ifndef SP_IMMOBJ_H
#define SP_IMMOBJ_H

#include "sp_object.h"

class SP_ImmObj : public SP_Object
{
    Q_OBJECT
public:
    explicit SP_ImmObj(QObject *parent = nullptr);

protected:
    uchar *toInt(uchar *ptr, uint &value);
    uchar *toShort(uchar *ptr, ushort &value);

    uchar *toItem(uchar *ptr, sItData &it);
    uchar *toLine(uchar *ptr, sLineData &line);
    uchar *getLines(uchar *ptr, sDevObj *obj);
    uchar *toAngles(uchar *ptr, sDevObj *obj);
    uchar *toBranchIt(uchar *ptr, sBranchIt &it);
    uchar *getBranchs(uchar *ptr, sDevObj *obj);
};

#endif // SP_IMMOBJ_H
