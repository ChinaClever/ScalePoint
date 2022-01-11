/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "home_workwid.h"
#include "ui_home_workwid.h"
#include <QFileDialog>

Home_WorkWid::Home_WorkWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home_WorkWid)
{
    ui->setupUi(this);
    QTimer::singleShot(450,this,SLOT(initFunSlot()));
}

Home_WorkWid::~Home_WorkWid()
{
    delete ui;
}

void Home_WorkWid::initLayout()
{
    QGridLayout *gridLayout = new QGridLayout(this->parentWidget());
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);

    ui->hwEdit->setText(mItem->hw);
    ui->fnEdit->setText(mItem->firmware);
    ui->cmdEdit->setText(mItem->commander);
    ui->blEdit->setText(mItem->bootloader);
}

void Home_WorkWid::initFunSlot()
{
    initLayout();
    mPro->step = Test_End;
    timer = new QTimer(this);
    timer->start(500);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    QTimer::singleShot(450,this,SLOT(updateCntSlot()));
    mCoreThread = new Test_CoreThread(this);   
}

void Home_WorkWid::updateCntSlot()
{
    sCount *cnt = &(mItem->cnts);
    ui->okLcd->display(cnt->ok);
    ui->allLcd->display(cnt->all);
    ui->errLcd->display(cnt->err);

    ui->cntSpin->setValue(mItem->cnts.cnt);
    if(mItem->cnts.cnt < 1)mItem->user.clear();
    ui->userEdit->setText(mItem->user);

    QString str = "0";
    if(cnt->all) {
        double value = cnt->ok*100.0 / cnt->all;
        str = QString::number(value,'f',0) +"%";
    }
    ui->passLcd->display(str);
}

QString Home_WorkWid::getTime()
{
    QTime t(0,0,0,0);
    t = t.addSecs(mPro->startTime.secsTo(QTime::currentTime()));
    return  tr("%1").arg(t.toString("mm:ss"));
}

void Home_WorkWid::updateTime()
{
    QString str = getTime();
    QString style = "background-color:yellow; color:rgb(0, 0, 0);";
    style += "font:100 34pt \"微软雅黑\";";

    ui->timeLab->setText(str);
    ui->timeLab->setStyleSheet(style);
}

void Home_WorkWid::setWidEnabled(bool en)
{
    ui->modeBox->setEnabled(en);
    ui->groupBox_2->setEnabled(en);
    ui->groupBox_4->setEnabled(en);
    ui->groupBox_6->setEnabled(en);
}

void Home_WorkWid::updateResult()
{
    QString style;
    QString str = tr("---");
    if(Test_Fail == mPro->result) {
        str = tr("失败"); style = "background-color:red; color:rgb(255, 255, 255);";
    } else {
        str = tr("成功"); style = "background-color:green; color:rgb(255, 255, 255);";
    } style += "font:100 34pt \"微软雅黑\";";

    setWidEnabled(true);
    mPro->step = Test_End;
    ui->timeLab->setText(str);
    ui->timeLab->setStyleSheet(style);
    ui->startBtn->setText(tr("开 始"));
    QTimer::singleShot(1450,this,SLOT(updateCntSlot()));
    str = QTime::currentTime().toString("hh:mm:ss");
    ui->endLab->setText(str);
}

void Home_WorkWid::updateWid()
{
    QString str = mDt->sn;
    if(str.isEmpty()) str = "--- ---";
    ui->snLab->setText(str);

    str = mDt->pn;
    if(str.isEmpty()) str = "--- ---";
    ui->pnLab->setText(str);

    str = mDt->code;
    if(str.isEmpty()) str = "--- ---";
    ui->codeLab->setText(str);

    if(mPro->step < Test_Over) {
        updateTime();
    } else if(mPro->step < Test_End){
        updateResult();
    }
}

void Home_WorkWid::timeoutDone()
{
    updateWid();
}

bool Home_WorkWid::initSerial()
{
//    bool ret = mItem->coms.sp->isOpened();
//    if(!ret) {MsgBox::critical(this, tr("请先打开Socket串口")); return ret;}
    QString str = ui->cmdEdit->text();
    if(str.isEmpty()) {
        QString str = tr(" commander.exe 烧录程序未指定\n 软件无法执行。。。");
        MsgBox::critical(this, str); return false;
    }

    str = ui->blEdit->text();
    if(str.isEmpty()) {
        QString str = tr(" Bootloader 文件未指定\n 软件无法执行。。。");
        MsgBox::critical(this,str); return false;
    }

    str = ui->fnEdit->text();
    if(str.isEmpty()) {
        QString str = tr(" 设备固件(*.s37)未指定\n 软件无法执行。。。");
        MsgBox::critical(this,str); return false;
    }

    return true;
}

bool Home_WorkWid::initUser()
{
    if(mItem->user != ui->userEdit->text()) {
        mItem->user = ui->userEdit->text();
        sCount *cnt = &(mItem->cnts);
        cnt->all = cnt->ok = cnt->err = 0;
        Cfg::bulid()->writeCnt();
    }

    if(mItem->cnts.cnt != ui->cntSpin->value()) {
        mItem->cnts.cnt = ui->cntSpin->value();
        Cfg::bulid()->writeCnt();
    }

    mItem->hw = ui->hwEdit->text();
    if(mItem->user.isEmpty()){MsgBox::critical(this, tr("请先填写客户名称！")); return false;}
    if(mItem->cnts.cnt < 1){MsgBox::critical(this, tr("请先填写订单剩余数量！")); return false;}
    if(!mItem->hw.size()){MsgBox::critical(this, tr("请填写设备硬件版本(HW)")); return false;}
    return true;
}

void Home_WorkWid::initData()
{
    mPacket->init();
    mDt->pn = "AG13098AA";
}

bool Home_WorkWid::inputCheck()
{
    bool ret = initSerial();
    if(ret) ret = initUser();
    return ret;
}

bool Home_WorkWid::initWid()
{
    bool ret = inputCheck();
    if(ret) {
        initData();
        setWidEnabled(false);
        ui->startBtn->setText(tr("终 止"));
        mPro->step = ui->modeBox->currentIndex()+Test_Start; emit startSig();
        QString str = mPro->startTime.toString("hh:mm:ss");
        ui->startLab->setText(str);
        ui->endLab->setText("---");
    }

    return ret;
}

void Home_WorkWid::on_startBtn_clicked()
{
    if(mPro->step == Test_End) {
        if(initWid()) mCoreThread->start();
    } else {
        bool ret = MsgBox::question(this, tr("确定需要提前结束？"));
        if(ret) {
            mPro->result = Test_Fail;
            updateResult();
        }
    }
}

QString Home_WorkWid::selectFile(const QString &filter, QString dir)
{
    if(dir.isEmpty()) dir = QCoreApplication::applicationDirPath();
    QString fn = QFileDialog::getOpenFileName(this, tr("选择文件"), dir, filter);
    if(fn.contains(".exe") || fn.contains(".s37")) {

    }

    return fn;
}

void Home_WorkWid::on_cmdBtn_clicked()
{
    QString fn = selectFile("执行文件(*.exe)", ui->cmdEdit->text());
    if(fn.contains(".exe"))  ui->cmdEdit->setText(fn);
    mItem->commander = fn;
}

void Home_WorkWid::on_dlBtn_clicked()
{
    QString fn = selectFile("引导文件(*.s37)", ui->blEdit->text());
    if(fn.contains(".s37")) ui->blEdit->setText(fn);
    mItem->bootloader = fn;
}

void Home_WorkWid::on_fnBtn_clicked()
{
    QString fn = selectFile("设备固件(*.s37)", ui->fnEdit->text());
    if(fn.contains(".s37")) ui->fnEdit->setText(fn);
    mItem->firmware = fn;
}
