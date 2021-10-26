#ifndef DEV_OBJECT_H
#define DEV_OBJECT_H

#include "baseobject.h"

class Dev_Object : public BaseThread
{
    Q_OBJECT
public:
    explicit Dev_Object(QObject *parent = nullptr);
    sDevData *getDev() {return mDev;}

protected:
    uchar *toInt(uchar *ptr, uint &value);
    uchar *toShort(uchar *ptr, ushort &value);
    void reflush();
};

#endif // DEV_OBJECT_H
