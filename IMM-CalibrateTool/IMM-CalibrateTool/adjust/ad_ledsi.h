#ifndef AD_LEDSI_H
#define AD_LEDSI_H

#include "yc_obj.h"
#include "dev_ipcfg.h"

class Ad_LedSi : public BaseThread
{
    Q_OBJECT
    explicit Ad_LedSi(QObject *parent = nullptr);
public:
    static Ad_LedSi *bulid(QObject *parent = nullptr);
    bool startAdjust();

protected:
    bool sentCmd();
    bool writeCmd(uchar fn, uchar line);
    bool transmit(uchar *buf, int len);

    bool writeDc();
    bool writeAc();

private:
    Dev_SiRtu *mRtu;
};

#endif // AD_LEDSI_H
