/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "td_datatabwid.h"

Td_DataTabWid::Td_DataTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
}

void Td_DataTabWid::initWid()
{
    QStringList header;
    QString title = tr("状态列表");
    header<<tr("RMS电压") << tr("RMS电流") << tr("峰值电流") << tr("有功功率") << tr("有功电能") << tr("最终电流") << tr("最终功率")<< tr("结果");
    initTableWid(header, 3, title);

    sDevData *dev = sDataPacket::bulid()->getDev();
    mData = &(dev->data);
}

QStringList Td_DataTabWid::getItem(sLineData &line)
{
    QStringList listStr;

    if(line.vol_rms) listStr << QString::number(line.vol_rms/COM_RATE_VOL)+"V"; else listStr << "---";
    if(line.cur_rms) listStr << QString::number(line.cur_rms/COM_RATE_CUR)+"A"; else listStr << "---";
    if(line.cur_peak) listStr << QString::number(line.cur_peak/COM_RATE_CUR)+"A"; else listStr << "---";
    if(line.pow.active) listStr << QString::number(line.pow.active/COM_RATE_POW)+"W"; else listStr << "---";
    if(line.ele) listStr << QString::number(line.ele/COM_RATE_ELE)+"Wh"; else listStr << "---";
    if(line.cur_ed) listStr << QString::number(line.cur_ed/COM_RATE_CUR)+"A"; else listStr << "---";
    if(line.powed) listStr << QString::number(line.powed/COM_RATE_POW)+"W"; else listStr << "---";

    switch (line.status) {
    case Test_Fail: listStr << "×"; break;
    case Test_Pass: listStr << "√"; break;
    default: listStr << " "; break;
    }

    return listStr;
}

void Td_DataTabWid::appendItem(sDevObj *obj)
{
    for(int i=0; i<LINE_NUM; ++i) {
        QStringList listStr = getItem(obj->lines[i]);
        setTableRow(i, listStr);

        if(obj->lines[i].status == Test_Fail) {
            setAlarmBackgroundColor(i);
        } else {
            setNormalBackgroundColor(i);
        }
    }
}

void Td_DataTabWid::timeoutDone()
{
    appendItem(mData);
}
