#ifndef TD_MAINWID_H
#define TD_MAINWID_H

#include<QWidget>

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
};

#endif // TD_MAINWID_H
