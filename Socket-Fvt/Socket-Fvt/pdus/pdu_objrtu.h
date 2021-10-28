#ifndef DEV_RTUOBJ_H
#define DEV_RTUOBJ_H

#include "pdu_object.h"


class Pdu_ObjRtu : public Pdu_Object
{
    Q_OBJECT
public:
    explicit Pdu_ObjRtu(QObject *parent = nullptr);
    virtual bool readPduData()=0;

protected:
    uchar *toInt(uchar *ptr, int line, uint *value);
    uchar *toChar(uchar *ptr, int line, uchar *value);
    uchar *toShort(uchar *ptr, int line, ushort *value);

    ushort getShort(uchar *ptr);
    uchar *getShort(uchar *ptr, int line, uchar *value);
    uchar *toOutputEle(uchar *ptr, int line, uint *value);
    uchar *toThreshold(uchar *ptr, int line, sUnitTh &unit);
    uchar *toEnvTh(uchar *ptr, sUnitTh &unit);
};

#endif // DEV_RTUOBJ_H
