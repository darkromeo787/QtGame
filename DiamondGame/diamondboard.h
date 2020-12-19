#ifndef DIAMONDBOARD_H
#define DIAMONDBOARD_H

#include <QBasicTimer>
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QTime>
#include <QLabel>

#define CHESS 'O'
#define BOUND 'X'
#define BLANK '-'
#define HISTORY_RECORD_NUM 10+1 // 循环队列长度+1

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
}diamond_game;

void initChessBoard(chess_state *cs);
void initGame(diamond_game *game);
int isValidMove(chess_state *cs, coordinate from, coordinate to);
void oneMove(diamond_game *game, coordinate from, coordinate to);
void undoMove(diamond_game *game);
int checkGameOver(diamond_game *game);
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


private:
    int leftGap = 20;
    int topGap = 20;
    int chessSpan = 70;
    int chessRadius = 18;

    diamond_game *game;
    coordinate movefrom, moveto;
    QTime *startTime;
    bool gameStart;


    QPushButton *backButton, *exitButton, *saveButton;
    QLabel *timeLabel;

    void paintChess(chess_state *cs);
    void paintCoin(QPainter &painter,int x, int y, QColor color);
    bool getRowCol(QPoint pt, int &x, int &y);
    QPoint chessPosition(int x, int y)
    { return QPoint(leftGap+x*chessSpan+chessSpan/2, topGap+y*chessSpan+chessSpan/2); }
signals:

public slots:

};



#endif // DIAMONDBOARD_H
