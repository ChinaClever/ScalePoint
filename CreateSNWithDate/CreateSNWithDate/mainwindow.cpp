#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDate>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QDate date = QDate::currentDate();
    QString currentDate = date.toString("yyyy-MM-dd");
    ui->dateEdit->setText(currentDate);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::dayOfYear(QString date)
{
    QStringList str = date.split("-");
    if(str.size() < 3 || str.size() >= 4 ) return -1;
    if(str.at(0).isEmpty() || str.at(1).isEmpty() ||  str.at(2).isEmpty()) return -4;
    int year = str.at(0).toInt();
    int month = str.at(1).toInt();
    int day = str.at(2).toInt();

    int m[12] = {31 , 28 , 31 , 30 , 31 , 30 , 31 , 31  , 30 , 31 , 30 , 31};
    bool flag = false;
    if( year%2000 >= 65 || year <= 2021 ) return -5;//检查年是否正常
    if( (year % 4 == 0 && year % 100 != 0 )|| year % 400 == 0 ) flag = true;
    if(month >12 || month < 1) return -2;//检查月是否正常
    if(flag ){//检查日是否正常//闰年
        if( month == 2 ){
            if((day > 29 || day <= 0)) return -3;
        }
        else{
            if(m[month - 1] < day || day <= 0 ) return -3;
        }
    }else{//平年
        if( m[month - 1] < day || day <= 0 ) return -3;
    }

    for(int i = 0 ; i < month - 1 ; i++) {
        day += m[i];
    }


    return flag && month > 2 ? day+1:day;
}

void MainWindow::on_createSnBtn_clicked()
{
    QString str = ui->dateEdit->text();
    int day = dayOfYear(str);
    if(day == -1) {ui->snEdit->setText("日期格式错误，必须yyyy-MM-dd"); return;}
    else if(day == -2) {ui->snEdit->setText("年月日中月份错误"); return;}
    else if(day == -3) {ui->snEdit->setText("年月日中日错误"); return;}
    else if(day == -4) {ui->snEdit->setText("日期格式错误，必须yyyy-MM-dd"); return;}
    else if(day == -5) {ui->snEdit->setText("年月日中年错误");return;}
    int year = str.split("-").at(0).toInt();
    if( year >= 2033 ) {ui->snEdit->setText("软件需要更新,请联系克莱沃软件！");return;}

    int y = year%2000;
    int count = y * 1000 + day;
    int hi = (count&0xff00)>>8;
    int lo = count&0xff;
    ui->snEdit->setText(QString("01 00 %1 %2").arg(QString::number(lo,16)).arg(QString::number(hi,16)));
}

void MainWindow::on_getDateBtn_clicked()
{
    QDate date = QDate::currentDate();
    QString currentDate = date.toString("yyyy-MM-dd");
    ui->dateEdit->setText(currentDate);
}
