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
    mThread = new ProgramThread(id, this);
    ui->groupBox->setTitle(tr("设备 %1").arg(mId+1));

    timer = new QTimer(this);
    timer->start(1000);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeoutDone()));
    connect(mThread, SIGNAL(fabSig(bool)), this, SLOT(endFunSlot(bool)));
    connect(mThread, SIGNAL(proSig(int, int)), this, SLOT(updateProSlot(int, int)));
}

ProgramWid::~ProgramWid()
{
    delete ui;
}

void ProgramWid::initWid()
{
    QPalette pl = ui->progressBar_1->palette();
    pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    ui->progressBar_1->setPalette(pl);
    ui->progressBar_2->setPalette(pl);
    // ui->progressBar->setAlignment(Qt::AlignTop);

    QPalette pa;
    pa.setColor(QPalette::WindowText, Qt::magenta);
    ui->devLab->setPalette(pa);
}


bool ProgramWid::isFileExist()
{
    bool ret = false;
    QString fn = "/dev/ttyACM%1";

    QFileInfo file(fn.arg(mId));
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
    if(res){
        str = tr("成 功");
        style = "background-color:green; color:rgb(255, 255, 255);";
        ui->progressBar_2->setValue(100);
    } else {
        str = tr("失 败");
        style = "background-color:red; color:rgb(255, 255, 255);";
    }

    isRun = false;
    style += "font:100 24pt \"微软雅黑\";";
    ui->statusLab->setStyleSheet(style);
    ui->startBtn->setEnabled(true);
    ui->statusLab->setText(str);
}

void ProgramWid::updateProSlot(int id, int v)
{
    switch (id) {
    case 1: ui->progressBar_1->setValue(v); break;
    case 2: ui->progressBar_2->setValue(v); break;
    }

    if(id > 1) {
        int ret = ui->progressBar_1->value();
        if(ret > 80)ui->progressBar_1->setValue(100);
    }
}

void ProgramWid::firmwareBurn()
{
    mCount += 1;
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
    ui->startBtn->setDisabled(true);
    ui->progressBar_1->setValue(0);
    ui->progressBar_2->setValue(0);
    ui->endLab->setText("");
    mThread->start();
}

void ProgramWid::startSlot()
{
    bool en = ui->startBtn->isEnabled();
    if(en && !isRun) {
        on_startBtn_clicked();
    }
}
