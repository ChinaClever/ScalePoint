#ifndef TEST_FBAPARTITION_H
#define TEST_FBAPARTITION_H

#include "test_serialnumber.h"

class test_FabPartition : public BaseThread
{
    Q_OBJECT
    explicit test_FabPartition(QObject *parent = nullptr);
public:
    static test_FabPartition *bulid(QObject *parent = nullptr);

    bool workDown();
    bool check(){return devExist();}

signals:
    void fabSig(QString str);

protected:
    bool devExist();
    bool at91recovery();

    bool createFab();
    bool programFab();
    bool changePermissions();
    int shexec(const char *cmd, char res[][512], int count);
    QString processOn(const QString &cmd);
    bool isFileExist(const QString &fn);
    bool mvFile(bool res);

private:
    Test_SerialNumber *mSn;
};

#endif // TEST_FBAPARTITION_H
