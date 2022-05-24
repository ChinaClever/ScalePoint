/*
 *
 *  Created on: 2022年1月1日
 *      Author: Lzy
 */
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

bool Test_Execute::readOutput(QProcess &pro, QString *ptr)
{
    bool ret, res = true;
    do {
        ret = pro.waitForFinished(1000);
        QByteArray bs = pro.readAllStandardOutput();
        bs +=  pro.readAllStandardError();
        QString str = QString::fromLocal8Bit(bs);
        if(str.contains("ERROR")) res = false;
        emit msgSig(str); if(ptr) ptr->append(str);
    } while(!ret);
    pro.close();

    return res;
}

bool Test_Execute::execute(const QStringList &arguments)
{
    QProcess pro(this);
    pro.start(mItem->commander, arguments);
    return readOutput(pro);
}

