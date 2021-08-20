#include "mainwindow.h"
#include <QApplication>
#include "qtsingleapplication.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QtSingleApplication a(QLatin1String("Node-Fab"),argc, argv);
    if(a.isRunning()) {
        QMessageBox::critical(NULL, QObject::tr("提示！"), QObject::tr("Node烧录程序已运行！"));
        a.sendMessage("raise_window_noop", 1000); //4s后激活前个实例
        return EXIT_SUCCESS;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
