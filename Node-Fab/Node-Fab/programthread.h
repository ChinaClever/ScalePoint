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
    void fabSig(int);

protected:
    void workDown();
    void run();

private:
    int mId;
    bool isRun;
};

#endif // PROGRAMTHREAD_H
