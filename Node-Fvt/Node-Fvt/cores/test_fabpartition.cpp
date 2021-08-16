#include "test_fabpartition.h"

test_FabPartition::test_FabPartition(QObject *parent) : BaseThread(parent)
{

}


test_FabPartition *test_FabPartition::bulid(QObject *parent)
{
    static test_FabPartition* sington = nullptr;
    if(sington == nullptr)
        sington = new test_FabPartition(parent);
    return sington;
}

bool test_FabPartition::isFileExist(const QString &fn)
{
    QFile file(fn);
    if (file.exists()){
        return true;
    }

    return false;
}

int test_FabPartition::shexec(const char *cmd, char res[][512], int count)
{
#if defined(Q_OS_LINUX)
    FILE* pp = popen(cmd, "r");
#elif
    FILE* fp = nullptr;
#endif

    if(!pp) {
        qDebug("error, cannot popen cmd: %s\n", cmd);
        return -1;
    }

    int i = 0;
    res[0][0] = 0;
    char tmp[512] ={0};
    while(fgets(tmp, sizeof(tmp), pp) != NULL) {
        if(tmp[strlen(tmp)-1] == '\n') {
            tmp[strlen(tmp)-1] = '\0';
        }
        // qDebug("%d.get return results: %s\n", i, tmp);
        strcpy(res[i], tmp); i++;
        if(i >= count) {
            qDebug("get enough results, return\n");
            break;
        }
    }

    int rv = pclose(pp);
    // qDebug("ifexited: %d\n", WIFEXITED(rv));
    if (WIFEXITED(rv)) {
        qDebug("subprocess exited, exit code: %d\n", WEXITSTATUS(rv));
    }

    return i;
}

QString test_FabPartition::processOn(const QString &cmd)
{
    static char res[10][512];

    QString str;
    emit fabSig("shexec, cmd: －－－－\n" + cmd);
    char *ptr = cmd.toLatin1().data();
    int cnt = shexec(ptr, res, 10);
    for(int i=0; i<cnt; ++i) str.append(res[i]);
    emit fabSig("return results: －－－－\n" +str);
    return str;
}

bool test_FabPartition::devExist()
{
    QString str = "Atmel USB";
    bool ret = isFileExist("/dev/ttyACM0");
    if(ret) {
        str += tr("已连接");
        // processOn("echo \"123456\" | sudo -S chmod 777 /dev/ttyACM0");
    } else {
        str += tr("未找到设备，请确认烧录线是否连接正确？");
    }

    return updatePro(str, ret);
}


bool test_FabPartition::at91recovery()
{
    bool ret = isFileExist("at91recovery");
    if(ret) {
        // processOn("echo \"123456\" | sudo -S chmod 777 at91recovery");
    } else {
        updatePro(tr(" at91recovery 执行程序未发现"), ret);
    }

    return ret;
}

bool test_FabPartition::changePermissions()
{
    QString str = tr("改变IMG文件的权限");
    updatePro(tr("准备")+str);

    QString cmd = "echo \"123456\" | sudo -S chmod 777 %1.img \n"
                  "sudo chmod 777 at91recovery \n"
                  "sudo chmod 777 /dev/ttyACM0" ;
    processOn(cmd.arg(mDt->sn));

    return updatePro(tr("已")+str);
}

bool test_FabPartition::programFab()
{
    QString str = tr("写入S/N Mac ");
    updatePro(tr("准备")+str);

    QStringList ls;
    QProcess pro(this);
    ls << "-y" << "/dev/ttyACM0" << mDt->sn+".img" << "fab";
    pro.start("at91recovery", ls);
    bool ret = pro.waitForFinished();

    QByteArray bs = pro.readAllStandardOutput();
    bs +=  pro.readAllStandardError();
    QString res = QString::fromLocal8Bit(bs);
    if(res.contains("ERR")) {
        ret = false;
        str += tr(" 失败");
    } else {
        str += tr(" 成功");
    }

    emit fabSig(res);
    return updatePro(str, ret);
}

bool test_FabPartition::createFab()
{
    QString cmd = "mkdir -p ScalePoint \n"
                  "cd ScalePoint \n"
                  "echo \"MAC=%1\" > system.cfg \n"
                  "echo \"BOARD_SERIAL=%2\" >> system.cfg \n"
                  "cd ../ \n"
                  "mkfs.cramfs -b 4096 ScalePoint/ %2.img \n";

    QString str = "create FAB partition ";
    bool ret = isFileExist("ScalePoint/eto-desc.xml");
    if(ret) {
        QString res = processOn(cmd.arg(mItem->macs.mac).arg(mDt->sn));
    } else {
        str = tr("配置文件缺少 eto-desc.xml");
    }

    return updatePro(str, ret);
}


bool test_FabPartition::workDown()
{
    bool ret = at91recovery();
    if(ret) ret = createFab();
    if(ret) ret = changePermissions();
    if(ret) ret = programFab();

    return ret;
}


