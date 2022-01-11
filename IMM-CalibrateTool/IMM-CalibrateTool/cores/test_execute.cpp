#include "test_execute.h"

Test_Execute::Test_Execute(QObject *parent) : BaseThread(parent)
{

}

Test_Execute *Test_Execute::bulid(QObject *parent)
{
    static Test_Execute* sington = nullptr;
    if(sington == nullptr)
        sington = new Test_Execute(parent);
    return sington;
}

bool Test_Execute::readOutput(QProcess &pro)
{
    bool ret, res = false;
    do {
        ret = pro.waitForFinished(1000);
        QByteArray bs = pro.readAllStandardOutput();
        bs +=  pro.readAllStandardError();
        QString str = QString::fromLocal8Bit(bs); // emit msgSig(str);
        if(str.contains("CONNECTED")) res = true; //else str = str.simplified();
        QStringList ls = str.split("\n", QString::SkipEmptyParts);
        for(int i=0; i<ls.size(); ++i) updatePro(ls.at(i).simplified());
    } while(!ret);
    pro.close();

    return res;
}

bool Test_Execute::execute()
{
    bool ret = false;
    QString exe = "SimpleIO_UM_CSExampleCode.exe";
    QFile file(exe);
    if (file.exists()){
        QProcess pro(this);
        pro.start(exe);
        ret = readOutput(pro);
        if(!ret)updatePro(tr("串口未正常连接") , ret);
    } else {
        updatePro(exe+tr(" 文件不存在") , ret);
    }

    return ret;
}

