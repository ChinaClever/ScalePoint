/*
 *
 *  Created on: 2021年10月1日
 *      Author: Lzy
 */
#include "programwid.h"
#include "ui_programwid.h"
#include <QPalette>

ProgramWid::ProgramWid(int id, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgramWid)
{
    ui->setupUi(this);

    initWid();
    mId=id; isRun = false;
    ui->progressBar->setMaximum(8*60);
    mThread = new ProgramThread(id, this);
    ui->groupBox->setTitle(tr("设备 %1").arg(mId+1));

    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    connect(mThread, SIGNAL(fabSig(bool)), this, SLOT(endFunSlot(bool)));
}

ProgramWid::~ProgramWid()
{
    delete ui;
}

void ProgramWid::initWid()
{
    QPalette pl = ui->progressBar->palette();
    pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    ui->progressBar->setPalette(pl);
    // ui->progressBar->setAlignment(Qt::AlignTop);

    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::magenta);
    ui->devLab->setPalette(pa);
}


bool ProgramWid::isFileExist()
{
    bool ret = false;
    QString fn = "/dev/ttyACM%1";

    QFile file(fn.arg(mId));
    if (file.exists()){
        ret =  true;
    } else {
        ui->startBtn->setEnabled(true);
    }

    ui->groupBox->setEnabled(ret);
    return ret;
}

void ProgramWid::endFunSlot(bool res)
{
    QString style;
    QString str = tr("---");

    isRun = false;
    if((mCount < 380)) {
        str = tr("失 败");
        style = "background-color:red; color:rgb(255, 255, 255);";
    } else if(res){
        str = tr("成 功");
        style = "background-color:green; color:rgb(255, 255, 255);";
        int m = ui->progressBar->maximum();
        ui->progressBar->setMaximum(m);
    }

    style += "font:100 24pt \"微软雅黑\";";
    ui->statusLab->setStyleSheet(style);
    ui->startBtn->setEnabled(true);
    ui->statusLab->setText(str);
}

void ProgramWid::firmwareBurn()
{
    ui->progressBar->setValue(mCount++);
    int sec = mCount % 60; int min = mCount / 60;
    ui->statusLab->setText(QString("0%1:%2").arg(min).arg(sec, 2, 10, QLatin1Char('0')));
    QString str = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->endLab->setText(str);

    QString style = "background-color:yellow; color:rgb(0, 0, 0);";
    style += "font:100 25pt \"微软雅黑\";";
    ui->statusLab->setStyleSheet(style);
}

bool ProgramWid::connected()
{
    QPalette pl;
    QColor acolor(Qt::gray);
    QString str = tr("已 连 接");

    bool ret = isFileExist();
    if(ret) {
        acolor = Qt::blue;
    } else {
        isRun = false;
        str = tr("未 连 接");
    }

    pl.setColor(QPalette::WindowText, acolor);
    ui->devLab->setPalette(pl);
    ui->devLab->setText(str);

    return ret;
}

void ProgramWid::timeoutDone()
{
    bool ret = connected();
    if(isRun && ret) {
        firmwareBurn();
    }
}

void ProgramWid::on_startBtn_clicked()
{
    QString str = QDateTime::currentDateTime().toString("hh:mm:ss");
    ui->startLab->setText(str); isRun = true; mCount =0;
    ui->progressBar->setValue(mCount++);
    ui->startBtn->setDisabled(true);
    ui->endLab->setText("");
    mThread->start();
}
