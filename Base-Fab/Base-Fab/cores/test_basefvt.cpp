#include "test_basefvt.h"
#define SC_REG "screg"
#define EXE_FN "scdump"
#define USB_DEV "/dev/ttyUSB0"

Test_BaseFvt::Test_BaseFvt(QObject *parent)
    : BaseThread{parent}
{
    mExe = Test_Execute::bulid(this);
}

Test_BaseFvt *Test_BaseFvt::bulid(QObject *parent)
{
    static Test_BaseFvt* sington = nullptr;
    if(sington == nullptr)
        sington = new Test_BaseFvt(parent);
    return sington;
}

bool Test_BaseFvt::isFileExist(const QString &fn)
{
    bool ret = false; QFileInfo file(fn);
    if (file.exists()) ret =  true;
    return ret;
}

bool Test_BaseFvt::inputCheck()
{
    QString str = USB_DEV;
    bool ret = isFileExist(str);
    if(ret) {
        str = "echo \"123456\" | sudo -S chmod 777 " + str;
        system(str.toLatin1().data());
    } else {
        str = tr(" 未接入设备，请连接Base");
        emit mExe->msgSig(str);
        return false;
    }

    str = EXE_FN;
    ret = isFileExist(str);
    if(ret) {
        str = "echo \"123456\" | sudo -S chmod 777 " + str;
        system(str.toLatin1().data());
    } else {
        str = tr(" 文件未接找到") + str;
        emit mExe->msgSig(str);
        return false;
    }

    return ret;
}

bool Test_BaseFvt::execute(QString *ptr)
{
    QProcess pro(this);
    QStringList ls{"-i", "/dev/ttyUSB0"};
    pro.start(EXE_FN, ls);
    return mExe->readOutput(pro, ptr);
}


bool Test_BaseFvt::relayControl(int id, int oc)
{
    QProcess pro(this);
    QStringList ls{"-i", "/dev/ttyUSB0","-w"};
    ls << QString::number(id);
    ls << QString::number(oc);
    pro.start(SC_REG, ls);
    return mExe->readOutput(pro);
}


void Test_BaseFvt::mdelay(int ms)
{
    QTime t = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < t) {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}

bool Test_BaseFvt::relayControl(int id)
{
    bool ret = relayControl(id, 0x03);
    if(ret){
        mdelay(200);
        ret = relayControl(id, 0x02);
        mdelay(500);
    }
    return ret;
}

bool Test_BaseFvt::zigbeeConnect()
{
    int id = 0x0097;
    return relayControl(id);
}

bool Test_BaseFvt::rsConnect()
{
    int id = 0x0096;
    return relayControl(id);
}


bool Test_BaseFvt::zigbeeCheck(const QString &v)
{
    bool ret = true; int id = 0; QString str = v;
    int index = str.indexOf("Dual Zigbee Sensor #0");
    QStringList ls = str.remove(0, index).split("\n");
    if(ls.size() > 6) {
        QString str = ls.at(id+1).split(":").last().simplified();
        if(str.toInt(nullptr, 16) > 0) mDt->zigbee = str;
        else { ret = false; str = tr("Zigbee设备信息错误");
            updatePro(str, ret); emit mExe->msgSig(str); }
    }
    return ret;
}


bool Test_BaseFvt::updateData(const QString &v)
{
    bool ret = false; int id = 0;
    QStringList ls = v.split("\n", QString::SkipEmptyParts);
    if(ls.size() > 6) {
        for(int i=0; i<2; ++i) ls.removeFirst();
        mDt->id = ls.at(id++).split(":").last().simplified();
        mDt->fw = ls.at(id++).split(":").last();
        mDt->boot = ls.at(id++).split(":").last();
        mDt->protocol = ls.at(id++).split(":").last();

        QString str = ls.at(id+1).split(":").last().simplified().remove(0,2);
        mDt->sn = QString::number(str.toInt(nullptr, 16)).rightJustified(10, '0');
        ret = zigbeeCheck(v);
    } else  {
        ret = false; QString str = tr("获取设备信息不全");
        updatePro(str, ret); emit mExe->msgSig(str);
    }

return ret;
}


bool Test_BaseFvt::workDown()
{
    QString str;
    bool ret = inputCheck();
    if(ret) ret = zigbeeConnect();
    if(ret) ret = execute(&str);
    if(ret) ret = updateData(str);
    if(ret) ret = rsConnect();
    return ret;
}
