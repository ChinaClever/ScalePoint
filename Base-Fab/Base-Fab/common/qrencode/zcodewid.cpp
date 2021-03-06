/*
 *
 *  Created on: 2022年1月1日
 *      Author: Lzy
 */
#include "zcodewid.h"
#include <QLineEdit>
#include <QComboBox>
#include <QVBoxLayout>

ZcodeWid::ZcodeWid(QWidget *parent) : QWidget(parent)
{
    m_symbol = BARCODE_QRCODE;
    mBarcode = new BarcodeItem();
    mView = new QGraphicsView(this);
    QGraphicsScene *scene = new QGraphicsScene(this);

    scene->addItem(mBarcode);
    mView->setScene(scene);

    mBarcode->setBoundingRect(QSize(100, 100)); //parent->size()
    QGridLayout *gridLayout = new QGridLayout(parent);
    gridLayout->setContentsMargins(9, 9, 9, 9);
    gridLayout->addWidget(mView);
    mView->adjustSize();
}

void ZcodeWid::setSymbol(int sym)
{
    switch (sym) {
    case Barcode_128: sym = BARCODE_CODE128; break;
    case Barcode_39: sym = BARCODE_CODE39; break;
    case EAN13: sym = BARCODE_EANX; break;
    case QRCode: sym = BARCODE_QRCODE; break;
    case Datamatrix: sym = BARCODE_DATAMATRIX; break;
    case Pdf147: sym = BARCODE_PDF417; break;
    }
    mBarcode->barcodeItemPara.m_symbol = sym;
}

void ZcodeWid::createCode(const QString &str, int sym)
{
    setSymbol(sym);
    mBarcode->barcodeItemPara.m_text = str;
    mBarcode->drawBarcodeItem(mBarcode->barcodeItemPara);
    mView->update();
}
