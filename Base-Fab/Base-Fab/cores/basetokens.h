#ifndef BASETOKENS_H
#define BASETOKENS_H

#include <QObject>
#include <QProcess>
#include "config.h"
#include <QFileDialog>
#include <QMessageBox>
#include "backcolourcom.h"

#define  TOKEN_FN   "mfg-tokens-Efr.txt"

struct sToken
{
    QString board_name;
    QString manuf_id;
    QString custom_eui;
    QString install_code;
    QString crc_code;
    unsigned long long CUSTOM_EUI;
};

class BaseTokens : public QObject
{
    Q_OBJECT
    explicit BaseTokens(QObject *parent = nullptr);
public:
    static BaseTokens *bulid(QObject *parent = nullptr);
    QString codeCrc() {return mToken->crc_code;}
    sToken *bulidTokens();

protected:
    void initFun();
    void writeFile();
    void argumentsWrite();
    QString addCustomEui();
    QString createInstallCode();

private:
    CfgCom *mCfg;
    sToken *mToken;
};

#endif // BASETOKENS_H
