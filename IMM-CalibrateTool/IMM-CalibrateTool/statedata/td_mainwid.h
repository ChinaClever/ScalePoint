#ifndef TD_MAINWID_H
#define TD_MAINWID_H

#include "td_envtabwid.h"

namespace Ui {
class Td_MainWid;
}

class Td_MainWid : public QWidget
{
    Q_OBJECT

public:
    explicit Td_MainWid(QWidget *parent = nullptr);
    ~Td_MainWid();

protected:
    void initWid();

private:
    Ui::Td_MainWid *ui;
    Td_DataTabWid *mTableWid;
    Td_EnvTabWid *mEnvTabWid;
    Td_LineTabWid *mLineTabWid;
    Td_ThresholdTabWid *mThTabWid;
};

#endif // TD_MAINWID_H
