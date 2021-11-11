/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "printer_bartender.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mNavBarWid = new NavBarWid(ui->barWid);
    QTimer::singleShot(50,this,SLOT(initFunSlot())); //延时初始化
    connect(mNavBarWid, SIGNAL(navBarSig(int)), this, SLOT(navBarSlot(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initFunSlot()
{
    initWid();

    sBarTend it;
    it.pn = "AG13095AA";
    it.hw = "0.1";
    it.fw = "1.0";
    it.sn = 0x78;
    Printer_BarTender::bulid(this)->printer(it);
    qDebug() << "AAAAAAAAA" << QDate::currentDate().toString("yy") + "W" << QDate::currentDate().weekNumber();




}

void MainWindow::initWid()
{
    mHomeWid = new Home_MainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mHomeWid);

    mSetupWid = new Setup_MainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mSetupWid);

    mLog = new LogMainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mLog);
}

void MainWindow::navBarSlot(int id)
{
    ui->stackedWid->setCurrentIndex(id);
}
