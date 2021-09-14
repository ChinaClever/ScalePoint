#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "common.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void workDown();
    bool execute();
    bool sentCmd(const QString &cmd);
    void msleep(int msec);

private slots:
    void on_addrBtn_clicked();
    void on_openBtn_clicked();
    void on_closeBtn_clicked();
    void on_startBtn_clicked();

private:
    Ui::MainWindow *ui;
    SerialPort *mCom;
    bool isRun;
};
#endif // MAINWINDOW_H
