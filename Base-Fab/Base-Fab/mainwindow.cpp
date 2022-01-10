/*
 *
 *  Created on: 2022年1月1日
 *      Author: Lzy
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"


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
    mCfg = CfgCom::bulid(this);
    mTokens = new BaseTokens(this);
    QString str = mCfg->read("commander").toString();
    ui->cmdEdit->setText(str);

    str = mCfg->read("Bootloader").toString();
    ui->blEdit->setText(str);

    str = mCfg->read("BaseFile").toString();
    ui->fnEdit->setText(str);
}




void MainWindow::argumentsWrite()
{
    mCfg->write("commander", ui->cmdEdit->text());
    mCfg->write("Bootloader", ui->blEdit->text());
    mCfg->write("BaseFile", ui->fnEdit->text());
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
    } while(!ret);
    pro.close();

    return res;
}


bool MainWindow::execute(const QStringList &arguments)
{
    QProcess pro(this);
    pro.start(ui->cmdEdit->text(), arguments);
    return readOutput(pro);
}

bool MainWindow::fabBootloader()
{
    QStringList ls{"flash"};
    ls << "--masserase" << "--device";
    ls << "EFR32MG12P332F1024GM48";
    ls << QString("%1").arg(ui->blEdit->text());

    return execute(ls);
}

bool MainWindow::fabFile()
{
    QStringList ls{"flash"};
    ls << "--device" << "EFR32MG12P332F1024GM48";
    ls << QString("%1").arg(ui->fnEdit->text());

    return execute(ls);
}

bool MainWindow::fabTokens()
{
    sToken *token = mTokens->bulidTokens();
    ui->euiLab->setText("TOKEN_MFG_CUSTOM_EUI_64: " + token->custom_eui);
    ui->codeLab->setText("Install Code: " + token->install_code);

    QStringList ls{"flash"};
    ls << "--device" << "EFR32MG12P332F1024GM48";
    ls << "--tokengroup" << "znet" << "--tokenfile" << TOKEN_FN;
    return execute(ls);
}


QString MainWindow::selectFile(const QString &filter, QString dir)
{
    if(dir.isEmpty()) dir = QCoreApplication::applicationDirPath();
    QString fn = QFileDialog::getOpenFileName(this, tr("选择文件"), dir, filter);
    if(fn.contains(".exe") || fn.contains(".s37")) {

    }

    return fn;
}

void MainWindow::on_cmdBtn_clicked()
{
    QString fn = selectFile("执行文件(*.exe)", ui->cmdEdit->text());
    if(fn.contains(".exe"))  ui->cmdEdit->setText(fn);
}

void MainWindow::on_dlBtn_clicked()
{
    QString fn = selectFile("引导文件(*.s37)", ui->blEdit->text());
    if(fn.contains(".s37")) ui->blEdit->setText(fn);
}

void MainWindow::on_fnBtn_clicked()
{
    QString fn = selectFile("设备固件(*.s37)", ui->fnEdit->text());
    if(fn.contains(".s37")) ui->fnEdit->setText(fn);
}

bool MainWindow::inputCheck()
{
    QString str = ui->cmdEdit->text();
    if(str.isEmpty()) {
        QString str = tr(" commander.exe 烧录程序未指定\n 软件无法执行。。。");
        QMessageBox::critical(this, tr("错误提示"),str); return false;
    }

    str = ui->blEdit->text();
    if(str.isEmpty()) {
        QString str = tr(" Bootloader 文件未指定\n 软件无法执行。。。");
        QMessageBox::critical(this, tr("错误提示"),str); return false;
    }

    str = ui->fnEdit->text();
    if(str.isEmpty()) {
        QString str = tr(" 设备固件(*.s37)未指定\n 软件无法执行。。。");
        QMessageBox::critical(this, tr("错误提示"),str); return false;
    }

    ui->textEdit->clear();
    ui->widget->setEnabled(false);
    ui->codeLab->setText("Install Code:");
    ui->euiLab->setText("TOKEN_MFG_CUSTOM_EUI_64:");
    return true;
}

bool MainWindow::workDown()
{
    bool ret = fabBootloader();
    if(ret) {
        ret = fabFile();
        if(ret) fabTokens();
        argumentsWrite();
    }

    return ret;
}

void MainWindow::on_startBtn_clicked()
{
    bool ret = inputCheck();
    if(ret) {
        int index = ui->modelBox->currentIndex();
        switch (index) {
        case 0: ret = workDown(); break;
        case 1: ret = fabBootloader(); break;
        case 2: ret = fabFile(); break;
        case 3: ret = fabTokens(); break;
        }
    }

    ui->widget->setEnabled(true);
    if(ret) QMessageBox::information(this, tr("信息提示"),tr("烧录完成！"));
    else QMessageBox::critical(this, tr("错误提示"),tr("烧录错误！"));
}

