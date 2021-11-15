/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "td_mainwid.h"
#include "ui_td_mainwid.h"

Td_MainWid::Td_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Td_MainWid)
{
    ui->setupUi(this);
    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
    initWid();
}

Td_MainWid::~Td_MainWid()
{
    delete ui;
}

void Td_MainWid::initWid()
{
    mTableWid = new Td_DataTabWid(ui->tabWidget);
    ui->tabWidget->addTab(mTableWid, tr("校准读数列表"));

    mPowTabWid = new Td_PowTabWid(ui->tabWidget);
    ui->tabWidget->addTab(mPowTabWid, tr("功率读数列表"));

    mBranchWid = new Td_BranchTabWid(ui->tabWidget);
    ui->tabWidget->addTab(mBranchWid, tr("支路读数列表"));

    mEleTabWid = new Td_EleTabWid(ui->tabWidget);
    ui->tabWidget->addTab(mEleTabWid, tr("电能读数列表"));

    mAngleWid = new Td_AngleTabWid(ui->tabWidget);
    ui->tabWidget->addTab(mAngleWid, tr("相位角状态"));
}
