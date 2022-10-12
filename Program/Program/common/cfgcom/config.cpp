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
    item->proPath = read("propath", "").toString();
    item->srcPath = read("srcpath", "").toString();
    item->jflashPath = read("jflashpath", "").toString();
}

void Cfg::writeCfgDev()
{
    writeCnt();
    write("user", item->user, "User");
    write("pc_num", item->pcNum, "Sys");
    write("propath", item->proPath);
    write("srcpath", item->srcPath);
    write("jflashpath", item->jflashPath);
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
