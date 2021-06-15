/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_linetabwid.h"

Td_LineTabWid::Td_LineTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
}

void Td_LineTabWid::initWid()
{
    QStringList header;
    QString title = tr("相数据列表");
    header << tr("开关") << tr("电流") << tr("电压") << tr("有功功率")  << tr("功率因数") << tr("电能");
    initTableWid(header, 3, title);

    sDevData *dev = sDataPacket::bulid()->getDev();
    mData = &(dev->data);
}


void Td_LineTabWid::appendItem(sObjData *dev)
{
    for(int i=0; i<dev->size; ++i) {
        QStringList listStr;

        if(1 == dev->sw[i]) listStr << tr("开"); else listStr << tr("关");
        listStr << QString::number(dev->cur.value[i]/COM_RATE_CUR,'f',2)+"A";
        listStr << QString::number(dev->vol.value[i]/COM_RATE_VOL,'f',1)+"V";
        listStr << QString::number(dev->pow[i]/COM_RATE_POW,'f',3)+"kW";
        listStr << QString::number(dev->pf[i]/COM_RATE_PF,'f',2);
        listStr << QString::number(dev->ele[i]/COM_RATE_ELE,'f',2)+"kWh";
        setTableRow(i, listStr);

        setItemColor(i, 1, dev->cur.status[i]);
        setItemColor(i, 2, dev->vol.status[i]);
        //setItemColor(i, 3, dev->powStatus[i]);
    }
}

void Td_LineTabWid::timeoutDone()
{
    clearTable();
    if(mData->size) {
        appendItem(mData);
    }
}
