/*
 *
 *  Created on: 2022年1月1日
 *      Author: Lzy
 */
#include "basetokens.h"
#include <QDateTime>
#include <QRandomGenerator64>

BaseTokens::BaseTokens(QObject *parent) : QObject(parent)
{
    initFun();
}

void BaseTokens::initFun()
{
    QString g = "Tokens";
    mToken = new sToken(); mCfg = Cfg::bulid(this);
    mToken->board_name = mCfg->read("BOARD_NAME","", g).toString();
    mToken->manuf_id = mCfg->read("MANUF_ID", "0x2110", g).toString();
    mToken->CUSTOM_EUI = mCfg->read("CUSTOM_EUI", 1, g).toUInt();
}

void BaseTokens::argumentsWrite()
{
    QString g = "Tokens";
    mCfg->write("BOARD_NAME", mToken->board_name, g);
    mCfg->write("MANUF_ID", mToken->manuf_id, g);
    mCfg->write("CUSTOM_EUI", mToken->CUSTOM_EUI, g);
}

void BaseTokens::writeFile()
{
    QFile file(TOKEN_FN);
    file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text);
    QString txt = "TOKEN_MFG_BOARD_NAME: \"" + mToken->board_name + "\"\n"
                  "TOKEN_MFG_MANUF_ID: " + mToken->manuf_id + "\n"
                  "TOKEN_MFG_CUSTOM_EUI_64: " + mToken->custom_eui + "\n"
                  "Install Code: " + mToken->install_code + "\n";
    file.write(txt.toUtf8());
    file.close();
}

QString BaseTokens::addCustomEui()
{
    return "000474" + QString::number(mToken->CUSTOM_EUI++,16).rightJustified(10, '0');
}


BaseTokens *BaseTokens::bulid(QObject *parent)
{
    static BaseTokens* sington = nullptr;
    if(sington == nullptr)
        sington = new BaseTokens(parent);
    return sington;
}

QString BaseTokens::createInstallCode()
{
    QRandomGenerator64 generator((unsigned)QDateTime::currentMSecsSinceEpoch());
    qint64 value = generator.generate() & std::numeric_limits<qint64>::max();
    QString OOBkey = QString::number(value, 16).rightJustified(16, '0');
    value = generator.generate() & std::numeric_limits<qint64>::max();
    OOBkey += QString::number(value, 16).rightJustified(16, '0');

    QByteArray ar = QByteArray::fromHex(OOBkey.toUtf8());
    quint16 crcBytes = qChecksum(ar.data(), ar.length());
    quint16 crc = (crcBytes << 8) + (crcBytes >> 8);
    mToken->crc_code = OOBkey + QString::number(crc, 16).rightJustified(4, '0');

    return OOBkey.toUpper();
}

sToken *BaseTokens::bulidTokens()
{
    mToken->custom_eui = addCustomEui();
    mToken->install_code = createInstallCode();
    writeFile(); argumentsWrite();

    return mToken;
}
