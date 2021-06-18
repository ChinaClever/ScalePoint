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
    QStringList getItem(sLineData &line);
    void appendItem(sDevObj *obj);

signals:

protected slots:
    void timeoutDone();

private:
    sDevObj *mData;
};

#endif // DATATABLEWID_H
