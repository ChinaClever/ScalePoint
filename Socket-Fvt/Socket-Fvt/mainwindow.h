#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "navbarwid.h"
#include "logmainwid.h"
#include "setup_mainwid.h"
#include "home_mainwid.h"
#include "printer_bartender.h"

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
    void initWid();

protected slots:
    void initFunSlot();
    void navBarSlot(int);

private:
    Ui::MainWindow *ui;

    LogMainWid *mLog;
    NavBarWid *mNavBarWid;
    Home_MainWid *mHomeWid;
    Setup_MainWid *mSetupWid;
    Printer_BarTender *mBar;
};
#endif // MAINWINDOW_H
