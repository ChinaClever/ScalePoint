#include "test_basefvt.h"
#define SC_REG "screg"
#define EXE_FN "scdump"
#define USB_DEV "/dev/ttyUSB0"
#include "config.h"

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
    mPinsName = "/dev/"+(&(Cfg::bulid()->item->coms))->sp->getSerialName();
    QString str = mPinsName;
    bool ret = isFileExist(mPinsName);
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
    QStringList ls{"-i", mPinsName};
    pro.start(EXE_FN, ls);
    return mExe->readOutput(pro, ptr);
}


bool Test_BaseFvt::relayControl(int id, int oc)
{
    QProcess pro(this);
    QStringList ls{"-i", mPinsName,"-w"};
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
    int id = 0x0093;
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

QString Test_BaseFvt::transmit(RtuRw * ser ,QString str)
{
    static uchar recv[128] = {0};
    static uchar send[20] = {0};
    int len = str.length()+1;
    memset(send , 0 , sizeof(send));
    memset(recv , 0 , sizeof(recv));
    strcpy((char*)send , str.toLatin1().data());
    send[str.length()]= 0x0D;
    len = ser->transmit(send, len, recv, 10);
    QString recvStr=QString((char*)recv);
    return recvStr;
}

bool Test_BaseFvt::startTest(RtuRw * ser ,QString str , int i)
{
    bool ret = false;
    QString recvStr = transmit(ser , str);
    if(recvStr.contains("test_start")) ret = true;
    else{
        recvStr = tr(" start test failed!!!!!!!!");
        //updatePro(mComStr+recvStr, ret);
        emit mExe->msgSig(mComStr+tr("发送 %1 次 start test: ").arg(i)+recvStr);
    }
    if(ret) emit mExe->msgSig(mComStr+" start test: "+recvStr);
    return ret;
}

bool Test_BaseFvt::getFw(RtuRw * ser ,QString str)
{
    bool ret = false;
    QString recvStr = transmit(ser ,str);
    if(recvStr!=""){ ret = true;mDt->fw = "0x"+recvStr;recvStr="0x"+recvStr;}
    else{
        recvStr = tr(" getFw failed!!!!!!!!");
        updatePro(mComStr+recvStr, ret);
    }
    if(ret) emit mExe->msgSig(mComStr+" getFw: "+recvStr);
    return ret;
}

bool Test_BaseFvt::getIc(RtuRw * ser ,QString str)
{
    bool ret = false;
    QString recvStr = transmit(ser , str);
    if(recvStr!=""){ ret = true;mDt->code= recvStr;}
    else{
        recvStr = tr(" getIc failed!!!!!!!!");
        updatePro(mComStr+recvStr, ret);
    }
    if(ret) emit mExe->msgSig(mComStr+ " getIc: "+recvStr);
    return ret;
}

QString Test_BaseFvt::reverse(QString str)
{
    int len = str.length();
    for(int i = 0 , j = len -1 ; i < j ; i++,j--)
    {
        QChar c = str[i];
        str[i] = str[j];
        str[j] = c;
    }
    for(int i = 0; i < len ; i+=2)
    {
        QChar c = str[i];
        str[i] = str[i+1];
        str[i+1] = c;
    }
    return str;
}

bool Test_BaseFvt::getIeee(RtuRw * ser ,QString str)
{
    bool ret = false;
    QString recvStr = transmit(ser ,str);
    if(recvStr!=""){
        //recvStr = recvStr.right(8)+recvStr.left(8);
        recvStr = reverse(recvStr.left(8))+reverse(recvStr.right(8));
        ret = true;
        mDt->sn= recvStr;
    }
    else{
        recvStr = tr("getIeee failed!!!!!!!!");
        //updatePro(mComStr+recvStr, ret);
        emit mExe->msgSig(mComStr+"    getIeee:   "+recvStr);
    }
    if(ret) emit mExe->msgSig(mComStr+"    getIeee:   "+recvStr);
    return ret;
}

bool Test_BaseFvt::disableTest(RtuRw * ser ,QString str)
{
    bool ret = false;
    QString recvStr = transmit(ser,str);
    if(recvStr.contains("test_disable")) ret = true;
    if(recvStr!=""){ ret = true;}
    else{
        recvStr = tr(" disableTest failed!!!!!!!!");
        updatePro(mComStr+recvStr, ret);
    }
    if(ret) emit mExe->msgSig(mComStr+" disableTest: pass");
    return ret;
}

bool Test_BaseFvt::fullReset(RtuRw * ser ,QString str)
{
    bool ret = false;
    QString recvStr = transmit(ser,str);
    if(recvStr.contains("reset_full")) ret = true;
    if(recvStr!=""){ ret = true;}
    else{
        recvStr = tr(" fullreset failed!!!!!!!!");
        updatePro(mComStr+recvStr, ret);
    }
    if(ret) emit mExe->msgSig(mComStr+" fullreset: "+ recvStr);
    return ret;
}

bool Test_BaseFvt::getToken(int index)
{
    mComStr = (&(Cfg::bulid()->item->coms))->sp->getSerialName();
    RtuRw *ser = mModbus;
    switch(index){
    //case 0:{ comStr = (&(Cfg::bulid()->item->coms))->sp->getSerialName();ser = mModbus;break;}
    case 1:{ mComStr = (&(Cfg::bulid()->item->coms))->comj7->getSerialName();ser = mModbusJ7;break;}
    case 2:{ mComStr = (&(Cfg::bulid()->item->coms))->comj8->getSerialName();ser = mModbusJ8;break;}
    }
    //    if(mModbus)
    //        emit openSig(comStr,QSerialPort::Baud57600);
    QString str = "start_test";
    bool ret = false;
    if(index == 1)
        for(int i = 0 ; i < 5 ; ++i){
            ret = startTest(ser ,str , i);
            if(ret) break;
        }
    if(index == 2){
        //        str = "get_fw";
        //        if(ret) ret = getFw(ser,str);
        //        str = "get_ic";
        //        if(ret) ret = getIc(ser,str);
        //        str = "get_ieee";
        //        if(ret) ret = getIeee(ser,str);
        sleep(2);
        str = "full_reset";
        ret = fullReset(ser,str);
        sleep(30);
        for(int i = 0 ; i < 5 ; ++i){
            str = "start_test";
            ret = startTest(ser,str,i);
            if(ret) break;
        }
        str = "get_ieee";
        if(ret) ret = getIeee(ser,str);
        str = "disable_test";
        ret = disableTest(ser,str);
    }
    return ret;
}

bool Test_BaseFvt::workDown()
{
    QString str;
    bool ret = inputCheck();
    QString comStr = (&(Cfg::bulid()->item->coms))->sp->getSerialName();
    if((&(Cfg::bulid()->item->coms))->sp->isOpened())
        emit closeSig();
    sleep(1);
    ret = inputCheck();
    if(ret) ret = zigbeeConnect();
    sleep(20);
    if(ret) ret = execute(&str);
    if(ret) ret = updateData(str);
    sleep(2);
    //        if(ret) ret = rsConnect();
    return ret;
}

