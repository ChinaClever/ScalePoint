#ifndef CFGCOM_H
#define CFGCOM_H
#include <QtCore>

class CfgCom
{
public:
    CfgCom(QObject *parent = nullptr);
    QString pathOfData(const QString& name);
    void write(const QString &key, const QVariant& v, const QString &g="Cfg");
    QVariant read(const QString &key, const QVariant &v = QVariant(), const QString &g="Cfg");

protected:
    bool cfgOpen(QObject *parent = nullptr, const QString& fn = "cfg.ini");

private:
    QSettings  *mCfgIni; //*mCfgIni;
};

#endif // CFGCOM_H
