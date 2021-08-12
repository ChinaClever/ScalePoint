/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "home_workwid.h"
#include "ui_home_workwid.h"

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
    QPalette pl = ui->textEdit->palette();
    pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    ui->textEdit->setPalette(pl);
    //ui->userEdit->setPalette(pl); //ui->cntSpin->setPalette(pl);
    //ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    //ui->textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QGridLayout *gridLayout = new QGridLayout(this->parentWidget());
    gridLayout->setContentsMargins(0, 0, 0, 0);
    gridLayout->addWidget(this);
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

void Home_WorkWid::setTextColor()
{
    QColor color("black");
    bool pass = mPro->pass.first();
    if(!pass) color = QColor("red");
    ui->textEdit->moveCursor(QTextCursor::Start);

    QTextCharFormat fmt;//文本字符格式
    fmt.setForeground(color);// 前景色(即字体色)设为color色
    QTextCursor cursor = ui->textEdit->textCursor();//获取文本光标
    cursor.mergeCharFormat(fmt);//光标后的文字就用该格式显示
    ui->textEdit->mergeCurrentCharFormat(fmt);//textEdit使用当前的字符格式
}

void Home_WorkWid::insertText()
{
    while(mPro->status.size()) {
        setTextColor();
        QString str = QString::number(mId++) + "、"+ mPro->status.first() + "\n";
        ui->textEdit->insertPlainText(str);
        mPro->status.removeFirst();
        mPro->pass.removeFirst();
    }
}

void Home_WorkWid::updateCntSlot()
{
    sCount *cnt = &(mItem->cnt);
    ui->okLcd->display(cnt->ok);
    ui->allLcd->display(cnt->all);
    ui->errLcd->display(cnt->err);

    ui->cntSpin->setValue(mItem->cnt.cnt);
    if(mItem->cnt.cnt < 1)mItem->user.clear();
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

void Home_WorkWid::updateResult()
{
    QString style;
    QString str = tr("---");
    if(isCheck){ if(mId < 14) mPro->result = Test_Fail; }
    if(Test_Fail == mPro->result) {
        str = tr("失败");
        style = "background-color:red; color:rgb(255, 255, 255);";
    } else {
        str = tr("成功");
        style = "background-color:green; color:rgb(255, 255, 255);";
    }
    style += "font:100 34pt \"微软雅黑\";";

    mPro->step = Test_End;
    ui->timeLab->setText(str);
    ui->groupBox_2->setEnabled(true);
    ui->timeLab->setStyleSheet(style);
    ui->startBtn->setText(tr("开 始"));
    QTimer::singleShot(450,this,SLOT(updateCntSlot()));
    str = QTime::currentTime().toString("hh:mm:ss");
    ui->endLab->setText(str);
}

void Home_WorkWid::updateWid()
{
    QString str = mDt->sn;
    if(str.isEmpty()) str = "--- ---";
    ui->snLab->setText(str);

    str = mDt->manufacturer;
    if(str.isEmpty()) str = "--- ---";
    ui->mfLab->setText(str);

    str = mDt->model;
    if(str.isEmpty()) str = "--- ---";
    ui->modelLab->setText(str);

    str = mDt->hwRevision;
    if(str.isEmpty()) str = "--- ---";
    ui->hwLab->setText(str);

    str = mDt->fwRevision;
    if(str.isEmpty()) str = "--- ---";
    ui->fwLab->setText(str);

    str = mDt->ctrlBoardSerial;
    if(str.isEmpty()) str = "--- ---";
    ui->ctrlSnLab->setText(str);

    str = mDt->macAddress;
    if(str.isEmpty()) str = "--- ---";
    ui->macLab->setText(str);

    if(mPro->step < Test_Over) {
        updateTime();
    } else if(mPro->step < Test_End){
        updateResult();
    }
}

void Home_WorkWid::timeoutDone()
{
    insertText();
    updateWid();
}

bool Home_WorkWid::initSerial()
{
    bool ret = mItem->com->isOpened();
    if(!ret) {MsgBox::critical(this, tr("请先打开PDU串口")); return ret;}
    return ret;
}

void Home_WorkWid::initUser()
{
    if(mItem->user != ui->userEdit->text()) {
        mItem->user = ui->userEdit->text();
        sCount *cnt = &(mItem->cnt);
        cnt->all = cnt->ok = cnt->err = 0;
        Cfg::bulid()->writeCnt();
    }

    if(mItem->cnt.cnt != ui->cntSpin->value()) {
        mItem->cnt.cnt = ui->cntSpin->value();
        Cfg::bulid()->writeCnt();
    }
}

void Home_WorkWid::initData()
{
    mId = 1;
    mPacket->init();
    ui->textEdit->clear();
    mDt->aiFind = ui->adCheckBox->isChecked();
    mDt->user = ui->userEdit_2->text();
    mDt->pwd = ui->pwdEdit->text();
    mDt->ip = ui->ipEdit->text();
}

bool Home_WorkWid::initWid()
{
    bool ret = initSerial();
    if(ret) {
        initUser();
        if(mItem->user.isEmpty()){MsgBox::critical(this, tr("请先填写客户名称！")); return false;}
        if(mItem->cnt.cnt < 1){MsgBox::critical(this, tr("请先填写订单剩余数量！")); return false;}

        initData();
        ui->groupBox_2->setEnabled(false);
        ui->startBtn->setText(tr("终 止"));
        mPro->step = Test_Start; emit startSig();
        if(mPro->step == Test_Start) isCheck = true; else isCheck = false;
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


void Home_WorkWid::on_adCheckBox_clicked(bool checked)
{
    ui->ipEdit->setEnabled(!checked);
}
