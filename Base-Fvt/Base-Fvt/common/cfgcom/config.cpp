/*
 * 配置文件公共基类
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "config.h"

Cfg::Cfg(QObject *parent) : CfgCom(parent)
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
}

void Cfg::writeCfgDev()
{
    writeCnt();
    write("HW", item->hw, "Sys");
    write("user", item->user, "User");
    write("pc_num", item->pcNum, "Sys");
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

/**
 * @brief 获取串口名称
 * @return 串口名
 */
QString Cfg::getSerialName(const QString &key)
{
    return read(key, "", "Serial").toString();
}

/**
 * @brief 设置串口名
 * @param name
 */
void Cfg::setSerialName(const QString &key, const QString &v)
{
    write(key, v, "Serial");
}

QString Cfg::getSerialBr(const QString &com)
{
    QString key = QString("BR_%1").arg(com);
    return read(key, "", "Serial").toString();
}

void Cfg::setSerialBr(const QString &com, const QString &br)
{
    QString key = QString("BR_%1").arg(com);
    write(key, br, "Serial");
}

/**
 * @brief 获取当前用户名称
 * @return 用户名
 */
QString Cfg::getLoginName()
{
    return read("name", "admin", "Login").toString();
}

/**
 * @brief 设置当前用户名
 * @param name
 */
void Cfg::setLoginName(const QString &name)
{
    write("name", name, "Login");
}
