#ifndef DEV_DEFTYPE_H
#define DEV_DEFTYPE_H
#include "baseobject.h"

class Sn_DevType
{
    Sn_DevType();
public:
    static Sn_DevType *bulid();
    bool analysDevType(uint id);

protected:
    QString getDevStr(uint id);
    QString getMpdu(uint id);

    int getDevType(const QString &str);
    int getAcType(const QString &str);
    int getColMode(const QString &str);
    int getLineNum(const QString &str);
    int getSerie(const QString &str);

private:
    sDevType *mDt;
};

#endif // DEV_DEFTYPE_H
