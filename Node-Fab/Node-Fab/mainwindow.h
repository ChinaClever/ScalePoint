#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "programwid.h"

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
    void initWidget();

private slots:
    void initFunSlot();
    void on_fnBtn_clicked();

private:
    Ui::MainWindow *ui;
    ProgramWid *mProgramWid[6];
};
#endif // MAINWINDOW_H
