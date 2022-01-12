#ifndef TEST_EXECUTE_H
#define TEST_EXECUTE_H
#include "baselogs.h"
#include "sp_socketrtu.h"

class Test_Execute : public BaseThread
{
    Q_OBJECT
    explicit Test_Execute(QObject *parent = nullptr);
public:
    static Test_Execute *bulid(QObject *parent = nullptr);
    bool startProcess(const QStringList &s) {return execute(s);}
protected:
    bool execute(const QStringList &arguments);
    bool readOutput(QProcess &pro);

signals:
    void msgSig(QString str);
};

#endif // TEST_EXECUTE_H
