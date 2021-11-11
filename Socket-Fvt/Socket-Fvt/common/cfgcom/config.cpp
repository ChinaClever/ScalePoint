/*
 * 配置文件公共基类
 *
 *  Created on: 2018年1月1日
 *      Author: Lzy
 */
#include "config.h"

Cfg::Cfg()
{
    mCfg = CfgCom::bulid();
    item = new sCfgItem();
    item->coms.sp = nullptr;

    initCnt();
    initCfgDev();
}

Cfg *Cfg::bulid()
{
    static Cfg* sington = nullptr;
    if(sington == nullptr)
        sington = new Cfg();
    return sington;
}


bool Cfg::getDate()
{
    bool ret = false;
    QString str = read("date","","Date").toString();
    if(!str.isEmpty()) {
        QDate date = QDate::fromString(str, "yyyy-MM-dd");
        if(QDate::currentDate() > date)  ret = true;
    }

    return ret;
}


void Cfg::initCfgDev()
{
    item->user = read("user", "", "User").toString();
    item->pcNum = read("pc_num", 0, "Sys").toInt();
}

void Cfg::writeCfgDev()
{
    writeCnt();
    write("user", item->user, "User");
    write("pc_num", item->pcNum, "Sys");
}

void Cfg::initCnt()
{
    item->cnts.cnt = read("cnt", 0, "Cnts").toInt();
    item->cnts.all = read("all", 0, "Cnts").toInt();
    item->cnts.ok = read("ok", 0, "Cnts").toInt();
    item->cnts.err = read("err", 0, "Cnts").toInt();
    item->cnts.currentNum = read("num", 0,"Cnts").toInt();
    if(getDate()) item->cnts.currentNum = 0;
}

void Cfg::writeCnt()
{
    write("cnt", item->cnts.cnt, "Cnts");
    write("all", item->cnts.all, "Cnts");
    write("ok", item->cnts.ok, "Cnts");
    write("err", item->cnts.err, "Cnts");
    write("num", item->cnts.currentNum, "Cnts");
    QString value = QDate::currentDate().toString("yyyy-MM-dd");
    write("date", value, "Cnts");
    write("user", item->user, "User");
}

/**
 * @brief 获取串口名称
 * @return 串口名
 */
QString Cfg::getSerialName(const QString &key)
{
    return mCfg->read(key, "", "Serial").toString();
}

/**
 * @brief 设置串口名
 * @param name
 */
void Cfg::setSerialName(const QString &key, const QString &v)
{
    mCfg->write(key, v, "Serial");
}

QString Cfg::getSerialBr(const QString &com)
{
    QString key = QString("BR_%1").arg(com);
    return mCfg->read(key, "", "Serial").toString();
}

void Cfg::setSerialBr(const QString &com, const QString &br)
{
    QString key = QString("BR_%1").arg(com);
    mCfg->write(key, br, "Serial");
}


/**
 * @brief 根据名称获取配置文件数值
 * @return 对应的配置文件数值
 */
QVariant Cfg::read(const QString &key, const QVariant &v, const QString &g)
{
    return mCfg->read(key, v, g);
}

/**
 * @brief 设置对应名称的配置文件数值
 * @param value
 * @param name
 */
void Cfg::write(const QString &key, const QVariant& v, const QString &g)
{
    mCfg->write(key, v, g);
}

/**
 * @brief 获取当前用户名称
 * @return 用户名
 */
QString Cfg::getLoginName()
{
    return mCfg->read("name", "admin", "Login").toString();
}

/**
 * @brief 设置当前用户名
 * @param name
 */
void Cfg::setLoginName(const QString &name)
{
    mCfg->write("name", name, "Login");
}
