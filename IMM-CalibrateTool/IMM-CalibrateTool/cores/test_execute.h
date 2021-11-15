#ifndef TEST_EXECUTE_H
#define TEST_EXECUTE_H
#include "yc_obj.h"
#include "ad_corethread.h"

class Test_Execute : public BaseThread
{
    Q_OBJECT
    explicit Test_Execute(QObject *parent = nullptr);
public:
    static Test_Execute *bulid(QObject *parent = nullptr);
    bool startProcess() {return execute();}

protected:
    bool execute();
    bool readOutput(QProcess &pro);

signals:
    void msgSig(QString str);
};

#endif // TEST_EXECUTE_H
