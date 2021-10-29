/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "sn_devtype.h"
#include "common/json/json_build.h"
#include "common/json/json_recv.h"

Sn_DevType::Sn_DevType()
{
    QJsonObject json;
    Json_Build::bulid()->getJson(json);
    Json_Recv::bulid()->recv("pdu_id");

    sDevData *dev = sDataPacket::bulid()->getDev();
    mDt = &(dev->dt);
}

Sn_DevType * Sn_DevType::bulid()
{
    static  Sn_DevType* sington = nullptr;
    if(sington == nullptr)
        sington = new  Sn_DevType();
    return sington;
}

QString Sn_DevType::getMpdu(uint id)
{
    QString str;
    if(2 == (id >> 12)) {
        str += "MPDU主控";
        if((id >> 11) & 1) str += "_二期"; else str += "_一期";
        if((id >> 10) & 1) str += "_三相"; else str += "_单相";
        str += QObject::tr("_%1回路").arg((id >> 6) & 0xF);
        str += QObject::tr("_%1输出位").arg(id & 0x3F);
    }
    return  str;
}


QString Sn_DevType::getDevStr(uint id)
{
    QString res = getMpdu(id);
    if(res.isEmpty()) {
        res = Json_Recv::bulid()->getDevTypeByID(id);
    }

    return res;
}



int Sn_DevType::getDevType(const QString &str)
{
    int ret = 0;
    if(str.contains("IMM")) ret = IMM;

    return ret;
}

int Sn_DevType::getLineNum(const QString &str)
{
    int ret = 1;
    if(str.contains("三相")) ret = 3;
    if(str.contains("两路")) ret = 2;

    return ret;
}


int Sn_DevType::getColMode(const QString &str)
{
    int ret = Transformer;
    if(str.contains("锰铜")) ret = Mn;
    //if(mDt->devType > RPDU) ret = Transformer;

    return ret;
}

int Sn_DevType::getSerie(const QString &str)
{
    int ret = 0;
    if(str.contains("A系列")) ret = 1;
    if(str.contains("B系列")) ret = 2;
    if(str.contains("C系列")) ret = 3;
    if(str.contains("B/D系列")) ret = 4;

    return ret;
}

bool Sn_DevType::analysDevType(uint id)
{
    bool ret = true;
    QString str = getDevStr(id);
    if(str.size()) {
        mDt->devId = id;
        mDt->dt = str;
        mDt->devType = getDevType(str);
        mDt->lines = getLineNum(str);
    } else {
        ret = false;
    }

    return ret;
}



