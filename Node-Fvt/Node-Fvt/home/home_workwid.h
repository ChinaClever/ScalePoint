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
    void insertText();
    void setTextColor();

    QString getTime();
    void updateWid();
    void updateTime();
    void updateResult();
    void setWidEnabled(bool en);

    bool initSerial();
    bool initWid();
    void initUser();
    void initData();

private slots:
    void timeoutDone();
    void initFunSlot();
    void waitForSlot();
    void updateCntSlot();
    void on_startBtn_clicked();
    void on_adCheckBox_clicked(bool checked);

    void on_checkBox_clicked(bool checked);

    void on_imgBtn_clicked();

private:
    Ui::Home_WorkWid *ui;

    int mId;
    bool isCheck;
    QTimer *timer;
    Test_CoreThread *mCoreThread;
};

#endif // HOME_WORKWID_H
