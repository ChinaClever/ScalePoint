/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_powtabwid.h"

Td_PowTabWid::Td_PowTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
}

void Td_PowTabWid::initWid()
{
    QStringList header;
    QString title = tr("功率读数列表");
    header << tr("电压") << tr("电流") << tr("有功功率") << tr("无功功率")  << tr("视在功率") << tr("线角");
    initTableWid(header, 3, title);

    sDevData *dev = sDataPacket::bulid()->getDev();
    mData = &(dev->data);
}


QStringList Td_PowTabWid::getItem(sLineData &line)
{
    QStringList listStr;

    if(line.vol_rms) listStr << QString::number(line.vol_rms/COM_RATE_VOL)+"V"; else listStr << "---";
    if(line.cur_rms) listStr << QString::number(line.cur_rms/COM_RATE_CUR)+"A"; else listStr << "---";
    if(line.pow.active) listStr << QString::number(line.pow.active/COM_RATE_POW)+"W"; else listStr << "---";
    if(line.pow.reactive) listStr << QString::number(line.pow.reactive/COM_RATE_POW)+"VAR"; else listStr << "---";
    if(line.pow.apparen) listStr << QString::number(line.pow.apparen/COM_RATE_POW)+"VA"; else listStr << "---";
    if(line.angle) listStr << QString::number(line.angle/COM_RATE_PF); else listStr << "---";

    return listStr;
}


void Td_PowTabWid::appendItem(sDevObj *dev)
{
    for(int i=0; i<LINE_NUM; ++i) {
        QStringList listStr = getItem(dev->lines[i]);
        setTableRow(i, listStr);
    }
}

void Td_PowTabWid::timeoutDone()
{
    appendItem(mData);
}
