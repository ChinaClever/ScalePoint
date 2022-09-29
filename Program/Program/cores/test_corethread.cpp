/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_corethread.h"
#include "printer_bartender.h"
#include <QDesktopServices>

Test_CoreThread::Test_CoreThread(QObject *parent) : BaseThread(parent)
{

}

void Test_CoreThread::initFunSlot()
{
    BaseLogs::bulid(this);
    mExe = Test_Execute::bulid(this);
    mRtu = SP_SocketRtu::bulid(this);
}

bool Test_CoreThread::enumDeviceType()
{
    QString str = tr("设备类型：");
    bool ret = mRtu->enumDeviceType();
    if(!ret) {
        ret = mExe->startProcess();
        if(ret) ret = mRtu->enumDeviceType();
    }

    if(ret) {
        str += tr("%1；输出位：%2").arg(mDt->dt).arg(mDt->outputs);
    } else str += tr("识别错误"); updatePro(str, ret);

    if(ret) {
        bool res = mRtu->onewireBus();
        str = "OneWire Bus " + tr("检测 ");
        if(res) str += tr("正常"); else str += tr("错误");
        updatePro(str, res);
    }

    return ret;
}

bool Test_CoreThread::readDev(QString& ver,QString& sn)
{
    QString str = tr("请求地址 ");
    bool ret = mRtu->requestAddr();
    if(ret) str += tr("正常：Addr=%1").arg(mDt->addr); else str += tr("错误");
    updatePro(str, ret);

    if(ret) {
        ret = mRtu->readVersion(ver);
        str = tr("Modbus-RTU通讯 版本读取 ");
        if(ret) str += tr("正常"); else str += tr("错误");
        updatePro(str, ret);
    }

    if(ret) {
        ret = mRtu->readSn(sn); str = tr("设备SN码读取 ");
        if(ret) str += tr("正常：SN %1").arg(mDt->sn); else str += tr("错误");
        updatePro(str, ret);
    }

    return ret;
}

void Test_CoreThread::workResult()
{
    bool res = true;
    BaseLogs::bulid()->start();//记录日志和文件路径
    QString str = tr("最终结果 ");
    if(mPro->result != Test_Fail) {
        str += tr("通过");
    } else {
        res = false;
        str += tr("失败");
    }

    updatePro(str, res, 2);
    mPro->step = Test_Over;
}

bool Test_CoreThread::isFileExist(const QString &fn)
{
    QFile file(fn);
    if (file.exists()){
        return true;
    }

    return false;
}

bool Test_CoreThread::readInfo(QString& ver , QString& sn)
{
    bool ret = updatePro(tr("即将开始，正在复位串口板"));
    if(ret) ret = mExe->startProcess();
    if(ret) ret = enumDeviceType();
    if(ret) ret = readDev(ver , sn);
    return ret;
}

bool Test_CoreThread::startProgram(const QString& propath,const QString& filepath)
{
    QProcess pingProcess;
    //QString strArg = "ping " + ip + " -n 2 -i 2";  //strPingIP 为设备IP地址
    QString strArg = QString("JFlash.exe -openprj%1 -open%2,0x8000000 -auto -exit").arg(propath).arg(filepath);  //strPingIP 为设备IP地址
    pingProcess.start(strArg,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);

    //    QString p_stdout = QString::fromLocal8Bit( pingProcess.readAllStandardOutput());
    bool bPingSuccess = true;

    return bPingSuccess;
}

void Test_CoreThread::getIndexSlot(int index)
{
    mIndex = index;
}

void Test_CoreThread::workDown()
{

}


bool Test_CoreThread::createFile(QString sn)
{
    bool ret = false;

    return ret;
}

bool Test_CoreThread::readFile(QString strPath , QByteArray &array)
{
    QFile file(strPath);
    qDebug()<<"strPath " << strPath<<endl;
    bool ret = file.exists();
    if(ret) {
        ret = file.open(QIODevice::ReadOnly);
        if(ret) array = file.readAll();
        file.close();
    } else {
        qDebug() << "open file err";
    }

    return ret;
}

bool Test_CoreThread::writeFile(QByteArray &array)
{
    QString curpath = QCoreApplication::applicationDirPath();
    QString path = curpath.remove(curpath.lastIndexOf('/') , curpath.size() - curpath.lastIndexOf('/') );///////
    qDebug()<< " path " << path<< endl;
    QFile file(path+QString("/srcFile/%1/%2.bin").arg(mDev->dt.sn).arg(mDev->dt.sn) );
    bool ret = file.open(QIODevice::WriteOnly);
    if(ret) {
        array.remove(array.size() - 4, 4);//删除原来的最后四个字节

        bool ok;
        QByteArray arr = cm_HexStringToByteArray("00 00 00 01" , &ok);///////
        qDebug()<<array.size() <<" remove filearray  " << arr.size();
        for(int i = 0 ; i < arr.size() ; i++){
            array.append(arr.at(3 - i));
        }
        qDebug()<< array.size() <<" last filearray  ";

        file.write(array);

        file.close();
    }

    return ret;
}

void Test_CoreThread::cmd(QString ls)
{
    QProcess pingProcess;
    //QString strArg = "ping " + ip + " -n 2 -i 2";  //strPingIP 为设备IP地址
    QString strArg = QString("%1").arg(ls);  //strPingIP 为设备IP地址
    pingProcess.start(strArg,QIODevice::ReadOnly);
    pingProcess.waitForFinished(-1);
}

void Test_CoreThread::run()
{
    if(isRun) return; else isRun = true;
    QString ver , sn;
    switch(mIndex){
    case 0:{
        //bool ret = readInfo(mDev->dt.fw , mDev->dt.sn);
        bool ret = true;
        if(ret) {
            ret = updatePro(tr("读取序列号和版本成功"));
            //读取序列号和版本成功
            if(ret){
                QString curpath = QCoreApplication::applicationDirPath();
                QString path = curpath.remove(curpath.lastIndexOf('/') , curpath.size() - curpath.lastIndexOf('/') );///////
                qDebug()<< " path " << path<< endl;

                QDir dirFile(path+"/srcFile");
                if(!dirFile.exists()) {
                    dirFile.mkdir(path+"/srcFile");
                }
                mDev->dt.sn ="00 00 00 01";///////
                mDev->dt.sn.remove(QRegExp("\\s"));
                QString filepath = QString("/srcFile/%1").arg(mDev->dt.sn);

                filepath =  path + filepath;
                QDir snFile( filepath );
                qDebug()<<filepath;
                if(!snFile.exists()) {
                    snFile.mkpath(filepath);
                }
                QByteArray array;
                ret = readFile(mItem->srcPath , array);
                qDebug()<< array.size() << " filearray  ";
                if(ret) {
                    ret = writeFile(array);
                    if(ret){
                        updatePro(tr("转换成功"));

//                        QString fn = curpath.remove(curpath.lastIndexOf('/') , curpath.size() - curpath.lastIndexOf('/') )
//                                +QString("/srcFile/%1/%2.bin").arg(mDev->dt.sn).arg(mDev->dt.sn);///////
//                        int index = fn.lastIndexOf('/');
//                        QDesktopServices::openUrl(QUrl("file:///"+fn.left(index), QUrl::TolerantMode));
                    }
                    else{
                        updatePro(tr("转换失败"),false);
                        mPro->result = Test_Fail;
                    }
                }else{
                    updatePro(tr("读取文件失败"),false);
                    mPro->result = Test_Fail;
                }
            }
        } else mPro->result = Test_Fail;
        //workResult();
        break;
    }
    case 1:{ /*mItem->srcPath*/startProgram(mItem->proPath,mItem->srcPath);break;}
    case 2:{
        bool ret = readInfo(ver , sn);
        if(ret){
        } else mPro->result = Test_Fail;
        workResult();break;}
    }
    isRun = false;
}
