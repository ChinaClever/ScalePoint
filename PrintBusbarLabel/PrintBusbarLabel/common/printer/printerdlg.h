#ifndef PRINTERDLG_H
#define PRINTERDLG_H

#include <QDialog>
#include "printerpixmap.h"

namespace Ui {
class PrinterDlg;
}

class PrinterDlg : public QDialog
{
    Q_OBJECT

public:
    explicit PrinterDlg(QWidget *parent = nullptr);
    ~PrinterDlg();
    void printer(sPrIt &it);

private:
    Ui::PrinterDlg *ui;
    PrinterWid *mPrinter;
    PrinterPixmap *mPixmap;
};

#endif // PRINTERDLG_H
