/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "setup_mainwid.h"
#include "ui_setup_mainwid.h"
#include "versiondlg.h"
#include "macaddr.h"

Setup_MainWid::Setup_MainWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Setup_MainWid)
{
    ui->setupUi(this);
    groupBox_background_icon(this);
    QTimer::singleShot(rand()%13,this,SLOT(initFunSlot()));
    mItem = Cfg::bulid()->item;
    initSerial();
}

Setup_MainWid::~Setup_MainWid()
{
    delete ui;
}

void Setup_MainWid::initFunSlot()
{
    initErrData();
    initLogCount();
    ui->pcNumSpin->setValue(mItem->pcNum);
    mUserWid = new UserMainWid(ui->stackedWid);
    ui->stackedWid->addWidget(mUserWid);
    QTimer::singleShot(2*1000,this,SLOT(checkPcNumSlot()));
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
    mItem->coms.sp = mComWid->initSerialPort(tr("IMM"));

    mSourceWid = new SerialStatusWid(ui->sourceWid);
    mItem->coms.src = mSourceWid->initSerialPort(tr("标准源"));
}

void Setup_MainWid::initLogCount()
{
    Cfg *con = Cfg::bulid(this);
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
        writeLogCount();
        mItem->pcNum = ui->pcNumSpin->value();
        Cfg::bulid()->writeCfgDev();
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
    //VersionDlg dlg(this);
    //dlg.exec();
}


void Setup_MainWid::updateErrData()
{
    sErrData *errs = &(mItem->errs);
    errs->volErr = ui->volErrBox->value();
    errs->curErr = ui->curErrBox->value() * 10;
    errs->powErr = ui->powErrBox->value() * 10;
    Cfg::bulid()->writeErrData();
}

void Setup_MainWid::initErrData()
{
    sErrData *errs = &(mItem->errs);
    ui->volErrBox->setValue(errs->volErr);
    ui->curErrBox->setValue(errs->curErr / 10.0);
    ui->powErrBox->setValue(errs->powErr / 10.0);
}


void Setup_MainWid::on_saveBtn_clicked()
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
        updateErrData();
    } else {
        str = tr("保存");
    }

    ui->saveBtn->setText(str);
    ui->volErrBox->setEnabled(ret);
    ui->curErrBox->setEnabled(ret);
    ui->powErrBox->setEnabled(ret);
}
