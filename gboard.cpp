#include "gboard.h"
#include "ui_board.h"

gBoard::gBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gBoard)
{
    ui->setupUi(this);
}

gBoard::~gBoard()
{
    delete ui;
}

