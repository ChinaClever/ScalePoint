/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_mainwid.h"
#include "ui_td_mainwid.h"
#include "test_execute.h"

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
     Test_Execute *net = Test_Execute::bulid(this);
     connect(net, SIGNAL(msgSig(QString)), this, SLOT(insertSlot(QString)));
}


void Td_MainWid::startSlot()
{
    ui->textEdit->clear();
}

void Td_MainWid::insertSlot(QString str)
{
    if(str.size()>2) ui->textEdit->appendPlainText(str);
    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textEdit->setTextCursor(cursor);
}
