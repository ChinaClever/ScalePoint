#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "common/printer/printer_bartender.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_index = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_printBtn_clicked()
{
    //QString str = "G$K:AG13098_A$I:616D3961B2A1CE6548C25DBA64BF063C6751%Z$A:0004740001100CBF%M:23W23$HW:1.5$FW:1.113";
    QString str = ui->labelEdit->text();
    bool ret = false;
    if(str.contains("￥")){
        ui->anslabel->setText(tr("请切换成英文输入法！！！"));
        return;
    }
    if(!str.isEmpty()){
        if(0 == m_index){
            str.replace("G$K:","/");
            str.replace("$I:","/");
            str.replace("%Z$A:","/");
            str.replace("%M:","/");
            str.replace("$HW:","/");
            str.replace("$FW:","/");
            if(str.contains("PN")) str.remove("PN");
            int port = ui->portEdit->text().toUInt();
            QStringList list = str.split("/");
            sBarTend it;
            it.pn = list.at(1);
            it.code = list.at(2);
            it.sn = list.at(3);
            it.date = list.at(4);
            it.hw = list.at(5);
            it.fw = list.at(6);
            //it.fw = "1.113";
            //QString ss = QString("G$K:%1$I:%2%Z$A:%3%M:%4$HW:%5$FW:%6").arg(it.pn).arg(it.code).arg(it.sn).arg(it.date).arg(it.hw).arg(it.fw);
            //qDebug()<<ss;
            ret = Printer_BarTender::bulid(this)->printer(it , port);
            if(!ret) ret = Printer_BarTender::bulid(this)->printer(it, port);
        }else if(1 == m_index){
            str.replace("G$K:","/");
            str.replace("%$S:","/");
            str.replace("%M:","/");
            str.replace("$HW:","/");
            str.replace("$FW:","/");
            if(str.contains("PN")) str.remove("PN");
            int port = ui->portEdit->text().toUInt();
            QStringList list = str.split("/");
            sBarTend it;
            it.pn = list.at(1);
            it.sn = list.at(2);
            it.date = list.at(3);
            it.hw = list.at(4);
            it.fw = list.at(5);
            ret = Printer_BarTender::bulid(this)->printer(it , port);
            if(!ret) ret = Printer_BarTender::bulid(this)->printer(it, port);
        }else{
            str.replace("G$K:","/");
            str.replace("%E$A:","/");
            str.replace("%M:","/");
            str.replace("$HW:","/");
            str.replace("$FW:","/");
            if(str.contains("PN")) str.remove("PN");
            int port = ui->portEdit->text().toUInt();
            QStringList list = str.split("/");
            sBarTend it;
            it.pn = list.at(1);
            it.sn = list.at(2);
            it.date = list.at(3);
            it.hw = list.at(4);
            it.fw = list.at(5);
            ret = Printer_BarTender::bulid(this)->printer(it , port);
            if(!ret) ret = Printer_BarTender::bulid(this)->printer(it, port);
        }
    }
    ui->labelEdit->setText("");
    if(ret) ui->anslabel->setText(tr("打印成功"));
    else ui->anslabel->setText(tr("打印失败"));
}

void MainWindow::on_typeComboBox_currentIndexChanged(int index)
{
    m_index = index;
}
