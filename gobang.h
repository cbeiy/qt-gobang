#ifndef GOBANG_H
#define GOBANG_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QTimer>
#include <QTime>
#include <sys/time.h>
#include "ai.h"
using namespace std;

enum gMode{PLAYER,AI};
enum gStatus{UNDERWAY,FINISH};
enum gTurn{T_BLACK,T_WHITE};

namespace Ui {
class gobang;
}

class gobang : public QWidget
{
    Q_OBJECT

public:
    explicit gobang(QWidget *parent = 0);
    ~gobang();

    void initializeGame();   

    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void setGameMode(gMode m);
    void oneChessMove(int row,int col);
    bool chessOneByPlayer();
    bool isDeadGame();
    bool isLegalMove(int row,int col);

signals:
    void returnSignal();

private slots:
    void on_btn_return_clicked();
    void chessOneByAi();

private:
    Ui::gobang *ui;

    QPoint qchessboard[15][15];
    _AI ai;

    gMode mode;
    gTurn turn;
    gStatus status;

    int cursorRow;  //光标位置
    int cursorCol;

    int score_black;
    int score_white;
};

#endif // GOBANG_H
