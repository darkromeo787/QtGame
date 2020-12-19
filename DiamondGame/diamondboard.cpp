#include "diamondboard.h"
#include "qpainter.h"
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <stdio.h>

void initChessBoard(chess_state *cs)
{
    int i, j;
    for(i=0;i<7;i++)
        for(j=0;j<7;j++)
            cs->board[i][j] = BOUND;

    for(i=2;i<5;i++)
        for(j=0;j<7;j++)
        {
            cs->board[i][j] = CHESS;
            cs->board[j][i] = CHESS;
        }
    cs->board[3][3] = BLANK;
}

void initGame(diamond_game *game)
{
    initChessBoard(&(game->state));
    game->history.front = 0;
    game->history.rear = 0;
}

int isValidMove(chess_state *cs, coordinate from, coordinate to)
{
    int dx = to.x - from.x;
    int dy = to.y - from.y;

    if ((dx==0 && (dy==2||dy==-2)) ||
        (dy==0 && (dx==2||dx==-2)))
    {
        coordinate mid;
        mid.x = (from.x + to.x) / 2;
        mid.y = (from.y + to.y) / 2;

        if (cs->board[mid.x][mid.y]==CHESS &&
            cs->board[from.x][from.y]==CHESS &&
            cs->board[to.x][to.y]==BLANK)
            return 1;
        else
            return 0;
    }
    else
    {
        return 0;
    }
}

void oneMove(diamond_game *game, coordinate from, coordinate to)
{
    // 保存上一步棋盘
    int index = game->history.front;
    int rear = game->history.rear;
    int nextIndex = (index + 1) % HISTORY_RECORD_NUM;
    if (nextIndex==rear) // 循环队列已满
    {
        printf("CircleQueue is Full\n");
        game->history.rear = (rear + 1) % HISTORY_RECORD_NUM;
    }

    game->history.backup[index] = game->state;
    game->history.front = nextIndex ;

    coordinate mid;
    mid.x = (from.x + to.x) / 2;
    mid.y = (from.y + to.y) / 2;
    game->state.board[from.x][from.y] = BLANK;
    game->state.board[mid.x][mid.y] = BLANK;
    game->state.board[to.x][to.y] = CHESS;
}

void undoMove(diamond_game *game)
{
    int index = game->history.front;
    int rear = game->history.rear;
    int lastIndex = (index - 1 + HISTORY_RECORD_NUM) % HISTORY_RECORD_NUM;
    if (index==rear) // 循环队列为空
    {
        printf("Circle Queue is Blank\n"); // 历史记录为空/最多返回10步
        return;
    }

    game->state = game->history.backup[lastIndex];
    game->history.front = lastIndex;
}

int checkGameOver(diamond_game *game)
{
    int i, j;
    int can_move = 0;
    for(i=0;i<7;i++)
    {
        for(j=0;j+2<7;j++)
        {
            if(game->state.board[i][j]==CHESS &&
               game->state.board[i][j+1]==CHESS &&
               game->state.board[i][j+2]==BLANK)
                can_move = 1;

            if(game->state.board[i][j+2]==CHESS &&
               game->state.board[i][j+1]==CHESS &&
               game->state.board[i][j]==BLANK)
                can_move = 1;
        }
    }

    for(i=0;i+2<7;i++)
    {
        for(j=0;j<7;j++)
        {
            if(game->state.board[i][j]==CHESS &&
               game->state.board[i+1][j]==CHESS &&
               game->state.board[i+2][j]==BLANK)
                can_move = 1;

            if(game->state.board[i+2][j]==CHESS &&
               game->state.board[i+1][j]==CHESS &&
               game->state.board[i][j]==BLANK)
                can_move = 1;
        }
    }

    return !can_move;
}

int getLeftChess(diamond_game *game)
{
    int i, j;
    int leftChess = 0;
    for(i=0;i<7;i++)
    {
        for(j=0;j<7;j++)
        {
            if(game->state.board[i][j]==CHESS)
                ++leftChess;
        }
    }

    if (leftChess==1 && game->state.board[3][3]==CHESS)
        leftChess = -1;

    return leftChess;
//    switch (leftChess)
//    {
//        case -1:
//            printf("Genius\n"); // 天才
//            break;
//        case 1:
//            printf("Master\n"); // 大师
//            break;
//        case 2:
//            printf("Jianzi\n"); // 尖子
//            break;
//        case 3:
//            printf("Smart\n"); // 聪明
//            break;
//        case 4:
//            printf("Terrific\n"); //很好
//            break;
//        case 5:
//            printf("Good\n"); // 较好
//            break;
//        case 6:
//        default :
//            printf("Normal\n"); // 一般
//            break;
//    }
}

void showChessBoard(diamond_game *game)
{
    int i, j;
    putchar(' ');
    putchar(' ');
    putchar(' ');
    for (i=0;i<7;i++)
        printf("%d", i);
    putchar('\n');

    for(i=0;i<7;i++)
    {
        printf("%d: ", i);
        for(j=0;j<7;j++)
            putchar(game->state.board[i][j]);
        putchar('\n');
    }

    printf("======================\n");
}

void saveRecord(int leftChess, int costTime)
{

}

DiamondBoard::DiamondBoard(diamond_game *game, QWidget *parent) :
	QWidget(parent)
{
    startTime = new QTime();
    gameStart = false;

    timeLabel = new QLabel(this);

    backButton = new QPushButton("Back", this);
    exitButton = new QPushButton("Exit", this);
    saveButton = new QPushButton("Save", this);

    this->game = game;
    resize(800, 550);

    timeLabel->setGeometry(600, 200, 100, 30);
    timeLabel->setText("0.00s");

    backButton->setGeometry(600, 300, 100, 30);
    // 悔棋
    connect(backButton, &QPushButton::clicked,
            [=](){ undoMove(this->game); movefrom.x = -1; update(); });

    saveButton->setGeometry(600, 350, 100, 30);


    exitButton->setGeometry(600, 400, 100, 30);
    // 退出游戏
    connect(exitButton, &QPushButton::clicked,
            [=](){ this->close(); });
}


void DiamondBoard::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
	int d = chessSpan;

    // 棋盘边界
	for (int i=0;i<8;i++)
	{
        // 横线
		if (i<2 || i>5)
		{
            painter.drawLine(QPoint(leftGap + 2*d, topGap + i*d), QPoint(leftGap + 5*d, topGap + i*d));
		}
		else
		{
            painter.drawLine(QPoint(leftGap, topGap + i*d), QPoint(leftGap + 7*d, topGap + i*d));
		}

        // 竖线
        if (i<2 || i>5)
        {
            painter.drawLine(QPoint(leftGap + i*d, topGap + 2*d), QPoint(leftGap + i*d, topGap + 5*d));
        }
        else
        {
            painter.drawLine(QPoint(leftGap + i*d, topGap), QPoint(leftGap + i*d, topGap + 7*d));
        }
	}

    paintChess(&game->state);


    timeLabel->setText(QString::number(startTime->elapsed()/1000)+"s");
}

void DiamondBoard::paintChess(chess_state *cs)
{
    QPainter painter(this);
    char chess_type;

    for (int i=0;i<7;i++)
    {
        for (int j=0;j<7;j++)
        {
            chess_type = cs->board[i][j];

            if (chess_type==BLANK)
                continue;

            if (chess_type==CHESS)
            {
                if (movefrom.x==i && movefrom.y==j)
                    paintCoin(painter, i, j, QColor(255, 0, 0));
                else
                    paintCoin(painter, i, j, QColor(0, 0, 0));
            }
        }
    }
}

void DiamondBoard::paintCoin(QPainter &painter, int x, int y, QColor color)
{
    QPoint circle_o = chessPosition(x, y);
    QRect rect(circle_o.x() - chessRadius, circle_o.y() - chessRadius,
               chessRadius*2, chessRadius*2);

    painter.drawEllipse(circle_o, chessRadius, chessRadius);
    QPainterPath path;
    path.moveTo(circle_o.x(), circle_o.y());
    path.arcTo(rect, 0, 360);
    painter.fillPath(path, QBrush(color));
}

bool DiamondBoard::getRowCol(QPoint pt, int &x, int &y)
{
    for (x=0;x<7;x++)
    {
        for (y=0;y<7;y++)
        {
            QPoint c = chessPosition(x, y);
            int dx = c.x() - pt.x();
            int dy = c.y() - pt.y();
            int dist = dx*dx + dy*dy;
            if (dist< chessRadius*chessRadius)
            {
                return true;
            }
        }
    }

    return false;
}

void DiamondBoard::mousePressEvent(QMouseEvent *me)
{
    if (!gameStart)
    {
        startTime->start();
        gameStart = true;
    }

    QPoint pt = me->pos();
    int x, y;
    bool ret = getRowCol(pt, x, y);

    if (!ret)  // 棋盘外
        return;

    if (game->state.board[x][y]==CHESS)
    {
        movefrom.x = x;
        movefrom.y = y;
    }

    update();

    printf("from %d %d\n", x, y);
}

void DiamondBoard::mouseReleaseEvent(QMouseEvent *me)
{
    QPoint pt = me->pos();
    int x, y;
    bool ret = getRowCol(pt, x, y);

    if (!ret)  // 棋盘外
        return;

    if (game->state.board[x][y]==BLANK)
    {
        moveto.x = x;
        moveto.y = y;
        if (isValidMove(&(game->state), movefrom, moveto))
        {
            oneMove(game, movefrom, moveto);
            bool gameover = checkGameOver(game);
            if (gameover)
            {

                int k = getLeftChess(game);
                saveRecord(k, startTime->elapsed());
            }
        }
    }
    else
    {
        movefrom.x = -1;
        movefrom.x = -1;
    }
    printf("to %d %d\n", x, y);

    update();
}


