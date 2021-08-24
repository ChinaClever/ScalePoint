#ifndef PROGRAMWID_H
#define PROGRAMWID_H

#include <QWidget>
#include "programthread.h"

namespace Ui {
class ProgramWid;
}

class ProgramWid : public QWidget
{
    Q_OBJECT

public:
    explicit ProgramWid(int id, QWidget *parent = nullptr);
    ~ProgramWid();

protected:
    void initWid();
    bool connected();
    bool isFileExist();
    void firmwareBurn();

protected slots:
    void updateProSlot(int, int);
    void endFunSlot(bool res);
    void timeoutDone();

private slots:
    void on_startBtn_clicked();

private:
    Ui::ProgramWid *ui;

    int mId;
    bool isRun;
    int mCount;
    QTimer *timer;
    ProgramThread *mThread;
};

#endif // PROGRAMWID_H
