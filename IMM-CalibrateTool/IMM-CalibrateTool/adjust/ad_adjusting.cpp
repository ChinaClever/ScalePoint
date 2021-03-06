/*
 *
 *  Created on: 2020年10月1日
 *      Author: Lzy
 */
#include "ad_adjusting.h"

Ad_Adjusting::Ad_Adjusting(QObject *parent) : BaseThread(parent)
{
}

Ad_Adjusting *Ad_Adjusting::bulid(QObject *parent)
{
    static Ad_Adjusting* sington = nullptr;
    if(sington == nullptr)
        sington = new Ad_Adjusting(parent);
    return sington;
}

bool Ad_Adjusting::transmit(uchar *buf, int len)
{
    bool ret = true;
    uchar recv[64] = {0};
    len = mModbus->transmit(buf, len, recv, 3);
    if(len > 0) {
        ret = recvStatus(recv, len);
    }

    return ret;
}

bool Ad_Adjusting::writeCmd(uchar fn)
{
    uchar cmd[] = {0x5E, 0x01, 0x00, 0xA0, 0xBB, 0xBB};
    int len = sizeof(cmd);

    cmd[1] = mDt->addr;
    cmd[3] = fn;

    ushort crc = mModbus->CRC16(cmd, len-2);
    cmd[len-1] = ((0xff) & crc);
    cmd[len-2] = (crc >> 8);

    return transmit(cmd, len);
}

bool Ad_Adjusting::waitDcRecv()
{
    bool ret = false;
    uchar buf[MODBUS_RTU_SIZE] = {0};
    updatePro(tr("正在等待直流偏移：请等待..."));

    int len = readSerial(buf, 70);
    if(len > 0){
        ret = recvStatus(buf, len);
    } else {
        ret = overWork(tr("直流偏移等待超时！"));
    }

    return ret;
}


bool Ad_Adjusting::sentCmd()
{
    updatePro(tr("发送校准解锁命令！"));
    bool ret = writeCmd(0xA0);
    if(!ret){
        delay(5); ret = writeCmd(0xA0);  // 重复发一次命令
        if(!ret) return ret;
    } else msleep(200);

    if(ret) {ret = writeCmd(0xA1); updatePro(tr("ADC校准启动！"),ret, 3);}
    if(ret) {ret = writeCmd(0xA2); updatePro(tr("增益校准启动！"), ret);}

    return ret;
}

bool Ad_Adjusting::updateStatus(ushort status)
{
    QString str;
    if(0x1100 == status) {
        uchar step = Test_vert;
        str = tr("校准返回正常！");
        if(mItem->aiMode) step = Test_Over;
        mPro->step = step;
    } else if(0x1101 == status) {
        str = tr("校准失败");
        mPro->step = Test_vert;
    } else if(0x1102 == status) {
        str = tr("校准解锁成功");
    } else if(0x1108 == status) {
        str = tr("直流偏移校准成功");
    }else if(0x1109 == status) {
        str = tr("直流偏移校准失败");
    }else if(0x110A == status) {
        str = tr("直流正在校准");
    }else if(0x110B == status) {
        str = tr("直流电流校准失败");
    }else if(0x110C == status) {
        str = tr("直流电压校准失败");
    }else if(status <= 0x1115) {
        if(status%3 == 0) {
            str = tr("L%1相， 正在校准").arg((status-0x110D)/3+1);
        } else if(status%3 == 1) {
            str = tr("L%1相， 校准成功").arg((status-0x110D)/3+1);
        } else if(status%3 == 2) {
            str = tr("L%1相， 校准失败").arg((status-0x110D)/3+1);
        }
    } else if(status <= 0x1118) {
        str = tr("校准失败：L%1相电流 ").arg(status-0x1115);
    } else if(status <= 0x111C) {
        str = tr("校准失败：L%1相电压 ").arg(status-0x1119);
    }else if(status <= 0x112F) {
        str = tr("校准完成，输出位%1 ").arg(status-0x1120);
    } else if(status <= 0x114F) {
        str = tr("电流校准失败：输出位%1").arg(status-0x1140);
    } else if(status <= 0x116F) {
        str = tr("电压校准失败：输出位%1").arg(status-0x1160);
    } else {
        str = tr("校准失败：状态返回错误%1 ").arg(QString::number(status, 16));
    }

    return updatePro(str);
}

bool Ad_Adjusting::recvStatus(uchar *recv, int len)
{
    bool ret = true;
    //qDebug() << cm_ByteArrayToHexStr(recv, len);
    if((recv[0] == 0xFF) || (recv[3] > 0xA0)){return ret;} // recv += 6; len -= 6;
    else if(len > 6) {recv += 6; len -= 6;}
    if((len>0) && (len%6 == 0)) {
        for(int i = 0; i < len; i+=6) {
            ushort status = recv[i+2]*256 + recv[i+3];
            ret = updateStatus(status);
        }
    } else if(len > 0){
        //ret = false;
        qDebug() << "Adjust res len err" << len;
    }

    return ret;
}

int Ad_Adjusting::readSerial(uchar *recv, int sec)
{
    uchar *ptr = nullptr;
    uchar buf[MODBUS_RTU_SIZE] = {0};

    int len = mModbus->readSerial(buf, sec);
    if(len > 0){
        if(len > 6) {
            ptr = &(buf[len-6]);
            len = 6;
        } else {
            ptr = buf;
        }

        for(int i=0; i<len; ++i) {
            recv[i] = ptr[i];
        }
    }

    return len;
}

bool Ad_Adjusting::overWork(const QString &str)
{
    mPro->step = Test_End;
    return updatePro(str, false, 0);
}

bool Ad_Adjusting::readData()
{
    bool ret = false;
    uchar buf[MODBUS_RTU_SIZE] = {0};
    updatePro(tr("正在校准：请等待..."));

    do {
        int len = readSerial(buf);
        if(len > 0){
            ret = recvStatus(buf, len);
        } else {
            ret = overWork(tr("校准超时！")); break;
        }
        if(mPro->step >= Test_vert) break;
    } while(true == ret);

    return ret;
}

bool Ad_Adjusting::startAdjust()
{
    mPro->step = Test_Ading;
    bool ret = sentCmd();
    if(ret && (mPro->step == Test_Ading)){
        ret = readData();
        if(mPro->step == Test_vert) {
            //updatePro(tr("等待设备重启"), ret, 3);
            //Yc_Obj::bulid()->get()->setVol(0, 0);
        }
    }

    return ret;
}
