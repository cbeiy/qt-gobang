#include "gobang.h"
#include "ui_gobang.h"

gobang::gobang(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::gobang)
{
    ui->setupUi(this);

    for(int i=0;i<15;++i){
        for(int j=0;j<15;++j){
            qchessboard[i][j].setX(20+30*i);
            qchessboard[i][j].setY(20+30*j);
        }
    }
    connect(this->ui->btn_return,SIGNAL(clicked(bool)),this,SLOT(on_btn_return_clicked()));

    setMouseTracking(true);
    initializeGame();
}

gobang::~gobang()
{
    delete ui;
}

void gobang::on_btn_return_clicked()
{
    emit returnSignal();
    this->hide();
    initializeGame();
}

void gobang::initializeGame(){

    memset(ai.chessboard,0,sizeof(ai.chessboard));

    status = UNDERWAY;
    turn = T_BLACK;
    cursorRow = -1;
    cursorCol = -1;
}

bool gobang::chessOneByPlayer(){
    if(ai.chessboard[cursorRow][cursorCol]==0){
        oneChessMove(cursorRow,cursorCol);
        return true;
    }
    return false;
}


void gobang::chessOneByAi(){

    struct timeval tpstart,tpend;
    float timeuse;
    gettimeofday(&tpstart,NULL);

    ai.nodenum=0;

    ai.dfs(ai.chessboard,0,-INT_MAX,INT_MAX);

    QPoint p=ai.nextPos;
    qDebug() << "下一步  " << p.x() << " " << p.y();

    if(isLegalMove(p.x(),p.y()))
        oneChessMove(p.x(),p.y());
    else qDebug()<<"ai下标不合法！";

    qDebug()<<"ai所求局势得分:"<<ai.evaluate(ai.chessboard).score;

    gettimeofday(&tpend,NULL);
    timeuse=(1000000*(tpend.tv_sec-tpstart.tv_sec) + tpend.tv_usec-tpstart.tv_usec)/1000000.0;
    qDebug()<<timeuse<<"s";

    QString text="ai计算耗时:"+QString::number(timeuse)+"s";
    this->ui->lb_timeuse->setText(text);

    text="ai叶结点数:"+QString::number(ai.nodenum);
    this->ui->lb_nodeNum->setText(text);

    text="ai局面估分:"+QString::number(ai.evaluate(ai.chessboard).score);
    this->ui->lb_eval->setText(text);
}

void gobang::oneChessMove(int row, int col){

    if(turn==T_BLACK){
        turn=T_WHITE;
        ai.chessboard[row][col]=1;
    }
    else{
        turn=T_BLACK;
        ai.chessboard[row][col]=2;
    }

    Result result=ai.evaluate(ai.chessboard).r;

    QMessageBox msg;
    msg.setIcon(QMessageBox::Critical);
    msg.setStandardButtons(QMessageBox::Yes);
    if(result!=DRAW){
        status=FINISH;
        if(result==Win){
            msg.setText("黑棋赢");
            score_black++;
        }
        else {
            msg.setText("白棋赢");
            score_white++;
        }
        msg.exec();

        ui->lcd_black->display(score_black);
        ui->lcd_write->display(score_white);
    }
    else if(isDeadGame()){
        status=FINISH;
        msg.setText("平局");
        msg.exec();
    }
    update();
}


bool gobang::isDeadGame(){
    int chessNum=0;
    for(int i=0;i<16;++i)
        for(int j=0;j<16;++j)
            if(ai.chessboard[i][j]!=0) chessNum++;
    if(chessNum==16*16) return true;
    else return false;
}
bool gobang::isLegalMove(int row, int col){
    if(ai.chessboard[row][col]==0)return true;
    else return false;
}

void gobang::mouseReleaseEvent(QMouseEvent *event){
    if(mode==PLAYER){
        if(chessOneByPlayer()){
            if(status==FINISH) initializeGame();
        }
    }
    else{
        if(chessOneByPlayer()){
            if(status==UNDERWAY){
                chessOneByAi();
                if(status==FINISH)initializeGame();
            }
            else initializeGame();
        }
    }
}

void gobang::mouseMoveEvent(QMouseEvent *event){
    if(event->x()>=5&&event->x()<=455&&event->y()>=5&&event->y()<=455){//5=20-15,455=20+14*30+15
        setCursor(Qt::BlankCursor);
        for(int i=0;i<15;++i)
            for(int j=0;j<15;++j){
                float x=event->x(),y=event->y();
                //判断鼠标落在哪一个点附近(正方形范围)
                if((x>=(qchessboard[i][j].x()-15))&&(x<(qchessboard[i][j].x()+15))&&
                   (y>=(qchessboard[i][j].y()-15))&&(y<(qchessboard[i][j].y()+15))){
                    cursorRow=j;
                    cursorCol=i;
                    if(ai.chessboard[cursorRow][cursorCol]!=0)
                        setCursor(Qt::ForbiddenCursor);

                    //展示图标坐标
                    QString str="坐标:";
                    str+=QString::number(j);
                    str+=",";
                    str+=QString::number(i);
                    if(turn==T_BLACK)ui->lb_black_position->setText(str);
                    else ui->lb_white_position->setText(str);
                    break;
                }
            }
    }
    else setCursor(Qt::ArrowCursor);
    update();
}


void gobang::paintEvent(QPaintEvent *event){

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //画棋盘
    painter.setPen(Qt::black);
    for(int i=0;i<15;++i){
        painter.drawLine(qchessboard[0][i],qchessboard[14][i]);
        painter.drawLine(qchessboard[i][0],qchessboard[i][14]);
    }
    if(turn==T_BLACK)painter.setBrush(Qt::black);
    else painter.setBrush(Qt::white);
    //画鼠标光标
    if(cursorRow!=-1&&cursorCol!=-1){
        //8为光标边长
        QRect rec(qchessboard[cursorCol][cursorRow].x()-8/2,qchessboard[cursorCol][cursorRow].y()-8/2,8,8);
        painter.drawRect(rec);
    }

    //画棋子
    for(int i=0;i<15;++i){
        for(int j=0;j<15;++j){
            if(ai.chessboard[i][j]!=0){
                if(ai.chessboard[i][j]==1)painter.setBrush(Qt::black);
                else painter.setBrush(Qt::white);
                painter.drawEllipse(qchessboard[j][i].x()-20/2,qchessboard[j][i].y()-20/2,20,20);

            }
        }
    }
}

void gobang::setGameMode(gMode m){
    mode=m;
}
