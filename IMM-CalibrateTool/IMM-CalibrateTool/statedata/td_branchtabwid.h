#ifndef HOME_THRESHOLDTABWID_H
#define HOME_THRESHOLDTABWID_H

#include "td_eletabwid.h"

class Td_BranchTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Td_BranchTabWid(QWidget *parent = nullptr);

signals:

protected:
    void initWid();
    void getItem(int id, sBranchIt &it);
    void appendItem(sDevObj *dev);

protected slots:
    void timeoutDone();

private:
    sDevObj *mData;
};

#endif // HOME_THRESHOLDTABWID_H
