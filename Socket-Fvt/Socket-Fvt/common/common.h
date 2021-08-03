#ifndef COMMON_H
#define COMMON_H
#include "msgbox.h"
#include "config.h"
#include "datapacket.h"

bool cm_pingNet(const QString& ip);
bool cm_isDigitStr(const QString &src);
bool cm_isIPaddress(const QString& ip);

QString cm_ByteArrayToHexStr(const QByteArray &array);
QString cm_ByteArrayToUcharStr(const QByteArray &array);
QString cm_ByteArrayToHexString(QByteArray ascii); //字符串转16进制
QByteArray cm_HexStringToByteArray(QString hex, bool *ok);

#endif // COMMON_H
