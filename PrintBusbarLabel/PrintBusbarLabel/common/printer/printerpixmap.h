#ifndef PRINTERPIXMAP_H
#define PRINTERPIXMAP_H
#include "printerwid.h"
#include "qrwidget.h"

namespace Ui {
class PrinterPixmap;
}

struct sPrIt {
    QString pn;
    QString hw;
    QString fw;
    QString sn;
};

class PrinterPixmap : public QWidget
{
    Q_OBJECT

public:
    explicit PrinterPixmap(QWidget *parent = nullptr);
    ~PrinterPixmap();

    QPixmap moduleGrab(sPrIt &it);
private:
    Ui::PrinterPixmap *ui;
    QRWidget *mQR;
};

#endif // PRINTERPIXMAP_H
