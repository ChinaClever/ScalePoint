/*
 *
 *  Created on: 2021年1月1日
 *      Author: Lzy
 */
#include "printerdlg.h"
#include "ui_printerdlg.h"

PrinterDlg::PrinterDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrinterDlg)
{
    ui->setupUi(this);
    mPrinter = new PrinterWid(this);

    sPrIt it;
    printer(it);
}

PrinterDlg::~PrinterDlg()
{
    delete ui;
}

void PrinterDlg::printer(sPrIt &it)
{
    it.pn = "AG13095AA";
    it.hw = "01"; it.fw = "1.2.3";
    it.sn = "0123456789";

    PrinterPixmap *pixmap = new PrinterPixmap;
    QPixmap pix = pixmap->moduleGrab(it);
    mPrinter->printerWid(pix);
}
