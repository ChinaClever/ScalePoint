#ifndef PROGRAMWID_H
#define PROGRAMWID_H

#include <QWidget>

namespace Ui {
class ProgramWid;
}

class ProgramWid : public QWidget
{
    Q_OBJECT

public:
    explicit ProgramWid(QWidget *parent = nullptr);
    ~ProgramWid();

private:
    Ui::ProgramWid *ui;
};

#endif // PROGRAMWID_H
