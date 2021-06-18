#ifndef HOME_LINETABWID_H
#define HOME_LINETABWID_H

#include "td_datatabwid.h"

class Td_PowTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Td_PowTabWid(QWidget *parent = nullptr);

signals:

protected:
    void initWid();
    QStringList getItem(sLineData &line);
    void appendItem(sDevObj *dev);

protected slots:
    void timeoutDone();

private:
    sDevObj *mData;
};

#endif // HOME_LINETABWID_H
