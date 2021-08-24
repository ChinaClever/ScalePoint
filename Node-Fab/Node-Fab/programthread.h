#ifndef PROGRAMTHREAD_H
#define PROGRAMTHREAD_H

#include <QObject>
#include <QtCore>

class ProgramThread : public QThread
{
    Q_OBJECT
public:
    explicit ProgramThread(int id, QObject *parent = nullptr);
    ~ProgramThread();

signals:
    void fabSig(bool);

protected:
    void secure_boot_prov();
    bool programFull();
    bool programFab();
    bool checkTime(const QTime &st);

    void workDown();
    void run();

private:
    int mId;
    bool isRun;
    QString mDir;
    QReadWriteLock *mRwLock;
};

#endif // PROGRAMTHREAD_H
