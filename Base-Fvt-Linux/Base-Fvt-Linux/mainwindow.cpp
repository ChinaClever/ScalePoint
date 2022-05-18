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
        mdelay(800);
        ret = relayControl(id, 0x02);
        mdelay(600);
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

    str = "－－－－";
    QString style = "color:black;" ;
    ui->idResLab->setText(str);
    ui->fwResLab->setText(str);
    ui->bootResLab->setText(str);
    ui->protocolResLab->setText(str);
    ui->snResLab->setText(str);
    ui->socketResLab->setText(str);
    ui->IMMResLab->setText(str);
    ui->zigbeeResLab->setText(str);
    ui->idResLab->setStyleSheet(style);
    ui->fwResLab->setStyleSheet(style);
    ui->bootResLab->setStyleSheet(style);
    ui->protocolResLab->setStyleSheet(style);
    ui->snResLab->setStyleSheet(style);
    ui->socketResLab->setStyleSheet(style);
    ui->IMMResLab->setStyleSheet(style);
    ui->zigbeeResLab->setStyleSheet(style);
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
        changeStatus(ui->zigbeeResLab , ret);
    }
    return ret;
}

void MainWindow::changeStatus(QLabel *lab , bool flag)
{
    QString str ,style;
    if(!flag){
        str = tr("失败"); style = "color:red;";
    } else {
        str = tr("成功"); style = "color:green;";
    }
    lab->setText(str);
    lab->setStyleSheet(style);
}

bool MainWindow::updateWid()
{
    bool ret = false; int id = 0;
    QStringList ls = ui->textEdit->toPlainText().split("\n", QString::SkipEmptyParts);
    if(ls.size() > 6) {
        for(int i=0; i<2; ++i) ls.removeFirst();
        QString str = ls.at(id++).split(":").last().simplified();
        ui->idLab->setText(str);
        changeStatus(ui->idResLab , str != "正在测试中" || str != "－－－－");

        str = ls.at(id++).split(":").last();
        ui->fwLab->setText(str);
        changeStatus(ui->fwResLab , str != "正在测试中" || str != "－－－－");

        str = ls.at(id++).split(":").last();
        ui->bootLab->setText(str);
        changeStatus(ui->bootResLab , str != "正在测试中" || str != "－－－－");

        str = ls.at(id++).split(":").last();
        ui->protocolLab->setText(str);
        changeStatus(ui->protocolResLab , str != "正在测试中" || str != "－－－－");

        str = ls.at(id+1).split(":").last();
        ui->snLab->setText(str);
        changeStatus(ui->snResLab , str != "正在测试中" || str != "－－－－");
        ret = zigbeeCheck();
    } else{
        changeStatus(ui->idResLab , false);
        changeStatus(ui->fwResLab , false);
        changeStatus(ui->bootResLab , false);
        changeStatus(ui->protocolResLab , false);
        changeStatus(ui->snResLab , false);
        changeStatus(ui->socketResLab , false);
        changeStatus(ui->IMMResLab , false);
        changeStatus(ui->zigbeeResLab , false);
        mStr = tr("获取设备信息不全");
    }
    return ret;
}

bool MainWindow::workDown()
{
    initWid();
    bool ret = zigbeeConnect();
    changeStatus(ui->socketResLab , ret);
    changeStatus(ui->IMMResLab , ret);
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

void MainWindow::initStatus()
{
    QString str = tr("正在测试中");
    ui->idResLab->setText(str);
    ui->fwResLab->setText(str);
    ui->bootResLab->setText(str);
    ui->protocolResLab->setText(str);
    ui->snResLab->setText(str);
    ui->socketResLab->setText(str);
    ui->IMMResLab->setText(str);
    ui->zigbeeResLab->setText(str);
}

void MainWindow::on_startBtn_clicked()
{
    bool ret = inputCheck();
    if(ret) {
        initStatus();
        ret = workDown();
        if(!ret) {
            QMessageBox::critical(this, tr("错误提示"), mStr);
        }
    }
}

