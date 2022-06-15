#ifndef CFGSERIAL_H
#define CFGSERIAL_H

#include "cfgcom.h"
#include "serialstatuswid.h"

struct sSerial
{
    sSerial() {sp=comj7=comj8=nullptr;}
    SerialPort *sp; // 标准源
    SerialPort *comj7; // 串口对象
    SerialPort *comj8; // 串口对象
};

class CfgSerial : public CfgCom
{
public:
    explicit CfgSerial(QObject *parent = nullptr);

    QString getSerialBr(const QString &com);
    QString getSerialName(const QString &key);
    void setSerialBr(const QString &com, const QString &br);
    void setSerialName(const QString &key, const QString &v);

    QString getLoginName();
    void setLoginName(const QString &name);
};

#endif // CFGSERIAL_H
