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

private:
    Test_Execute *mExe;
};

#endif // TEST_BASEFVT_H
