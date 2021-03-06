/*
 *
 *
 *  Created on: 2018年10月1日
 *      Author: Lzy
 */
#include "excel_savethread.h"
#include "msgbox.h"

Excel_SaveThread::Excel_SaveThread(QObject *parent) : QThread(parent)
{

}

Excel_SaveThread::~Excel_SaveThread()
{
    wait();
}

void Excel_SaveThread::saveData(const QString &fileName, QList<QStringList> &list)
{
    mFileName = fileName;
    mList.clear();
    mList.append(list);
    start();
}


/**
 * @brief 百分制来计算
 */
int Excel_SaveThread::getProgress()
{
    int ret = 100;

    if(mSize < mAllSize)
        ret = (mSize *100.0) / mAllSize ;

    return ret;
}


bool Excel_SaveThread::init()
{
    bool ret = true;
#if defined(Q_OS_WIN32)
    mSize = mAllSize = 0;
    HRESULT r = OleInitialize(0);  // 增加头文件 #include <sapi.h>
    if (r != S_OK && r != S_FALSE)
    {
        qWarning("Qt:初始化Ole失败（error %x）",(unsigned int)r);
        MsgBox::critical(0, tr("Qt:初始化Ole失败"));
        ret = false;
    }
#endif
    return ret;
}

void Excel_SaveThread::writeFile(QList<QStringList> &list)
{
#if defined(Q_OS_WIN32)
    QExcel excel(this);
    bool ret = excel.createNewExcel(mFileName +".xlsx");
    if(ret) {
        excel.selectSheet("Sheet1"); //  Sheet1
        excel.saveAs();

        for(int i=0; i<list.size(); ++i)
        {
            for(int j=0; j<list.at(i).size(); ++j) {
                excel.setCellString(i+1, j+1, list.at(i).at(j));
            }
            mSize++;
            msleep(25);
        }
        excel.save();
    }
#endif
}

void Excel_SaveThread::run()
{
    bool ret = init();
    if(ret) {
        mAllSize = mList.size();
        writeFile(mList);
    }
}
