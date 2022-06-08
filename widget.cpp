#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    go = new gobang();
    go->hide();
    connect(this->ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_1_clicked()));
    connect(this->ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_2_clicked()));
    connect(go,&gobang::returnSignal,this,&Widget::show);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_1_clicked()
{
    this->hide();
    go->show();
    go->setGameMode(PLAYER);
}

void Widget::on_pushButton_2_clicked()
{
    this->hide();
    go->show();
    go->setGameMode(AI);
}
