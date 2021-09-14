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
    void proSig(int, int);

protected:
    bool programFull();
    void updatePro(QString &str);
    bool readOutput(QProcess &pro);
    void workDown();
    void run();

private:
    int mId;
    bool isRun;
    QString mDir;
};

#endif // PROGRAMTHREAD_H
