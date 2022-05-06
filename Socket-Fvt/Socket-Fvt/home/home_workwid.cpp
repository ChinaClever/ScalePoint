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
    ui->hwEdit->setText(mItem->hw);
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
}

void Home_WorkWid::updateResult()
{
    QString style;
    QString str = tr("---");
    if(isCheck){ if(mId < 10) mPro->result = Test_Fail; }
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
    QTimer::singleShot(450,this,SLOT(updateCntSlot()));
    str = QTime::currentTime().toString("hh:mm:ss");
    ui->endLab->setText(str);
}

void Home_WorkWid::updateWid()
{
    QString str = mDt->sn;
    if(str.isEmpty()) str = "--- ---";
    ui->snLab->setText(str);

    str = mDt->fw;
    if(str.isEmpty()) str = "--- ---";
    ui->fwLab->setText(str);

    str = mDt->pn;
    if(str.isEmpty()) str = "--- ---";
    ui->pnLab->setText(str);

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
    bool ret = mItem->coms.sp->isOpened();
    if(!ret) {MsgBox::critical(this, tr("请先打开Socket串口")); return ret;}

    ret = mItem->coms.src->isOpened();
    if(!ret) {MsgBox::critical(this, tr("请先打开治具串口")); return ret;}

    return ret;
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
    mId = 1;
    mPacket->init();
    ui->textEdit->clear();
}

bool Home_WorkWid::inputCheck()
{
    bool ret = initSerial();
    if(ret) ret = initUser();
    if(ret) {
        switch (ui->specBox->currentIndex()) {
        case 1: mDt->pn = "AG13093AA"; break;
        case 2: mDt->pn = "AG13094AA"; break;
        case 3: mDt->pn = "AG13095AA"; break;
        case 4: mDt->pn = "AG13096AA"; break;
        case 5: mDt->pn = "AG13097AA"; break;
        default: MsgBox::critical(this, tr("请选择设备规格")); ret = false; break;
        }
    }

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


