#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common/printer/printer_bartender.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_index = 0;
    ui->label_3->hide();
    ui->typeComboBox->hide();
    ui->label->hide();
    ui->portEdit->hide();
    connect(Printer_BarTender::bulid(), SIGNAL(reslutSig(bool)), this, SLOT(resultSlot(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_printBtn_clicked()
{
    //QString str = "https://clbusbar.legrandchina.cn/report?productSN=RQ202511120013&orderId=LPZA160SS3000&moduleSN=31020025122300010201";
    QString str = ui->labelEdit->text();
    bool ret = false;
    if(str.contains("￥")){
        ui->anslabel->setText(tr("请切换成英文输入法！！！"));
        return;
    }
    if(!str.isEmpty()){
        QString method = "Integration/Busbar-Product/Execute";
        QString ip = "127.0.0.1";
        sBarTend it;
        if(str.contains("https://clbusbar.legrandchina.cn/report?"))
            str.remove("https://clbusbar.legrandchina.cn/report?");
        if(str.contains("productSN="))
            str.replace("productSN=","");
        if(str.contains("orderId="))
            str.replace("orderId=","");
        if(str.contains("moduleSN="))
            str.replace("moduleSN=","");
        QStringList list = str.split("&");
        //str += it.on + ","; str += it.pn + ","; str += it.sn + ",";
        it.pn = list.at(0);
        it.on = list.at(1);
        it.sn = list.at(2);

        if(!ret){
            Printer_BarTender::bulid()->init(method , ip ,it);
            Printer_BarTender::bulid()->start();
        }
    }
    ui->labelEdit->setText("");
    //if(ret) ui->anslabel->setText(tr("打印成功"));
    //else ui->anslabel->setText(tr("打印失败"));
}

void MainWindow::on_typeComboBox_currentIndexChanged(int index)
{
    m_index = index;
}

void MainWindow::resultSlot(bool ret)
{
    if(ret) ui->anslabel->setText(tr("打印成功"));
    else ui->anslabel->setText(tr("打印失败"));
}
