#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    //QProcess pro(this);
    //pro.startDetached(); ///////=========
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
