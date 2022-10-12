#ifndef TEST_CORETHREAD_H
#define TEST_CORETHREAD_H
#include "test_execute.h"
#include "bt_serial.h"

class Test_CoreThread : public BaseThread
{
    Q_OBJECT
public:
    explicit Test_CoreThread(QObject *parent = nullptr);

protected:
    void run();
    bool readInfo(QString &ver, QString &sn);
    bool readDev(QString &ver, QString &sn );
    bool enumDeviceType();

    void workDown();
    void workResult(int step = 0);
    bool startProgram(const QString& propath,const QString& filepath);
    bool isFileExist(const QString &fn);
    bool createFile();
    bool readFile(QString strPath, QByteArray &array);
    bool writeFile(QByteArray &array);

protected slots:
    void initFunSlot();
    void getIndexSlot(int index);
    void getTypeSlot(int type);

private:
    Test_Execute *mExe;
    SP_SocketRtu *mRtu;
    int mIndex;
    int mType;
    QString mTypeStr;
    QString mWritePath;
    QString firstVer , firstSN;
};

#endif // TEST_CORETHREAD_H
