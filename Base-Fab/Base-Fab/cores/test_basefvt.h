#ifndef TEST_BASEFVT_H
#define TEST_BASEFVT_H

#include "test_execute.h"

class Test_BaseFvt : public BaseThread
{
    Q_OBJECT
    explicit Test_BaseFvt(QObject *parent = nullptr);
public:
    static Test_BaseFvt *bulid(QObject *parent = nullptr);
    bool workDown();
    bool getToken(int index);
    QString reverse(QString str);

signals:
    void closeSig();
    void openSig(QString str , int baud);

private:
    bool isFileExist(const QString &fn);
    bool relayControl(int id, int oc);
    bool relayControl(int id);
    bool zigbeeConnect();
    void mdelay(int ms);
    bool inputCheck();
    bool rsConnect();
    bool execute(QString *ptr);

    bool updateData(const QString &v);
    bool zigbeeCheck(const QString &v);

    inline bool startTest(RtuRw * ser , QString str, int i);
    inline bool disableTest(RtuRw * ser ,QString str);
    inline QString transmit(RtuRw *ser, QString str);
    inline bool getFw(RtuRw *ser, QString str);
    inline bool getIc(RtuRw *ser, QString str);
    inline bool getIeee(RtuRw * ser ,QString str);
    inline bool fullReset(RtuRw * ser ,QString str);

private:
    Test_Execute *mExe;
    QString mPinsName;
    QString mComStr;
};

#endif // TEST_BASEFVT_H
