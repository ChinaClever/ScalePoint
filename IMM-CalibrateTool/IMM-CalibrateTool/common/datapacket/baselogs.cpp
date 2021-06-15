#include "baselogs.h"
extern QString user_land_name();

BaseLogs::BaseLogs(QObject *parent) : QThread(parent)
{

}

BaseLogs *BaseLogs::bulid(QObject *parent)
{
    static BaseLogs* sington = nullptr;
    if(sington == nullptr)
        sington = new BaseLogs(parent);
    return sington;
}


bool BaseLogs::writeMac()
{
    sMacItem it;

    it.dev = mDt->dev_type.split("_").first();
    it.user = mItem->user;
    it.sn = mDt->sn;
    it.mac = mMac;
    mMac.clear();

    return DbMacs::bulid()->insertItem(it);
}

bool BaseLogs::appendLogItem(const QString &str, bool pass)
{
    sStateItem it;
    if(pass) {
        it.result = tr("通过");
    } else {
        it.result = tr("失败");
    }

    it.memo = str;
    mLogItems << it;

    return pass;
}

void BaseLogs::saveLogs()
{
    bool ret = writeLog();
    if(ret) {
        writeLogs();
        if(mMac.size()) writeMac();
    } else {
        // updatePro(tr("因未创建序列号，日志无法保存！"), false);
    }
    mLogItems.clear();
}

bool BaseLogs::writeLog()
{
    Db_Tran db;
    sLogItem it;

    it.dev = mDt->dev_type.split("_").first();
    it.op = user_land_name();
    it.user = mItem->user;
    it.sn = mDt->sn;

    mItem->cnt.all += 1;
    if(mPro->result != Test_Fail) {
        it.result = tr("通过");
        mItem->cnt.ok += 1;
        mItem->cnt.cnt--;
        if(!mItem->cnt.cnt)  {
            mItem->user.clear();
            Cfg::bulid()->write("user", mItem->user, "User");
        }
    } else {
        mItem->cnt.err += 1;
        it.result = tr("失败");
    }

    Cfg::bulid()->writeCnt();
    if(it.sn.isEmpty()) {
        emit DbLogs::bulid()->itemChanged(0, 1);
        return false;
    }

    return DbLogs::bulid()->insertItem(it);
}

bool BaseLogs::initItem(sStateItem &it)
{
    it.dev = mDt->dev_type.split("_").first();
    it.user = mItem->user;
    it.sn = mDt->sn;

    return it.sn.size();
}

void BaseLogs::writeLogs()
{
    Db_Tran db;
    for(int i=0; i<mLogItems.size(); ++i) {
        sStateItem it = mLogItems.at(i);
        if(initItem(it)) DbStates::bulid()->insertItem(it);
    }
    mLogItems.clear();
}
