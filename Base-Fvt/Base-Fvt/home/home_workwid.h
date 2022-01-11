#ifndef HOME_WORKWID_H
#define HOME_WORKWID_H

#include <QWidget>
#include "td_mainwid.h"
#include "test_corethread.h"

namespace Ui {
class Home_WorkWid;
}

class Home_WorkWid : public QWidget, public BaseObject
{
    Q_OBJECT

public:
    explicit Home_WorkWid(QWidget *parent = nullptr);
    ~Home_WorkWid();

signals:
    void startSig();

protected:
    void initLayout();
    QString getTime();
    void updateWid();
    void updateTime();
    void updateResult();
    void setWidEnabled(bool en);

    bool inputCheck();
    bool initSerial();
    bool initWid();
    bool initUser();
    void initData();    
    QString selectFile(const QString &filter, QString dir);

private slots:
    void timeoutDone();
    void initFunSlot();
    void updateCntSlot();
    void on_cmdBtn_clicked();
    void on_dlBtn_clicked();
    void on_fnBtn_clicked();
    void on_startBtn_clicked();

private:
    Ui::Home_WorkWid *ui;

    QTimer *timer;
    Test_CoreThread *mCoreThread;
};

#endif // HOME_WORKWID_H
