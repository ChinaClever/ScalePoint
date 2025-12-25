/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "printerpixmap.h"
#include "ui_printerpixmap.h"

PrinterPixmap::PrinterPixmap(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrinterPixmap)
{
    ui->setupUi(this);
    mQR = new QRWidget(ui->qrWid);
    setPalette(QPalette(Qt::white));//设置窗口为白色背景
    setAutoFillBackground(true);
}

PrinterPixmap::~PrinterPixmap()
{
    delete ui;
}

QPixmap PrinterPixmap::moduleGrab(sPrIt &it)
{
    ui->pnLab->setText("PN:" + it.pn);
    ui->snLab->setText("SN " + it.sn);
    ui->hwLab->setText(tr("HW:%1-FW:%2").arg(it.hw).arg(it.fw));
    QString date = QDate::currentDate().toString("yy") + "W";
    date += QString("%1").arg(QDate::currentDate().weekNumber(), 2, 10, QLatin1Char('0'));
    ui->dateLab->setText(date);

    QString str = QString("G$K:%1%$S:%2%M:%3$HW:%4$FW%5").arg(it.pn).arg(it.sn).arg(date).arg(it.hw).arg(it.fw);
    mQR->setString(str);
    return ui->widget->grab();
}
