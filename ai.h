#ifndef AI_H
#define AI_H

#define DEPTH 4 //搜索深度

#include <QPoint>
#include <QMap>
#include <QDebug>


enum Result{Win,Lose,DRAW}; //黑赢，白赢，和棋
struct EVA
{
    Result r;
    int score;
};

struct POS{  //记录前十个最佳落子位置
    QPoint pos[10];
    int score[10];
};

class _AI
{
public:
    _AI();
    int chessboard[15][15]; //棋盘

    int nodenum;
    QPoint nextPos;

    void init_tuple6(); //对棋型判断数组赋初值
    EVA evaluate(int now_chessboard[15][15]); //估值函数、判断局势

    POS chooseBest10(int board[15][15]); //记录前十个最佳落子位置
    int dfs(int board[15][15], int depth, int alpha, int beta);  //alpha-beta剪枝


private:
    int tuple6[4][4][4][4][4][4];//棋型辨识数组,0空,1黑子,2白子,3边界

};

#endif // AI_H
