#ifndef HOME_ENVTABWID_H
#define HOME_ENVTABWID_H

#include "td_thresholdtabwid.h"

class Td_EnvTabWid : public ComTableWid
{
    Q_OBJECT
public:
    explicit Td_EnvTabWid(QWidget *parent = nullptr);

signals:

protected:
    void initWid();
    void setDataUnit(int id, sDataUnit &unit);
    void setHum(sDataUnit &unit);
    void appendItem();

protected slots:
    void timeoutDone();

private:
    sObjData *mEnv;
};

#endif // HOME_ENVTABWID_H
