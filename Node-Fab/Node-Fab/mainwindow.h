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
    int initWidget();

private slots:
    void initFunSlot();
    void on_fnBtn_clicked();

private:
    Ui::MainWindow *ui;

    int mSize;
    QString mDir;
    ProgramWid *mProgramWid[16];
};
#endif // MAINWINDOW_H
