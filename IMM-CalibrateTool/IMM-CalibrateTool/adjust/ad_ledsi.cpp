/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "ad_ledsi.h"

Ad_LedSi::Ad_LedSi(QObject *parent) : BaseThread(parent)
{
    mRtu = Dev_SiRtu::bulid();
}

Ad_LedSi *Ad_LedSi::bulid(QObject *parent)
{
    static Ad_LedSi* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_LedSi(parent);
    return sington;
}

bool Ad_LedSi::transmit(uchar *buf, int len)
{
    bool ret = false;
    uchar recv[64] = {0};
    len = mModbus->transmit(buf, len, recv, 20);
    if(len > 0) {
        if(recv[1] == buf[1]) ret = true;
    } else {
        qDebug() << "Ad_LedSi err " << len;
    }

    return ret;
}

bool Ad_LedSi::writeCmd(uchar fn, uchar line)
{
    uchar cmd[] = {0x7B, 0xA0, 0x00, 0x00, 0x00, 0xBB, 0xBB};
    int len = sizeof(cmd);

    cmd[1] = fn;
    cmd[2] = line;
    uchar crc = mModbus->xorNum(cmd, len-1);
    cmd[len-1] = crc;

    return transmit(cmd, len);
}


bool Ad_LedSi::writeDc()
{
    bool ret = true;
    for(int i=0; i<3; ++i) {
        updatePro(tr("发送直流偏移命令 %1").arg(i+1));
        ret = writeCmd(0xA2, 0); delay(5);
        ret = mRtu->readPduData(); if(!ret) ret = mRtu->readPduData();
        if(mData->cur.value[0]) ret = false; else break; delay(5);
    }

    if(!ret) return ret;
    updatePro(tr("设置标准源电流6A"));
    ret = YC_Dc107::bulid()->setCur(60, 10);
    if(ret) ret = writeCmd(0xA1, 0);

    return ret;
}

bool Ad_LedSi::writeAc()
{
    bool res = true;
    int line = mDt->lines;
    updatePro(tr("正在校准：请等待..."), res, 8);
    for(int i=0; i<line; ++i) {
        bool ret = writeCmd(0xA1, i);
        QString str = tr("L%1 校准").arg(i+1);
        if(ret) str += tr("正常"); else {str += tr("错误"); res = false;}
        updatePro(str, ret, 5);
    }

    return res;
}


bool Ad_LedSi::sentCmd()
{
    bool ret = true;
    if(DC == mDt->ac) {
        ret = writeDc();
    } else {
        ret = writeAc();
    }

    return ret;
}

bool Ad_LedSi::startAdjust()
{
    bool ret = sentCmd();
    if(ret) {
        if(mItem->aiMode) mPro->step = Test_Over;
        else mPro->step = Test_vert;
    } else {
        mPro->step = Test_vert;
    }

    return updatePro(tr("校准返回！"), ret, 1);
}
