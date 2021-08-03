#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    groupBox_background_icon(ui->widget);
    set_background_icon(ui->barWid,":/image/title_back.jpg");
    SerialStatusWid *com = new SerialStatusWid(ui->comBox);
    mCom = com->initSerialPort(tr("PDU"));
    isRun = false;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::msleep(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while(QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
        if(isRun) continue; else break;
    }
}

bool MainWindow::sentCmd(const QString &cmd)
{
    bool ok = mCom->isOpened();
    if(ok) {
        QByteArray bytes = cm_HexStringToByteArray(cmd, &ok);
        if(ok) mCom->write(bytes);
        else MsgBox::critical(this, tr("命令格式错误，请重新填写!"));
    } else {
        MsgBox::critical(this, tr("串口未打开，请先打开串口!"));
    }

    return ok;
}

void MainWindow::on_addrBtn_clicked()
{
    QString str = ui->addrEdit->text();
    sentCmd(str);
}

void MainWindow::on_openBtn_clicked()
{
    QString str = ui->openEdit->text();
    sentCmd(str);
}

void MainWindow::on_closeBtn_clicked()
{
    QString str = ui->closeEdit->text();
    sentCmd(str);
}

void MainWindow::workDown()
{
    int t = ui->spinBox->value()*1000;
    QString str = ui->addrEdit->text();
    isRun = sentCmd(str);
    if(isRun) msleep(500); else return;

    str = ui->openEdit->text();
    isRun = sentCmd(str);
    if(isRun) msleep(t); else return;

    str = ui->closeEdit->text();
    isRun = sentCmd(str);
    if(isRun) msleep(t); else return;

    on_openBtn_clicked();
    isRun = false;
}

void MainWindow::on_startBtn_clicked()
{
    if(isRun) {isRun = false; return;}
    bool ret = mCom->isOpened();
    if(ret) {
        ui->startBtn->setText(tr("终 止"));
        workDown();
        ui->startBtn->setText(tr("开 始"));
    } else {
        MsgBox::critical(this, tr("串口未打开，请先打开串口!"));
    }
}
