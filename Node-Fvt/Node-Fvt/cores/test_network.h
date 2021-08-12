#ifndef TEST_NETWORK_H
#define TEST_NETWORK_H
#include "test_serialnumber.h"
#include "udprecvsocket.h"

class Test_NetWork : public BaseThread
{
    Q_OBJECT
    explicit Test_NetWork(QObject *parent = nullptr);
public:
    static Test_NetWork *bulid(QObject *parent = nullptr);

    bool checkNet();
    bool startProcess();
    QString updateMacAddr(int step=1);

signals:
    void msgSig(QString str);

protected:
    void run();
    void workDown();
    void pduInfo(int fn, QString &msg);

    QStringList getCmd();

protected slots:
    void initFunSlot();

private:
    bool mac;
    UdpRecvSocket *mUdp;
    QProcess *mProcess;
};

#endif // TEST_NETWORK_H
