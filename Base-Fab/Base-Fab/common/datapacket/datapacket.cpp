/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "datapacket.h"
#include "config.h"

sDataPacket::sDataPacket()
{
    dev = new sDevData;
    pro = new sProgress();
}


sDataPacket *sDataPacket::bulid()
{
    static sDataPacket* sington = nullptr;
    if(sington == nullptr)
        sington = new sDataPacket();
    return sington;
}

void sDataPacket::init()
{
    pro->step = 0;
    pro->result = 0;
    pro->pass.clear();
    pro->status.clear();
    dev->dt.sn.clear();

    dev->dt.sn.clear();
    dev->dt.fw.clear();
    dev->dt.code.clear();
    pro->result = Test_Info;
    pro->startTime = QTime::currentTime();
}
