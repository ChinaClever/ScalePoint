#ifndef HOME_ENVTABWID_H
#define HOME_ENVTABWID_H

#include "td_branchtabwid.h"

class Td_AngleTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Td_AngleTabWid(QWidget *parent = nullptr);

signals:

protected:
    void initWid();
    void appendItem();

protected slots:
    void timeoutDone();

private:
    sDevObj *mData;
};

#endif // HOME_ENVTABWID_H
