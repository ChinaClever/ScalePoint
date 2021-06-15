/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_thresholdtabwid.h"

Td_ThresholdTabWid::Td_ThresholdTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
}

void Td_ThresholdTabWid::initWid()
{
    QStringList header;
    QString title = tr("报警阈值");
    header << tr("项目") << tr("当前值") << tr("最小值") << tr("最大值");
    initTableWid(header, 6, title);
    sDevData *dev = sDataPacket::bulid()->getDev();
    mData = &(dev->data);
}

void Td_ThresholdTabWid::setDataUnit(int id, sDataUnit &unit)
{
    double rate = COM_RATE_CUR;
    QString suffix = "A";
    QString str = tr("电流");
    if(id < 3) {
        rate = 1;
        suffix = "V";
        str = tr("电压");
    }

    for(int i=0; i<unit.size; ++i) {
        QStringList listStr;

        listStr << str;
        listStr << QString::number(unit.value[i]/rate,'f',2)+suffix;
        listStr << QString::number(unit.min[i]/rate,'f',2)+suffix;
        listStr << QString::number(unit.max[i]/rate,'f',2)+suffix;
        setTableRow(id+i, listStr);

        if((unit.value[i] < unit.min[i]) || (unit.value[i] > unit.max[i])) {
            unit.alarm[i] = 2;
            setAlarmBackgroundColor(id+i);
        } else {
            unit.alarm[i] = 0;
            setNormalBackgroundColor(id+i);
        }
    }
}

void Td_ThresholdTabWid::appendItem(sObjData *dev)
{
    setDataUnit(0, dev->vol);
    setDataUnit(3, dev->cur);
}

void Td_ThresholdTabWid::timeoutDone()
{
    clearTable();
    if(mData->size) {
        appendItem(mData);
    }
}
