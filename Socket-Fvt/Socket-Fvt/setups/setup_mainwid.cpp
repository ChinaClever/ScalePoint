/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "setup_mainwid.h"
#include "ui_setup_mainwid.h"
#include "versiondlg.h"
#include "macaddr.h"
#include "dblogs.h"

Setup_MainWid::Setup_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setup_MainWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    QTimer::singleShot(rand()%13,this,SLOT(initFunSlot()));
    mItem = Cfg::bulid()->item;
    initSerial();
    ui->groupBox_4->setHidden(true);
}

Setup_MainWid::~Setup_MainWid()
{
    delete ui;
}

void Setup_MainWid::initFunSlot()
{
    initPcNum();
    initLogCount();
    mUserWid = new UserMainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mUserWid);
    QTimer::singleShot(2*1000,this,SLOT(checkPcNumSlot()));

    initMac();
    timer = new QTimer(this);
    timer->start(3*1000);
    connect(timer, SIGNAL(timeout()),this, SLOT(timeoutDone()));
    DbLogs *db = DbLogs::bulid();
    connect(db, SIGNAL(itemChanged(int, int)),this, SLOT(updateSlot(int,int)));

    QDate buildDate = QLocale(QLocale::English ).toDate( QString(__DATE__).replace("  ", " 0"), "MMM dd yyyy");
    ui->label_date->setText(buildDate.toString("yyyy-MM-dd"));
}


void Setup_MainWid::checkPcNumSlot()
{
    int num = mItem->pcNum;
    if(num < 1) {
        if(!usr_land_jur())
            MsgBox::warning(this, tr("请联系研发部设定电脑号！\n 服务设置 -> 设置功能 \n 需要管理员权限!"));
        else
            MsgBox::warning(this, tr("请自行设定电脑号！\n 服务设置 -> 设置功能 \n 需要管理员权限!"));
        QTimer::singleShot(20*1000,this,SLOT(checkPcNumSlot()));
    }
}

void Setup_MainWid::initSerial()
{
    mComWid = new SerialStatusWid(ui->comWid);
    mItem->coms.sp = mComWid->initSerialPort(tr("PDU"));
}

void Setup_MainWid::initLogCount()
{
    Cfg *con = Cfg::bulid();
    int value = con->read("log_count", 10, "Sys").toInt();

    sCfgItem *item = con->item;
    item->logCount = value * 10000;
    ui->logCountSpin->setValue(value);
}


void Setup_MainWid::writeLogCount()
{
    int arg1 = ui->logCountSpin->value();
    mItem->logCount = arg1;
    Cfg::bulid()->write("log_count", arg1, "Sys");
}


void Setup_MainWid::initPcNum()
{
    Cfg *con = Cfg::bulid();
    int value = con->read("pc_num", 0, "Sys").toInt();

    mItem->pcNum = value;
    ui->pcNumSpin->setValue(value);
}

void Setup_MainWid::writePcNum()
{
    int arg1 = ui->pcNumSpin->value();
    mItem->pcNum = arg1;
    Cfg::bulid()->write("pc_num", arg1, "Sys");
}

void Setup_MainWid::on_pcBtn_clicked()
{
    static int flg = 0;
    QString str = tr("修改");

    bool ret = usr_land_jur();
    if(!ret) {
        MsgBox::critical(this, tr("你无权进行此操作"));
        return;
    }

    if(flg++ %2) {
        ret = false;
        writePcNum();
        writeLogCount();
        Cfg::bulid()->writeCnt();
    } else {
        str = tr("保存");
    }

    ui->pcBtn->setText(str);
    ui->logCountSpin->setEnabled(ret);
    if(mItem->pcNum) ret = false;
    ui->pcNumSpin->setEnabled(ret);
}

void Setup_MainWid::on_verBtn_clicked()
{
    VersionDlg dlg(this);
    dlg.exec();
}


void Setup_MainWid::updateMac()
{
    sMac *it = &(mItem->macs);
    int ret =  MacAddr::bulid()->macCnt(it->mac, it->endMac);
    ui->cntMacLab->setNum(ret);
}


void Setup_MainWid::initMac()
{
    updateMac();
    sMac *it = &(mItem->macs);
    //ui->spinBox->setValue(it->cntMac);
    ui->startMacLab->setText(it->startMac);
    ui->endMacLab->setText(it->endMac);
}

void Setup_MainWid::updateSlot(int,int)
{
    initMac();
}

void Setup_MainWid::timeoutDone()
{
    updateMac();
}
