#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProcess>

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
    QString selectFile(const QString &filter);
    void insertText(const QString &str);
    bool readOutput(QProcess &pro);
    bool execute(const QStringList &arguments);
    bool fabBootloader();
    bool fabFile();

private slots:
    void on_cmdBtn_clicked();
    void on_dlBtn_clicked();
    void on_fnBtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
