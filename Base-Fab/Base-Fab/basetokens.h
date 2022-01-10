#ifndef BASETOKENS_H
#define BASETOKENS_H

#include <QObject>
#include <QProcess>
#include "cfgcom.h"
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
    uint CUSTOM_EUI;
};

class BaseTokens : public QObject
{
    Q_OBJECT
public:
    explicit BaseTokens(QObject *parent = nullptr);
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
