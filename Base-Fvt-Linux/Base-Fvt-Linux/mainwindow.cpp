/*
 *
 *  Created on: 2022年1月1日
 *      Author: Lzy
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

#define SC_REG "screg"
#define SC_DUMP "scdump"
#define USB_DEV "/dev/ttyUSB0"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    groupBox_background_icon(ui->groupBox);
    QTimer::singleShot(50,this,SLOT(initFunSlot()));
    set_background_icon(ui->barWid,":/image/title_back.jpg");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initFunSlot()
{

}


bool MainWindow::isFileExist(const QString &fn)
{
    bool ret = false; QFileInfo file(fn);
    if (file.exists()) ret =  true;
    return ret;
}

void MainWindow::insertText(const QString &str)
{
    if(str.size()>2) ui->textEdit->appendPlainText(str);
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
}

bool MainWindow::readOutput(QProcess &pro)
{
    bool ret, res = true;
    do {
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        ret = pro.waitForFinished(1000);
        QByteArray bs = pro.readAllStandardOutput();
        bs +=  pro.readAllStandardError();
        QString str = QString::fromLocal8Bit(bs);
        insertText(str); if(str.contains("ERROR")) res = false;
        if(str.contains("error")) res = false;
    } while(!ret);
    pro.close();

    return res;
}

bool MainWindow::execute()
{
    QProcess pro(this);
    QStringList ls{"-i", "/dev/ttyUSB0"};
    pro.start(SC_DUMP, ls);
    return readOutput(pro);
}

bool MainWindow::relayControl(int id, int oc)
{
    QProcess pro(this);
    QStringList ls{"-i", "/dev/ttyUSB0","-w"};
    ls << QString::number(id);
    ls << QString::number(oc);
    pro.start(SC_REG, ls);
    return readOutput(pro);
}

void MainWindow::mdelay(int ms)
{
    QTime t = QTime::currentTime().addMSecs(ms);
    while (QTime::currentTime() < t) {
        QCoreApplication::processEvents(QEventLoop::AllEvents,100);
    }
}

bool MainWindow::relayControl(int id)
{
    bool ret = relayControl(id, 0x03);
    if(ret){
        mdelay(200);
        ret = relayControl(id, 0x02);
        mdelay(500);
    }
    return ret;
}

bool MainWindow::zigbeeConnect()
{
    int id = 0x0097;
    return relayControl(id);
}

bool MainWindow::rsConnect()
{
    int id = 0x0096;
    return relayControl(id);
}

void MainWindow::initWid()
{
    QString str = "--- ---";
    ui->idLab->setText(str);
    ui->fwLab->setText(str);
    ui->snLab->setText(str);
    ui->bootLab->setText(str);
    ui->zigbeeLab->setText(str);
    ui->protocolLab->setText(str);
    ui->textEdit->clear();
}

bool MainWindow::zigbeeCheck()
{
    bool ret = true; int id = 0;
    QString str = ui->textEdit->toPlainText();
    int index = str.indexOf("Dual Zigbee Sensor #0");
    QStringList ls = str.remove(0, index).split("\n");
    if(ls.size() > 6) {
        QString str = ls.at(id+1).split(":").last().simplified();
        if(str.toInt(nullptr, 16) > 0) ui->zigbeeLab->setText(str);
        else {ret = false; mStr = tr("Zigbee设备信息错误");}
    }
    return ret;
}

bool MainWindow::updateWid()
{
    bool ret = false; int id = 0;
    QStringList ls = ui->textEdit->toPlainText().split("\n", QString::SkipEmptyParts);
    if(ls.size() > 6) {
        for(int i=0; i<2; ++i) ls.removeFirst();
        QString str = ls.at(id++).split(":").last().simplified();
        ui->idLab->setText(str);

        str = ls.at(id++).split(":").last();
        ui->fwLab->setText(str);

        str = ls.at(id++).split(":").last();
        ui->bootLab->setText(str);

        str = ls.at(id++).split(":").last();
        ui->protocolLab->setText(str);

        str = ls.at(id+1).split(":").last().simplified().remove(0,2);
        str = QString::number(str.toInt(nullptr, 16)).rightJustified(10, '0');
        ui->snLab->setText(str); ret = zigbeeCheck();
    } else mStr = tr("获取设备信息不全");
    return ret;
}

bool MainWindow::workDown()
{
    initWid();
    bool ret = zigbeeConnect();
    if(ret) ret = execute();
    if(ret) ret = updateWid();
    if(ret) ret = rsConnect();
    return ret;
}

bool MainWindow::inputCheck()
{
    QString str = USB_DEV;
    bool ret = isFileExist(str);
    if(ret) {
        str = "echo \"123456\" | sudo -S chmod 777 " + str;
        system(str.toLatin1().data());
    } else {
        str = tr(" 未接入设备，请连接Base");
        QMessageBox::critical(this, tr("错误提示"),str);
        return false;
    }

    str = SC_DUMP;
    ret = isFileExist(str);
    if(ret) {str =SC_REG; ret = isFileExist(str);}
    if(ret) {
        str = "echo \"123456\" | sudo -S chmod 777 sc*";
        system(str.toLatin1().data());
    } else {
        str = tr(" 文件未接找到") + str;
        QMessageBox::critical(this, tr("错误提示"),str);
        return false;
    }

    return ret;
}

void MainWindow::on_startBtn_clicked()
{
    bool ret = inputCheck();
    if(ret) {
        ret = workDown();
        if(!ret) {
            QMessageBox::critical(this, tr("错误提示"), mStr);
        }
    }
}

