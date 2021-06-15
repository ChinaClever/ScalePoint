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
    ui->tabWidget->addTab(mTableWid, tr("校准数据列表"));

    mLineTabWid = new Td_LineTabWid(ui->tabWidget);
    ui->tabWidget->addTab(mLineTabWid, tr("相数据列表"));

    mThTabWid = new Td_ThresholdTabWid(ui->tabWidget);
    ui->tabWidget->addTab(mThTabWid, tr("报警阈值信息"));

    mEnvTabWid = new Td_EnvTabWid(ui->tabWidget);
    ui->tabWidget->addTab(mEnvTabWid, tr("传感器状态"));
}
