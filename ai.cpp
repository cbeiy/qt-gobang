#include "ai.h"
#include <queue>

#define R_WIN 5000000
#define R_FLEX4 50000
#define R_BLOCK4 400
#define R_FLEX3 400
#define R_BLOCK3 20
#define R_FLEX2 20
#define R_BLOCK2 1
#define R_FLEX1 1

#define WIN -10000000
#define FLEX4 -100000
#define BLOCK4 -100000
#define FLEX3 -8000
#define BLOCK3 -50
#define FLEX2 -50
#define BLOCK2 -3
#define FLEX1 -3


_AI::_AI()
{
    init_tuple6();
    nodenum = 0;
    memset(chessboard,0,sizeof(chessboard));
}

void _AI::init_tuple6()
{
    memset(tuple6,0,sizeof(tuple6));

    //0空,1黑子,2白子,3边界
    //第0位非己方棋子，第1位己方棋子
    //黑连5
    tuple6[0][1][1][1][1][1] = WIN;
    tuple6[2][1][1][1][1][1] = WIN;
    tuple6[3][1][1][1][1][1] = WIN;
    //黑活4
    tuple6[0][1][1][1][1][0] = FLEX4;
    //黑冲4
    tuple6[2][1][1][1][1][0] = BLOCK4;
    tuple6[3][1][1][1][1][0] = BLOCK4;
    tuple6[0][1][1][1][1][2] = BLOCK4;
    tuple6[0][1][1][1][1][3] = BLOCK4;
    for(int i=0;i<4;++i)
    {
        if(i==1) continue;
        tuple6[i][1][0][1][1][1] = BLOCK4;
        tuple6[i][1][1][0][1][1] = BLOCK4;
        tuple6[i][1][1][1][0][1] = BLOCK4;
    }
    //黑活3
    tuple6[0][1][1][0][1][0] = FLEX3;
    tuple6[0][1][0][1][1][0] = FLEX3;
    for(int i=0;i<4;++i)
    {
        if(i==1) continue;
        tuple6[0][1][1][1][0][i] = FLEX3;
    }
    //黑眠3
    for(int i=2;i<=3;++i)
    {
        tuple6[i][1][1][1][0][0] = BLOCK3;
        tuple6[i][1][1][0][1][0] = BLOCK3;
        tuple6[i][1][0][1][1][0] = BLOCK3;
        tuple6[0][1][0][1][1][i] = BLOCK3;
        tuple6[0][1][1][0][1][i] = BLOCK3;
        tuple6[0][0][1][1][1][i] = BLOCK3; //特例
    }
    for(int i=0;i<4;++i)
    {
        if(i==1) continue;
        tuple6[i][1][1][0][0][1] = BLOCK3;
        tuple6[i][1][0][0][1][1] = BLOCK3;
        tuple6[i][1][0][1][0][1] = BLOCK3;
    }
    //黑活2
    tuple6[0][1][0][0][1][0] = FLEX2;
    for(int i=0;i<4;++i)
    {
        if(i==1) continue;
        tuple6[0][1][0][1][0][i] = FLEX2;
    }
    for(int i=0;i<4;++i)
        for(int j=0;j<4;++j)
        {
            if(i==1||j==1) continue;
            tuple6[0][1][1][0][i][j] = FLEX2;
        }
    //黑活1、眠2
    //黑眠2
    tuple6[2][1][1][0][0][0] = BLOCK2;
    //黑活1
    tuple6[0][1][0][0][0][0] = FLEX1;
    for(int i=0;i<4;++i)
    {
        if(i==1) continue;
        tuple6[i][0][1][0][0][0] = FLEX1;
        tuple6[i][0][0][1][0][0] = FLEX1;
        tuple6[i][0][0][0][1][0] = FLEX1;
        tuple6[i][0][0][0][0][1] = FLEX1;
    }

    //0空,1黑子,2白子,3边界
    //第0位非己方棋子，第1位己方棋子
    //白连5
    tuple6[0][2][2][2][2][2] = R_WIN;
    tuple6[1][2][2][2][2][2] = R_WIN;
    tuple6[3][2][2][2][2][2] = R_WIN;
    //白活4
    tuple6[0][2][2][2][2][0] = R_FLEX4;
    //白冲4
    tuple6[1][2][2][2][2][0] = R_BLOCK4;
    tuple6[3][2][2][2][2][0] = R_BLOCK4;
    tuple6[0][2][2][2][2][1] = R_BLOCK4;
    tuple6[0][2][2][2][2][3] = R_BLOCK4;
    for(int i=0;i<4;++i)
    {
        if(i==2) continue;
        tuple6[i][2][0][2][2][2] = R_BLOCK4;
        tuple6[i][2][2][0][2][2] = R_BLOCK4;
        tuple6[i][2][2][2][0][2] = R_BLOCK4;
    }
    //白活3
    tuple6[0][2][2][0][2][0] = R_FLEX3;
    tuple6[0][2][0][2][2][0] = R_FLEX3;
    for(int i=0;i<4;++i)
    {
        if(i==2) continue;
        tuple6[0][2][2][2][0][i] = R_FLEX3;
    }
    //白眠3
    for(int i=1;i<=3;++i)
    {
        if(i==2) continue;
        tuple6[i][2][2][2][0][0] = R_BLOCK3;
        tuple6[i][2][2][0][2][0] = R_BLOCK3;
        tuple6[i][2][0][2][2][0] = R_BLOCK3;
        tuple6[0][2][0][2][2][i] = R_BLOCK3;
        tuple6[0][2][2][0][2][i] = R_BLOCK3;
        tuple6[0][0][2][2][2][i] = R_BLOCK3; //特例
    }
    for(int i=0;i<4;++i)
    {
        if(i==2) continue;
        tuple6[i][2][2][0][0][2] = R_BLOCK3;
        tuple6[i][2][0][0][2][2] = R_BLOCK3;
        tuple6[i][2][0][2][0][2] = R_BLOCK3;
    }
    //白活2
    tuple6[0][2][0][0][2][0] = R_FLEX2;
    for(int i=0;i<4;++i)
    {
        if(i==2) continue;
        tuple6[0][2][0][2][0][i] = R_FLEX2;
    }
    for(int i=0;i<4;++i)
        for(int j=0;j<4;++j)
        {
            if(i==2||j==2) continue;
            tuple6[0][2][2][0][i][j] = R_FLEX2;
        }
    //白活1、眠2
    //白眠2
    tuple6[1][2][2][0][0][0] = R_BLOCK2;
    //白活1
    tuple6[0][2][0][0][0][0] = R_FLEX1;
    for(int i=0;i<4;++i)
    {
        if(i==2) continue;
        tuple6[i][0][2][0][0][0] = R_FLEX1;
        tuple6[i][0][0][2][0][0] = R_FLEX1;
        tuple6[i][0][0][0][2][0] = R_FLEX1;
        tuple6[i][0][0][0][0][2] = R_FLEX1;
    }

}

EVA _AI::evaluate(int now_chessboard[15][15]){

    int board[17][17];   //增加了一圈边界的棋盘
    //3边界
    for(int i=0;i<17;++i)
        board[i][0] = board[0][i] = board[i][16] = board[16][i] = 3;

    for(int i=0;i<15;++i)
        for(int j=0;j<15;++j)
            board[i+1][j+1]=now_chessboard[i][j];

    EVA eval;
    eval.score = 0;  //记录当前局势得分

    bool win = 0, lose = 0; //记录是否对局结束

    //判断横向棋型
    for(int i=1;i<=15;++i)
        for(int j=0;j<=11;++j){
            int score = tuple6[board[i][j]][board[i][j+1]][board[i][j+2]][board[i][j+3]][board[i][j+4]][board[i][j+5]];
            eval.score += score;
            if(score == WIN) win = 1;
            if(score == R_WIN) lose = 1;
        }

    //判断竖向棋型
    for(int j=1;j<=15;++j)
        for(int i=0;i<=11;++i){
            int score = tuple6[board[i][j]][board[i+1][j]][board[i+2][j]][board[i+3][j]][board[i+4][j]][board[i+5][j]];
            eval.score += score;
            if(score == WIN) win = 1;
            if(score == R_WIN) lose = 1;
        }

    //判断左上向右下棋型
    for(int i=0;i<=11;++i)
        for(int j=0;j<=11;++j){
            int score = tuple6[board[i][j]][board[i+1][j+1]][board[i+2][j+2]][board[i+3][j+3]][board[i+4][j+4]][board[i+5][j+5]];
            eval.score += score;
            if(score == WIN) win = 1;
            if(score == R_WIN) lose = 1;
        }

    //判断右上向左下棋型
    for(int i=0;i<=11;++i)
        for(int j=16;j>=5;--j){
            int score = tuple6[board[i][j]][board[i+1][j-1]][board[i+2][j-2]][board[i+3][j-3]][board[i+4][j-4]][board[i+5][j-5]];
            eval.score += score;
            if(score == WIN) win = 1;
            if(score == R_WIN) lose = 1;
        }

    if(win) eval.r = Win;
    else if(lose) eval.r = Lose;
    else eval.r = DRAW;

    return eval;
}

struct ePoint{
    QPoint p;
    int score;

    bool operator<(ePoint b) const
    {
        return score < b.score;
    }
};

POS _AI::chooseBest10(int board[15][15]){
    POS pos;

    int tboard[15][15];
    for(int i=0;i<15;++i)
        for(int j=0;j<15;++j)
        tboard[i][j] = board[i][j];

    std::priority_queue<ePoint> que;  //优先队列
    for(int i=0;i<15;++i)
        for(int j=0;j<15;++j)
        {
            if(tboard[i][j]!=0) continue;
            tboard[i][j] = 2; //0空,1黑子,2白子,3边界
            QPoint qp(i,j);
            ePoint ep;
            ep.p = qp;
            ep.score = evaluate(tboard).score;
            que.push(ep);
            tboard[i][j] = 0;
        }

    for(int i=0;i<10;++i)   //前10个分最高的局势
    {
        ePoint ep = que.top();
        que.pop();
        pos.pos[i] = ep.p;
        pos.score[i] = ep.score;
        qDebug()<< "坐标 " << ep.p.x() << " " << ep.p.x() << "   " << ep.score;
    }

    return pos;
}

int _AI::dfs(int board[15][15], int depth, int alpha, int beta){

    EVA eval = evaluate(board);

    //搜索到指定深度或者对局结束
    if(depth==DEPTH||eval.r!=DRAW){
        nodenum++;  //计数
        if(depth==DEPTH){
            POS pos = chooseBest10(board);
            qDebug()<< "到达DEPTH  " << pos.score[0];
            return pos.score[0];
        }
        else return eval.score;
    }
    //搜索到MAX层
    else if(depth % 2 == 0){

        POS pos = chooseBest10(board);

        //搜索最好的前10个
        for(int i=0;i<10;++i){

            int tmpBoard[15][15];   //复制棋盘
            for(int k1=0;k1<15;++k1)
                for(int k2=0;k2<15;++k2)
                tmpBoard[k1][k2] = board[k1][k2];

            //0空,1黑子,2白子,3边界
            tmpBoard[pos.pos[i].x()][pos.pos[i].y()] = 2;
            int a = dfs(tmpBoard,depth+1,alpha,beta);
            if(a>alpha){
                alpha=a;
                if(depth==0){  //记录下一步要走的位置
                    nextPos.setX(pos.pos[i].x());
                    nextPos.setY(pos.pos[i].y());
                    qDebug() << "set下一步  " << "i " << i << "坐标  " << pos.pos[i].x() << " " << pos.pos[i].y();
                }
            }

            //剪枝
            if(beta<=alpha)
                break;
        }

        return alpha;
    }
    //搜索到MIN层
    else{

        int rBoard[15][15];   //复制棋盘并将颜色对调
        for(int k1=0;k1<15;++k1)
            for(int k2=0;k2<15;++k2){
                if(board[k1][k2]==1) rBoard[k1][k2] = 2;
                else if(board[k1][k2]==2) rBoard[k1][k2] = 1;
                else rBoard[k1][k2] = board[k1][k2];
            }

        POS pos = chooseBest10(rBoard);
        for(int i=0;i<10;++i){

            int tmpBoard[15][15];   //复制棋盘
            for(int k1=0;k1<15;++k1)
                for(int k2=0;k2<15;++k2)
                tmpBoard[k1][k2] = board[k1][k2];

            //0空,1黑子,2白子,3边界
            tmpBoard[pos.pos[i].x()][pos.pos[i].y()] = 1;
            int a = dfs(tmpBoard,depth+1,alpha,beta);
            if(a<beta){
                beta=a;
            }

            //剪枝
            if(beta<=alpha)
                break;

        }

        return beta;
    }
}

