#include "td_eletabwid.h"

Td_EleTabWid::Td_EleTabWid(QWidget *parent) : ComTableWid(parent)
{
    initWid();
}

void Td_EleTabWid::initWid()
{
    QStringList header;
    QString title = tr("电能读数列表");
    header << tr("电压") << tr("电流") << tr("有功电能") << tr("无功电能")  << tr("视在电能");
    initTableWid(header, 3, title);

    sDevData *dev = sDataPacket::bulid()->getDev();
    mData = &(dev->data);
}


QStringList Td_EleTabWid::getItem(sLineData &line)
{
    QStringList listStr;

    if(line.vol_rms) listStr << QString::number(line.vol_rms/COM_RATE_VOL)+"V"; else listStr << "---";
    if(line.cur_rms) listStr << QString::number(line.cur_rms/COM_RATE_CUR)+"A"; else listStr << "---";
    if(line.ele.active) listStr << QString::number(line.ele.active/COM_RATE_POW)+"Wh"; else listStr << "---";
    if(line.ele.reactive) listStr << QString::number(line.ele.reactive/COM_RATE_POW)+"VARh"; else listStr << "---";
    if(line.ele.apparen) listStr << QString::number(line.ele.apparen/COM_RATE_POW)+"VAh"; else listStr << "---";

    return listStr;
}


void Td_EleTabWid::appendItem(sDevObj *dev)
{
    for(int i=0; i<LINE_NUM; ++i) {
        QStringList listStr = getItem(dev->lines[i]);
        setTableRow(i, listStr);
    }
}

void Td_EleTabWid::timeoutDone()
{
    appendItem(mData);
}
