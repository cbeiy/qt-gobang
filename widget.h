#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "gobang.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_1_clicked();

private:
    Ui::Widget *ui;
    gobang* go;
};

#endif // WIDGET_H
