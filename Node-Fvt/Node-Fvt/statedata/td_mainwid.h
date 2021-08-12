#ifndef TD_MAINWID_H
#define TD_MAINWID_H

#include "baseobject.h"

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

public slots:
    void startSlot();
    void initFunSlot();
    void timeoutDone();
    void insertSlot(QString str);

private:
    Ui::Td_MainWid *ui;
    SerialPort *mSerial;
    QTimer *timer;
};

#endif // TD_MAINWID_H
