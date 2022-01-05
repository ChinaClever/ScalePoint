/*
 *
 *  Created on: 2022年1月1日
 *      Author: Lzy
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "backcolourcom.h"
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    groupBox_background_icon(ui->groupBox);
    set_background_icon(ui->barWid,":/image/title_back.jpg");
}

MainWindow::~MainWindow()
{
    delete ui;
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
    ls << "\"EFR32MG12P332F1024GM48\"";
    ls << QString("\"%1\"").arg(ui->blEdit->text());

    return execute(ls);
}

bool MainWindow::fabFile()
{
    QStringList ls{"flash"};
    ls << "--device" << "\"EFR32MG12P332F1024GM48\"";
    ls << QString("\"%1\"").arg(ui->fnEdit->text());

    return execute(ls);
}


QString MainWindow::selectFile(const QString &filter)
{
    QString dir = QCoreApplication::applicationDirPath();
    QString fn = QFileDialog::getOpenFileName(this, tr("选择文件"), dir, filter);
    if(fn.contains(".exe") || fn.contains(".s37")) {

    }

    return fn;
}

void MainWindow::on_cmdBtn_clicked()
{
    QString fn = selectFile("执行文件(*.exe)");
    if(fn.contains(".exe")){
        ui->cmdEdit->setText(fn);
    }
}

void MainWindow::on_dlBtn_clicked()
{
    QString fn = selectFile("引导文件(*.s37)");
    if(fn.contains(".s37")) {
        ui->blEdit->setText(fn);
    }
}

void MainWindow::on_fnBtn_clicked()
{
    QString fn = selectFile("设备固件(*.s37)");
    if(fn.contains(".s37")) {
        ui->fnEdit->setText(fn);
    }
}



















