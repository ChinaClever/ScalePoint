/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "test_corethread.h"
#include "printer_bartender.h"
#include <QDesktopServices>
#include <QUrl>

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

void Test_CoreThread::workResult(int step)
{
    bool res = true;
    BaseLogs::bulid()->getStep(step);
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
        QByteArray array;
        bool ret = file.open(QIODevice::ReadOnly);
        if(ret){
            array = file.readAll();
            bool ok;
            QByteArray arr = cm_HexStringToByteArray(mDev->dt.sn , &ok);
            int n = array.size();
            for(int i = 0 ; i < arr.size() ; i++){
                if ( array.at(n - 1 - i) == arr.at(i)) continue;
                else return false;
            }
        }
        file.close();
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
    //QString strArg = QString("-openprj%1 -open%2,0x8000000 -auto -exit").arg(propath).arg(filepath);  //strPingIP 为设备IP地址
    QStringList ls;
    ls << QString("-openprj%1").arg(propath) << QString("-open%1,0x8000000").arg(filepath)
          << QString("-auto")<< QString("-exit");
    pingProcess.start(mItem->jflashPath, ls);

    bool bPingSuccess = pingProcess.waitForFinished(5000);

    return bPingSuccess;
}

void Test_CoreThread::getIndexSlot(int index)
{
    mIndex = index;
}


void Test_CoreThread::getTypeSlot(int type)
{
    mType = type;
    if(0 == mType){
        mTypeStr = "Socket";
    }
    else{
        mTypeStr = "IMM";
    }
}

bool Test_CoreThread::createFile()
{
    bool ret = true;
    QByteArray array;
    ret = readFile(mItem->srcPath , array);
    if(ret) {
        ret = writeFile(array);
        if(ret){
            updatePro(tr("转换成功"));
            //mPro->step = Test_Over;
        }
        else{
            updatePro(tr("转换失败"),false);
            mPro->result = Test_Fail;
        }
    }else{
        updatePro(tr("读取文件失败"),false);
        mPro->result = Test_Fail;
    }
    return ret;
}

bool Test_CoreThread::readFile(QString strPath , QByteArray &array)
{
    QFile file(strPath);
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
    QFile file( mWritePath );
    bool ret = file.open(QIODevice::WriteOnly);
    if(ret) {
        array.remove(array.size() - 4, 4);//配合测试不同情况
        bool ok;
        QByteArray arr = cm_HexStringToByteArray(mDev->dt.sn , &ok);
        for(int i = 0 ; i < arr.size() ; i++){
            array.append(arr.at(3 - i));
        }
        file.write(array);
        file.close();
    }

    return ret;
}

void Test_CoreThread::run()
{
    if(isRun) return; else isRun = true;
    switch(mIndex){
    case First_read:{
        bool ret = readInfo(mDev->dt.fw , mDev->dt.sn);////////////////
        //bool ret = true;////////////////
        if(ret) {
            firstVer = mDev->dt.fw;
            firstSN = mDev->dt.sn;
            ret = updatePro(tr("读取序列号和版本成功"));
            //读取序列号和版本成功
            if(ret){
                QString curpath = QCoreApplication::applicationDirPath();
                QString path = curpath.remove(curpath.lastIndexOf('/') , curpath.size() - curpath.lastIndexOf('/') );///////

                //mDev->dt.sn ="00 00 00 01";///////test
                QString snstr = mDev->dt.sn;
                snstr.remove(QRegExp("\\s"));
                QString filepath = QString("/srcFile/%2/%1").arg(snstr).arg(mTypeStr);

                filepath =  path + filepath;
                QDir snFile( filepath );
                if(!snFile.exists()) {
                    ret = snFile.mkpath(filepath);
                }
                mWritePath = path+QString("/srcFile/%3/%1/%2.bin").arg(snstr).arg(snstr).arg(mTypeStr);
                if(ret){
                    ret = createFile();
                    if(ret){
                        ret = isFileExist(mWritePath);
                        if(ret){
                            QString str = QString("%2升级文件创建成功 ： %1").arg(mDev->dt.sn).arg(mTypeStr);
                            updatePro(str);
                        }else{
                            updatePro(tr("升级文件创建失败"),false);
                            mPro->result = Test_Fail;
                        }
                    }else{
                        updatePro(tr("写文件失败"),false);
                        mPro->result = Test_Fail;
                    }
                }
                else{
                    updatePro(tr("创建文件夹失败"),false);
                    mPro->result = Test_Fail;
                }
            }
        } else {
            updatePro(tr("读取序列号和版本失败"),false);
            mPro->result = Test_Fail;
        }
        workResult(0);
        break;
    }
    case Program:{startProgram(mItem->proPath,mWritePath);updatePro(tr("执行完烧录步骤，请重启通讯板！"));mPro->step = Test_Over;break;}
    case Second_read:{
        QString secondVer , secondSN;
        bool ret = readInfo(secondVer , secondSN);
        if(ret){
            //compare
            if(firstVer != secondVer && firstSN == secondSN){
                ret = true;
                updatePro(tr("烧录成功"));
            }
            else{
                ret = true;
                updatePro(tr("烧录失败"),false);
            }
        } else {
            updatePro(tr("读取序列号和版本失败"),false);
            mPro->result = Test_Fail;
        }
        workResult(1);break;}
    }
    isRun = false;
}
