/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "pdu_mrtu.h"

Pdu_MRtu::Pdu_MRtu(QObject *parent) : Pdu_MCtrl(parent)
{

}

Pdu_MRtu *Pdu_MRtu::bulid(QObject *parent)
{
    Pdu_MRtu* sington = nullptr;
    if(sington == nullptr)
        sington = new Pdu_MRtu(parent);
    return sington;
}

bool Pdu_MRtu::recvMpduVolCur(uchar *recv, int)
{
    bool ret = false;
    uchar *ptr = recv;
    sPduData *obj = mPduData;

    if(*ptr++ == 0xc1) {
        if(*ptr++ == obj->addr) {
            ptr++;
            int size = obj->size;
            obj->size= 8;
            uchar sw = *ptr++; // 开关状态 1表示开，0表示关
            for(int i=0; i<obj->size; ++i) {
                obj->sw[i] =  (sw >> (7-i)) & 1;
            }

            ptr = toShort(ptr, obj->size, obj->cur.value);
            ushort vol = getShort(ptr); ptr += 2;
            for(int i=0; i<obj->size/2; ++i) obj->vol.value[i] = vol;
            vol = getShort(ptr); ptr += 2;
            for(int i=obj->size/2; i<obj->size; ++i) obj->vol.value[i] = vol;
            ptr = toShort(ptr, obj->size, obj->pow);

            for(int i=0; i<obj->size; ++i) {
                obj->pow[i] *= obj->rate; //功率乘以倍率10
            }

            obj->size = size;
            if(!obj->size) obj->size = 8;
            obj->vol.size = obj->cur.size = obj->size;
            ret = true;
        } else {
            qDebug() << "AdjustCoreThread recvMpduVolCur addr err!";
        }
    } else {
        qDebug() << "AdjustCoreThread recvMpduVolCur err!";
    }

    return ret;
}

bool Pdu_MRtu::getMpduVolCur()
{
    bool res = false;
    static uchar recv[256] = {0};
    static uchar cmd[] = {0x7B, 0xC1, 0x01, 0x15, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0xCC};

    cmd[2] = mPduData->addr;
    cmd[20] = mModbus->xorNum(cmd,sizeof(cmd)-1);
    int ret = mModbus->transmit(cmd, sizeof(cmd), recv, 2);
    if(ret > 0) {
        res = recvMpduVolCur(recv, ret);
    } else {
        qDebug() << "AdjustCoreThread getMpduVolCur err!";
    }

    return res;
}

bool Pdu_MRtu::recvMpduEle(uchar *recv, int len)
{
    bool ret = true;
    uchar *ptr = recv;
    if(*ptr++ == 0xE1){
        if((0x03&(*ptr++)) == mPduData->addr) {
            ptr = toOutputEle(ptr, mPduData->size, mPduData->ele);
        } else {
            ret = false; qDebug() << "AdjustCoreThread recvMpduEle addr err!" << len << *ptr ;
        }
    } else if(recv[0] == 0xC1){
        recvMpduVolCur(recv, len);
    } else {
        qDebug() << "AdjustCoreThread recvMpduEle res err!" << len << *ptr ;
        QByteArray array;
        array.append((char *)recv, len);
        qDebug() << cm_ByteArrayToHexStr(array);
    }
    return ret;
}

int Pdu_MRtu::getMpduEle()
{
    static uchar recv[256] = {0};
    static uchar cmd[] = {0x7B, 0xE1, 0x01, 0x10, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0xCC};

    cmd[2] = mPduData->addr;
    cmd[15] = mModbus->xorNum(cmd,sizeof(cmd)-1);
    int ret = mModbus->transmit(cmd, sizeof(cmd), recv, 2);
    if(ret > 0) {
        recvMpduEle(recv, ret);
    } else {
        qDebug() << "AdjustCoreThread getMpduVolCur err!";
    }
    return ret;
}

bool Pdu_MRtu::readPduData()
{
    mPduData->rate = 10;
    bool ret = getMpduVolCur();
    if(ret) {
        msleep(240);
        ret = getMpduEle();
    }
    return ret;
}
