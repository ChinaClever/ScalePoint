#include "programwid.h"
#include "ui_programwid.h"

ProgramWid::ProgramWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProgramWid)
{
    ui->setupUi(this);
}

ProgramWid::~ProgramWid()
{
    delete ui;
}
