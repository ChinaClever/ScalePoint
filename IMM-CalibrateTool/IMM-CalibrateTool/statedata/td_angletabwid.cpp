/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_angletabwid.h"

Td_AngleTabWid::Td_AngleTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
}

void Td_AngleTabWid::initWid()
{
    QStringList header;
    QString title = tr("相位角状态");
    header << tr("项目") << tr("相位角");
    initTableWid(header, 2, title);

    sDevData *dev = sDataPacket::bulid()->getDev();
    mData = &(dev->data);
}


void Td_AngleTabWid::appendItem()
{
    QStringList listStr;

    listStr << "Line 2 phase angle relative to line 1";
    if(mData->angles[0]) listStr << QString::number(mData->angles[0]/COM_RATE_PF); else listStr << "---";
    setTableRow(0, listStr); listStr.clear();

    listStr << "Line 3 phase angle relative to line 1";
    if(mData->angles[1]) listStr << QString::number(mData->angles[1]/COM_RATE_PF); else listStr << "---";
    setTableRow(1, listStr);
}

void Td_AngleTabWid::timeoutDone()
{
    appendItem();
}
