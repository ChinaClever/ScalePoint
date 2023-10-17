/*
 * 配置文件公共基类
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "config.h"

Cfg::Cfg(QObject *parent) : CfgSerial(parent)
{
    item = new sCfgItem();
    item->coms.sp = nullptr;

    initCnt();
    initCfgDev();
}

Cfg *Cfg::bulid(QObject *parent)
{
    static Cfg* sington = nullptr;
    if(!sington) sington = new Cfg(parent);
    return sington;
}

void Cfg::initCfgDev()
{
    item->user = read("user", "", "User").toString();
    item->pcNum = read("pc_num", 0, "Sys").toInt();
    item->hw = read("HW", "", "Sys").toString();
    item->port = read("port", 10046, "Sys").toInt();
}

//int Cfg::initPort()
//{
//    item->port = read("port", 10046, "Sys").toInt();
//    return item->port;
//}

void Cfg::writeCfgDev()
{
    writeCnt();
    write("HW", item->hw, "Sys");
    write("user", item->user, "User");
    write("pc_num", item->pcNum, "Sys");
    write("port", item->port, "Sys");
}

void Cfg::initCnt()
{
    item->cnts.cnt = read("cnt", 0, "Cnts").toInt();
    item->cnts.all = read("all", 0, "Cnts").toInt();
    item->cnts.ok = read("ok", 0, "Cnts").toInt();
    item->cnts.err = read("err", 0, "Cnts").toInt();
}

void Cfg::writeCnt()
{
    write("cnt", item->cnts.cnt, "Cnts");
    write("all", item->cnts.all, "Cnts");
    write("ok", item->cnts.ok, "Cnts");
    write("err", item->cnts.err, "Cnts");
    write("user", item->user, "User");
}
