#ifndef HOME_THRESHOLDTABWID_H
#define HOME_THRESHOLDTABWID_H

#include "td_linetabwid.h"

class Td_ThresholdTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Td_ThresholdTabWid(QWidget *parent = nullptr);

signals:

protected:
    void initWid();
    void setDataUnit(int id, sDataUnit &unit);
    void appendItem(sObjData *dev);

protected slots:
    void timeoutDone();

private:
    sObjData *mData;
};

#endif // HOME_THRESHOLDTABWID_H
