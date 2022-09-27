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
    void sendIndexSig(int);

protected:
    void initLayout();
    void insertText();
    void setTextColor();

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
    bool initFilePath();
    QString selectFile(const QString &filter, QString dir);


private slots:
    void timeoutDone();
    void initFunSlot();
    void updateCntSlot();
    void on_startBtn_clicked();

    void on_proBtn_clicked();

    void on_srcBtn_clicked();

private:
    Ui::Home_WorkWid *ui;

    int mId;
    bool isCheck;
    QTimer *timer;
    Test_CoreThread *mCoreThread;
};

#endif // HOME_WORKWID_H
