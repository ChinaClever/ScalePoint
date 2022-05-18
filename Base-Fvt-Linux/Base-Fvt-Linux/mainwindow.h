#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>
#include "cfgcom.h"
#include <QFileDialog>
#include <QMessageBox>
#include "backcolourcom.h"

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
    bool isFileExist(const QString &fn);
    void insertText(const QString &str);
    bool readOutput(QProcess &pro);
    bool relayControl(int id, int oc);
    bool relayControl(int id);
    bool zigbeeConnect();
    bool rsConnect();

    bool inputCheck();
    void initWid();
    bool updateWid();
    bool zigbeeCheck();

    bool execute();
    bool workDown();
    void mdelay(int ms);
    void changeStatus(QLabel *lab, bool flag);

private slots:
    void initFunSlot();
    void on_startBtn_clicked();
    void initStatus();

private:
    Ui::MainWindow *ui;
    QString mStr;
};
#endif // MAINWINDOW_H
