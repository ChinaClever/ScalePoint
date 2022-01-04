/*
 *
 *  Created on: 2022年1月1日
 *      Author: Lzy
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "backcolourcom.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    groupBox_background_icon(ui->groupBox);
    set_background_icon(ui->barWid,":/image/title_back.jpg");
}

MainWindow::~MainWindow()
{
    delete ui;
}

