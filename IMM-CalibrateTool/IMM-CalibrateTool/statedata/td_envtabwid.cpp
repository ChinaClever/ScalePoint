/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_envtabwid.h"

Td_EnvTabWid::Td_EnvTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
}

void Td_EnvTabWid::initWid()
{
    QStringList header;
    QString title = tr("传感器信息");
    header << tr("传感器") << tr("监测值") << tr("最小值") << tr("最大值");
    initTableWid(header, 2, title);

    sDevData *dev = sDataPacket::bulid()->getDev();
    mEnv = &(dev->data);
}


void Td_EnvTabWid::setDataUnit(int id, sDataUnit &unit)
{
    QString suffix = "℃";
    QString str = tr("温度");
    if(id) {
        suffix = "%";
        str = tr("湿度");
    }

    for(int i=0; i<unit.size; ++i) {
        QStringList listStr;

        listStr << str;
        listStr << QString::number(unit.value[i]/COM_RATE_TEM,'f',1)+suffix;
        listStr << QString::number(unit.min[i]/COM_RATE_TEM,'f',1)+suffix;
        listStr << QString::number(unit.max[i]/COM_RATE_TEM,'f',1)+suffix;
        setTableRow(id+i, listStr);

        if((unit.value[i] < unit.min[i]) || (unit.value[i] > unit.max[i])) {
            unit.alarm[i] = 2;
            setAlarmBackgroundColor(id+i);
        } else {
            unit.alarm[i] = 0;
            setNormalBackgroundColor(id+i);
            setItemColor(i, 1, unit.status[i]);
        }
    }
}


void Td_EnvTabWid::appendItem()
{
    setDataUnit(0, mEnv->tem);
    setDataUnit(1, mEnv->hum);
}

void Td_EnvTabWid::timeoutDone()
{
    clearTable();
    if(mEnv->size) {
        appendItem();
    }
}
