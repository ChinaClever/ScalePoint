/*
 *
 *
 *  Created on: 2020年10月11日
 *      Author: Lzy
 */
#include "dblogs.h"

DbLogs::DbLogs()
{
    createTable();
    tableTile = tr("状态日志");
    //hiddens <<  9;
    headList << tr("设备型号") << tr("客户名称") << tr("软件版本") << tr("硬件版本") << tr("结果") << tr("设备序列号");
}

void DbLogs::createTable()
{
    QString cmd =
            "create table if not exists %1("
            "id             INTEGER primary key autoincrement not null,"
            "date           VCHAR,"
            "time           VCHAR,"
            "pn             VCHAR,"
            "user           VCHAR,"
            "fw             VCHAR,"
            "hw             VCHAR,"
            "result         VCHAR,"
            "sn             VCHAR not null);";
    QSqlQuery query(mDb);
    if(!query.exec(cmd.arg(tableName()))) {
        throwError(query.lastError());
    }
}


DbLogs *DbLogs::bulid()
{
    static DbLogs* sington = nullptr;
    if(sington == nullptr)
        sington = new DbLogs();
    return sington;
}

bool DbLogs::insertItem(const sLogItem &item)
{
    QString cmd = "insert into %1 (date,time,pn,user,fw,hw,result,sn) "
                  "values(:date,:time,:pn,:user,:fw,:hw,:result,:sn)";
    bool ret = modifyItem(item,cmd.arg(tableName()));
    if(ret) emit itemChanged(item.id, Insert);
    return ret;
}

bool DbLogs::modifyItem(const sLogItem &item, const QString &cmd)
{
    QSqlQuery query(mDb);
    query.prepare(cmd);
    query.bindValue(":date",item.date);
    query.bindValue(":time",item.time);
    query.bindValue(":pn",item.pn);
    query.bindValue(":user",item.user);
    query.bindValue(":fw",item.fw);
    query.bindValue(":hw",item.hw);
    query.bindValue(":result",item.result);
    query.bindValue(":sn",item.sn);
    bool ret = query.exec();
    if(!ret) throwError(query.lastError());
    return ret;
}
