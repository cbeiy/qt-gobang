#ifndef BOARD_H
#define BOARD_H

#include <QWidget>

namespace Ui {
class gBoard;
}

class gBoard : public QWidget
{
    Q_OBJECT

public:
    explicit gBoard(QWidget *parent = 0);
    ~gBoard();

private:
    Ui::gBoard *ui;
};


#endif // BOARD_H
