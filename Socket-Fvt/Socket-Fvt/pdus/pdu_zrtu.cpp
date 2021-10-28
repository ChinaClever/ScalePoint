/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "pdu_zrtu.h"

Pdu_ZRtu::Pdu_ZRtu(QObject *parent) : Pdu_ZCtrl(parent)
{

}


Pdu_ZRtu *Pdu_ZRtu::bulid(QObject *parent)
{
    Pdu_ZRtu* sington = nullptr;
    if(sington == nullptr)
        sington = new Pdu_ZRtu(parent);
    return sington;
}

bool Pdu_ZRtu::recvZpduVolCur(uchar *recv, int len)
{
    int op = 14;
    bool ret = false;
    uchar *ptr = recv;
    sPduData *obj = mPduData;

    if((*ptr++ == 0x7B) && (*ptr++ == 0xC1) && (len == 127)) {
        if(*ptr++ == mItem->addr) {
            obj->size = *ptr++;
            obj->hz = *ptr++;

            obj->vol.value[0] = getShort(ptr); ptr += 2;
            if( obj->size == 4 ){//ZPDU四位没有第二个电压
                obj->vol.value[obj->size-1] = obj->vol.value[0]; ptr += 2;
            }
            else {
                obj->vol.value[obj->size-1] = getShort(ptr); ptr += 2;
            }
            ushort sw = getShort(ptr); ptr += 2; // 开关状态 1表示开，0表示关
            for(int i=0; i<op; ++i)  obj->sw[i] = (sw >> (15-i)) & 1;

            ptr = toShort(ptr, op, obj->cur.value);
            ptr = toChar(ptr, op, obj->pf);
            ptr = toOutputEle(ptr, op, obj->ele);

            ptr += 3; //忽略三位97
            obj->version = *ptr++;
            obj->chipStatus = *ptr++; // 01表示执行版计量芯片模块损坏，00表示正常。
            ptr++;

            for(int i=1; i<obj->size-1; ++i) {
                obj->vol.value[i] = getShort(ptr); ptr += 2;
            }

            for(int i=0; i<op; ++i) {
                uint pow = obj->vol.value[i] * obj->cur.value[i] * obj->pf[i];
                obj->pow[i] = pow / (10*10*100);
            }

            obj->vol.size = obj->cur.size = obj->size;
            ret = true;
        }
    } else {
        qDebug() << "recvZpduVolCur err" << len << cm_ByteArrayToHexStr(QByteArray((char *)recv, len));
    }

    return ret;
}

bool Pdu_ZRtu::getZpduVolCur()
{
    bool res = false;
    int k = 6;
    static uchar recv[256] = {0};
    static uchar cmd[68] = {0x7B, 0xC1, 0x01, 0xA1, 0xB1, 0x01};

    cmd[2] = mPduData->addr; // mItem->addr;
    for(int i=1; i<61; i++) cmd[k++] = 0x00;
    cmd[k++] = 0x44;  //长度
    cmd[k] = mModbus->xorNum(cmd,sizeof(cmd)-1);
    int ret = mModbus->transmit(cmd, sizeof(cmd), recv, 2);
    if(ret > 0) {
        res = recvZpduVolCur(recv, ret);
    } else {
        qDebug() << "Col_ZPduThread getZpduVolCur err!" << res;
    }

    return res;
}

bool Pdu_ZRtu::readPduData()
{
    mPduData->rate = 10;
    return getZpduVolCur();
}
