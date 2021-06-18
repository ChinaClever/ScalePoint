/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_branchtabwid.h"

Td_BranchTabWid::Td_BranchTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
}

void Td_BranchTabWid::initWid()
{
    QStringList header;
    QString title = tr("支路读取列表");
    header << tr("项目") << tr("RMS电流") << tr("峰值电流");
    initTableWid(header, 7, title);

    sDevData *dev = sDataPacket::bulid()->getDev();
    mData = &(dev->data);
}


void Td_BranchTabWid::getItem(int id, sBranchIt &it)
{
    QStringList listStr;
    if(id < OpSize) {
        listStr << "Branch " + QString::number(id+1);
    } else {
        listStr << "Neutral";
    }

    if(it.cur_rms) listStr << QString::number(it.cur_rms/COM_RATE_CUR)+"A"; else listStr << "---";
    if(it.cur_peak) listStr << QString::number(it.cur_peak/COM_RATE_CUR)+"A"; else listStr << "---";
    setTableRow(id, listStr);
}


void Td_BranchTabWid::appendItem(sDevObj *dev)
{
    for(int i=0; i<OpSize; ++i) {
        getItem(i, dev->branchs[i]);
    }

    getItem(OpSize, dev->neutral);
}

void Td_BranchTabWid::timeoutDone()
{
    appendItem(mData);
}
