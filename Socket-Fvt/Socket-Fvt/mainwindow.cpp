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
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
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
    on_addrBtn_clicked(); msleep(500);

    on_openBtn_clicked();
    msleep(ui->spinBox->value()*1000);

    on_closeBtn_clicked();
    msleep(ui->spinBox->value()*1000);

    on_openBtn_clicked(); msleep(100);
}

void MainWindow::on_startBtn_clicked()
{
    bool ret = mCom->isOpened();
    if(ret) {
        ui->startBtn->setText(tr("终 止"));
        workDown();
        ui->startBtn->setText(tr("开 始"));
    } else {
        MsgBox::critical(this, tr("串口未打开，请先打开串口!"));
    }
}
