/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_mainwid.h"
#include "ui_td_mainwid.h"
#include "test_network.h"

Td_MainWid::Td_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Td_MainWid)
{
    ui->setupUi(this);
    QTimer::singleShot(1350,this,SLOT(initFunSlot()));
    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
}

Td_MainWid::~Td_MainWid()
{
    delete ui;
}

void Td_MainWid::initFunSlot()
{
    mSerial = Cfg::bulid()->item->deSerial;
    timer = new QTimer(this);
    timer->start(155);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    //if(!mSerial->isOpened()) ui->textEdit->appendPlainText(tr("Debug串口未打开！"));

    Test_NetWork *net = Test_NetWork::bulid(this);
    connect(net, SIGNAL(msgSig(QString)), this, SLOT(insertSlot(QString)));
}

void Td_MainWid::timeoutDone()
{
    if(mSerial->isOpened()) {
        QString str = mSerial->readAll();
        if(str.size()>2) ui->textEdit->appendPlainText(str);
    }
}

void Td_MainWid::startSlot()
{
    ui->textEdit->clear();
}

void Td_MainWid::insertSlot(QString str)
{
    ui->textEdit->appendPlainText(str);
}
