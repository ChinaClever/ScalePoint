#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "basetokens.h"

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
    QString selectFile(const QString &filter, QString dir);
    bool execute(const QStringList &arguments);
    void insertText(const QString &str);
    bool readOutput(QProcess &pro);
    void argumentsWrite();
    bool fabBootloader();
    bool inputCheck();
    bool fabFile();
    bool fabTokens();
    bool workDown();

private slots:
    void initFunSlot();
    void on_cmdBtn_clicked();
    void on_dlBtn_clicked();
    void on_fnBtn_clicked();
    void on_startBtn_clicked();

private:
    Ui::MainWindow *ui;
    BaseTokens *mTokens;
    CfgCom *mCfg;
};
#endif // MAINWINDOW_H