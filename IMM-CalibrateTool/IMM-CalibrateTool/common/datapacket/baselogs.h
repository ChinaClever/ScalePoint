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
    ~BaseLogs();

    void saveLogs();
    bool appendLogItem(const QString &str, bool pass);

protected:
    void run(){saveLogs();}
    bool initItem(sStateItem &item);
    bool writeLog();
    void writeLogs();

private:
    QString mMac;
    QList<sStateItem> mLogItems;
};

#endif // BASELOGS_H
