/*
 *
 *  Created on: 2021年10月1日
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
    groupBox_background_icon(ui->widget);
    set_background_icon(ui->barWid,":/image/title_back.jpg");
    QTimer::singleShot(50,this,SLOT(initFunSlot()));
    mDir = "./Firmware_Build/4.0.0.5-48035/";
    mSize = initWidget();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initFunSlot()
{
    QFile file(mDir+"at91recovery");
    if (file.exists()){
        QString cmd = "cd " + mDir +" \n mkdir -p tmp/ \n"
                      "echo \"123456\" | sudo -S chmod -R 777 ./ \n"
                      "rm -f tmp/* \n ";
        for(int i=0; i<mSize; ++i) {
            cmd += "cp -f at91recovery tmp/at91recovery_" + QString::number(i) + " \n";
        }   system(cmd.toLocal8Bit().data());
    } else {
        this->setDisabled(true);
        QString str = tr(" at91recovery 执行程序未发现\n 软件无法执行。。。");
        QMessageBox::critical(this, tr("错误提示"),str);
    }

}

int MainWindow::initWidget()
{
    int id = 0;
    QObjectList list = ui->widget->children();
    foreach (QObject *obj1, list)
    {
        if (obj1->inherits("QWidget")){
            QWidget *b = qobject_cast<QWidget*>(obj1);
            QString name = b->objectName();
            if(name.contains("widget_")) {
                id = name.remove("widget_").toInt()-1;
                mProgramWid[id] = new ProgramWid(id, b);
                connect(this, &MainWindow::startSig, mProgramWid[id], &ProgramWid::startSlot);
            }
        }
    }
    return id;
}

void MainWindow::on_fnBtn_clicked()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("选择烧录文件"), mDir, "镜像文件(*.img)");
    if(fn.contains(".img")) {
        ui->fnLab->setText(fn.split("/").last());
        QString cmd = "cd " + mDir +" \n echo \"123456\" | sudo -S chmod 777 " + fn +" \n";
        for(int i=0; i<mSize; ++i) cmd += "cp -f " + fn +" tmp/" + QString::number(i) + ".img \n";
        cmd += "sudo chmod -R 777 ./tmp/* \n";
        system(cmd.toLocal8Bit().data());
        ui->widget->setEnabled(true);
    }
}


void MainWindow::on_startBtn_clicked()
{
   int ret = QMessageBox::question(this, tr("信息询问"), tr("请确认进行一键烧录"),
                                   QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
   if(QMessageBox::Ok == ret) {
       emit startSig();
   }
}
