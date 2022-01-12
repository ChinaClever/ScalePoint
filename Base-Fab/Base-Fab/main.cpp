/*
 *
 *  Created on: 2022年1月1日
 *      Author: Lzy
 */
#include "msgbox.h"
#include "mainwindow.h"
#include <QApplication>
#include "qtsingleapplication.h"

int main(int argc, char *argv[])
{
    QtSingleApplication a(QLatin1String("Base-Fvt"),argc, argv);
    if(a.isRunning()) {
        MsgBox::critical(NULL, QObject::tr("Base 测试程序已运行！"));
        a.sendMessage("raise_window_noop", 1000); //4s后激活前个实例
        return EXIT_SUCCESS;
    }
    MainWindow w;
    w.show();
    return a.exec();
}
