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
    item->coms.comj7 = nullptr;
    item->coms.comj8 = nullptr;

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
    QString g = "Sys";
    item->hw = read("HW", "", g).toString();
    item->pcNum = read("pc_num", 0, g).toInt();
    item->user = read("user", "", g).toString();

    item->firmware = read("firmware", "").toString();
    item->commander = read("commander", "").toString();
    item->bootloader = read("bootloader", "").toString();
}

void Cfg::writeCfgDev()
{
    writeCnt();
    QString g = "Sys";
    write("HW", item->hw, g);
    write("user", item->user, g);
    write("pc_num", item->pcNum, g);

    write("firmware", item->firmware);
    write("commander", item->commander);
    write("bootloader", item->bootloader);
}

void Cfg::initCnt()
{
    QString g = "Cnts";
    item->cnts.cnt = read("cnt", 0, g).toInt();
    item->cnts.all = read("all", 0, g).toInt();
    item->cnts.err = read("err", 0, g).toInt();
    item->cnts.ok = read("ok", 0, g).toInt();
}

void Cfg::writeCnt()
{
    QString g = "Cnts";
    write("cnt", item->cnts.cnt, g);
    write("all", item->cnts.all, g);
    write("err", item->cnts.err, g);
    write("ok", item->cnts.ok, g);
}
