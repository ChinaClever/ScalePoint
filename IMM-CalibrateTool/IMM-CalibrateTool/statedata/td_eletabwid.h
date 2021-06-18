#ifndef TD_ELETABWID_H
#define TD_ELETABWID_H

#include "td_powtabwid.h"

class Td_EleTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Td_EleTabWid(QWidget *parent = nullptr);

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

#endif // TD_ELETABWID_H
