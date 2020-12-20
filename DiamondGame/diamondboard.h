#ifndef DIAMONDBOARD_H
#define DIAMONDBOARD_H

#include <QBasicTimer>
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <time.h>

#define CHESS 'o'
#define BOUND 'x'
#define BLANK '-'
#define HISTORY_RECORD_NUM (10+1) // 循环队列长度+1


// 坐标
typedef struct
{
    int x, y;
}coordinate;

// 棋盘
typedef struct
{
    char board[7][7];
}chess_state;

// 历史状态
typedef struct
{
    chess_state backup[HISTORY_RECORD_NUM];
    int front;
    int rear;
}history_state;

// 钻石游戏
typedef struct
{
    chess_state   state;
    history_state history;
    int usedTime;
}diamond_game;

#define LEVEL_NUM  6
#define RECORD_NUM 10
typedef struct
{
    int level;
    int costTime;
    char name[30];
}game_record;


typedef struct
{
    game_record records[LEVEL_NUM][RECORD_NUM]; // 六个等级 每个等级记录前十名
    int tot[LEVEL_NUM]; // 每个等级记录条数
}game_rank_rec;


void initChessBoard(chess_state *cs);
void initGame(diamond_game *game);
int isValidMove(chess_state *cs, coordinate from, coordinate to);
void oneMove(diamond_game *game, coordinate from, coordinate to);
void undoMove(diamond_game *game);
int checkGameOver(diamond_game *game);
void showChessBoard(diamond_game *game);
void saveRecord(int leftChess, int costTime);

void rank(diamond_game *game);

class DiamondBoard : public QWidget
{
    Q_OBJECT

public:
    explicit DiamondBoard(diamond_game *game, QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

public slots: // 实时显示时间
    void timeUpdate(void);

private:
    int leftGap = 50;
    int topGap = 50;
    int chessSpan = 70;
    int chessRadius = 18;

    diamond_game *game;
    coordinate movefrom, moveto;

    QTime *startTime;
    int usedTime, loadTime;  // 本局游戏用时 载入游戏用时
    bool gameStart;
    bool gameEnd;


    QPushButton *backButton, *exitButton, *saveButton;
    QLabel *timeLabel;

    void initSet();
    void setTimeLabel(QLabel *timeLabel);
    void setBackButton(QPushButton *button);
    void aboutAuthor();
    void aboutGame();
    void getRankBoard();
//    void trigerMenu(QAction* act);

    void paintChess(QPainter &painter, chess_state *cs);
    void paintCoin(QPainter &painter,int x, int y, QColor color);
    bool getRowCol(QPoint pt, int &x, int &y);
    QPoint chessPosition(int x, int y)
    { return QPoint(leftGap+x*chessSpan+chessSpan/2, topGap+y*chessSpan+chessSpan/2); }
signals:

public slots:

};



#endif // DIAMONDBOARD_H
