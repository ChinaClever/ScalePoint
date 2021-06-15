#ifndef DATATABLEWID_H
#define DATATABLEWID_H

#include "comtablewid.h"
#include "baseobject.h"

class Td_DataTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Td_DataTabWid(QWidget *parent = nullptr);

protected:
    void initWid();
    void appendItem(sObjData *unit);

signals:

protected slots:
    void timeoutDone();

private:
    sObjData *mData;
};

#endif // DATATABLEWID_H
