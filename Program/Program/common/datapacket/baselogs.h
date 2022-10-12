#ifndef BASELOGS_H
#define BASELOGS_H

#include "dbstates.h"
#include "baseobject.h"

class BaseLogs : public QThread, public BaseObject
{
    Q_OBJECT
    explicit BaseLogs(QObject *parent = nullptr);
public:
    static BaseLogs *bulid(QObject *parent = nullptr);

    void saveLogs();
    void writeMac(const QString &mac){mMac=mac;}
    bool appendLogItem(const QString &str, bool pass);
    void getStep(int step);

protected:
    void run(){saveLogs();}
    bool initItem(sStateItem &item);
    bool writeLog();
    void writeLogs();
    bool writeMac();

private:
    QString mMac;
    QList<sStateItem> mLogItems;
    QString mStep;
};

#endif // BASELOGS_H
