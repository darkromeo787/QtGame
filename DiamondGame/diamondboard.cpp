#include "diamondboard.h"
#include "qpainter.h"
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>
#include <QTime>
#include <QMessageBox>
#include <QInputDialog>
#include <QTableView>
#include <QHeaderView>
#include <QStandardItemModel>
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
    game->usedTime = 0;
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

    showChessBoard(game);
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

    showChessBoard(game);
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
        leftChess = 0;

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
    const char *debugFile = "debug.txt";
    FILE *f = fopen(debugFile, "a+");

    fputs("   1234567\n", f);
    int i, j;
    for(i=0;i<7;i++)
    {
        fputc('0'+i, f);
        fputc(':', f);
        fputc(' ', f);

        for(j=0;j<7;j++)
            fputc(game->state.board[i][j], f);
        fputc('\n', f);
    }

    fclose(f);

    return;


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

void saveRecord(diamond_game *game, int leftChess, int costTime)
{
    const char *debugFile = "record.txt";
    FILE *f = fopen(debugFile, "a+");

    fputs("   1234567\n", f);
    int i, j;
    for(i=0;i<7;i++)
    {
        fputc('0'+i, f);
        fputc(':', f);
        fputc(' ', f);

        for(j=0;j<7;j++)
            fputc(game->state.board[i][j], f);
        fputc('\n', f);
    }

    fclose(f);

}

void saveGame(diamond_game *game, int usedTime)
{
    game->usedTime = usedTime;
    const char *gameFile = "game.db";
    FILE *f = fopen(gameFile, "w");

    char *contentBuf;
    int gSize = sizeof(diamond_game);
    fwrite(game, gSize, 1, f);

    fclose(f);
}

void loadGame(diamond_game *game)
{
    const char *gameFile = "game.db";
    FILE *f = fopen(gameFile, "r");

    char *contentBuf;
    int gSize = sizeof(diamond_game);
    fread(game, gSize, 1, f);

    fclose(f);
}



// ======================================


DiamondBoard::DiamondBoard(diamond_game *game, QWidget *parent) :
	QWidget(parent)
{
    this->game = game;

    // 面板组件
    startTime = new QTime();
    timeLabel = new QLabel(this);
    backButton = new QPushButton("Back", this);
    exitButton = new QPushButton("Exit", this);
    saveButton = new QPushButton("Save", this);

    //菜单栏
    QMenuBar *mBar = new QMenuBar(this);
    QMenu *menu1 = mBar->addMenu("选择");
    QMenu *menu2 = mBar->addMenu("排行榜");
    QMenu *menu3 = mBar->addMenu("游戏说明");

    QAction *saveAction = menu1->addAction("保存");
    QAction *loadAction = menu1->addAction("载入");
    QAction *gameAction = menu1->addAction("新游戏");
    QAction *exitAction = menu1->addAction("保存并退出");

    QAction *rankAction = menu2->addAction("查看排行榜");
    QAction *helpAction = menu3->addAction("关于游戏");
    QAction *authorAction = menu3->addAction("关于作者");

//    connect(mBar, SIGNAL(triggered(QAction*)),this, SLOT(trigerMenu(QAction*)));
// 改为以下方式

    connect(saveAction, &QAction::triggered,
            [=](){ saveGame(game, usedTime); });

    connect(loadAction, &QAction::triggered,
            [=]() {
                    loadGame(game);

                    movefrom.x = -1;
                    movefrom.y = -1;

                    loadTime = game->usedTime;
                    *startTime = QTime::currentTime();

                    update();
    });

    connect(gameAction, &QAction::triggered,
            [=]() {
                    initGame(game);
                    initSet();

                    update();
    });

    connect(exitAction, &QAction::triggered,
            [=]() {
                    saveGame(game, usedTime);
                    this->close();
    });

    connect(rankAction, &QAction::triggered,
            [=]() {
                    getRankBoard();
    });

    connect(helpAction, &QAction::triggered,
            [=]() { aboutGame(); });

    connect(authorAction, &QAction::triggered,
            [=]() { aboutAuthor(); });

    // 位置、功能设置
    setTimeLabel(timeLabel);
    setBackButton(backButton);

    resize(800, 600);

    saveButton->setGeometry(600, 350, 100, 30);
    connect(saveButton, &QPushButton::clicked,
            [=](){ saveGame(game, usedTime); });


    exitButton->setGeometry(600, 400, 100, 30);
    // 退出游戏
    connect(exitButton, &QPushButton::clicked,
            [=](){ this->close(); });

    initSet();

}

/*
void DiamondBoard::trigerMenu(QAction *act)
{
    if (act->text()=="保存")
    {
        saveGame(game, usedTime);
    }
    else if (act->text()=="载入")
    {
        diamond_game game;
        loadGame(&game);
        *this->game = game;

        movefrom.x = -1;
        loadTime = game.usedTime;
        *startTime = QTime::currentTime();

        update();
    }
    else if (act->text()=="新游戏")
    {
        initGame(game);
        initSet();

        update();
    }
    else if (act->text()=="退出")
    {
        saveGame(game, usedTime);
        this->close();
    }
}
*/

void DiamondBoard::aboutGame()
{
    QMessageBox::about(this, "About Game", "        独立钻石是源于18世纪法国的宫廷贵族的自我挑战类单人棋游戏。棋盘是一个对称的“十字形”，纵横各有4条短线，3条长线，形成33个交点。\n      走棋的规则与跳棋基本一样，就是一枚棋子可以跳过一枚棋子然后落在空格里，如果还满足跳过棋子可以落在空格的条件，就连续跳到无法再跳为止，这个过程算一步。需要注意的是，行棋时，不许转弯跳或斜跳；被跳过的棋子必须从棋盘上取下来，算作被吃掉，最后在棋盘上留下来的棋子越少越好。");

}

void DiamondBoard::aboutAuthor()
{
    QMessageBox::about(this, "About Author", "by izcat @ cnblogs.com/izcat");
}

void DiamondBoard::getRankBoard()
{
    /* 创建表格视图 */
    QTableView *tableView = new QTableView();

    /* 设置表格视图大小 */
    tableView->resize(850, 300);

    /* 创建数据模型 */
    QStandardItemModel* model = new QStandardItemModel();

    /* 设置表格标题行(输入数据为QStringList类型) */
    model->setHorizontalHeaderLabels({"Level", "User Name", "Cost Time"});

    /* 自适应所有列，让它布满空间 */
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    /* 加载数据，每行有3列数据 */
    const char* LEVEL_TYPE[7] = {"天才", "大师", "尖子", "聪明", "很好", "较好", "一般"};
    game_rank_rec rec;
    int ret = loadRankingRecord(&rec);
    if (ret)
    {
        int line = 0;
        for (int i = 0; i < LEVEL_NUM; i++)
        {
            for (int j=0;j<rec.tot[i];j++)
            {

                model->setItem(line, 0, new QStandardItem(QString(LEVEL_TYPE[i])));
                model->setItem(line, 1, new QStandardItem(QString(rec.records[i][j].name)));
                model->setItem(line, 2, new QStandardItem(QString("%1").arg(rec.records[i][j].costTime)));
                ++line;
            }

        }
    }


    /* 设置表格视图数据 */
    tableView->setModel(model);

    /* 显示 */
    tableView->show();

}

void DiamondBoard::initSet()
{
    gameStart = false;
    gameEnd = false;
    usedTime = 0;
    loadTime = 0;

    movefrom.x = -1;
    movefrom.y = -1;
    moveto.x = -1;
    moveto.y = -1;

    timeLabel->setText("0s");
}

void DiamondBoard::setTimeLabel(QLabel *timeLabel)
{
    // 时间显示
    QFont font;
    font.setPointSize(25);

    timeLabel->setFont(font);
    timeLabel->setGeometry(600, 200, 100, 30);

    // 实时显示时间
    // 建立信号槽关系，1s后启动时间更新
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeUpdate()));
    timer->start(1000);
}

void DiamondBoard::setBackButton(QPushButton *button)
{
    // 悔棋
    connect(button, &QPushButton::clicked,
            [=](){ undoMove(this->game); movefrom.x=-1; update(); });
    button->setGeometry(600, 300, 100, 30);
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

    paintChess(painter, &game->state);


}

void DiamondBoard::paintChess(QPainter &painter, chess_state *cs)
{
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
    /*
    if (!gameStart)
    {
        startTime->start();
        gameStart = true;
    }

    if (gameEnd)
        return;

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

//    printf("from %d %d\n", x, y);
    */
}

void DiamondBoard::mouseReleaseEvent(QMouseEvent *me)
{
    if (!gameStart)
    {
        startTime->start();
        gameStart = true;
    }

    if (gameEnd)
        return;


    QPoint pt = me->pos();
    int x, y;
    bool ret = getRowCol(pt, x, y);

    if (!ret)  // 棋盘外
        return;

    if (movefrom.x==-1 && movefrom.y==-1 &&
        game->state.board[x][y]==CHESS ) // 第一次点击 是棋子
    {
        movefrom.x = x;
        movefrom.y = y;

    }
    else if (movefrom.x!=-1 && movefrom.y!=-1 &&
            game->state.board[x][y]==CHESS )// 第二次点击 是棋子
    {
        movefrom.x = x;
        movefrom.y = y;
    }
    else if (movefrom.x!=-1 && movefrom.y!=-1 &&
            game->state.board[x][y]!=CHESS )// 第二次点击 不是棋子
    {
        moveto.x = x;
        moveto.y = y;

        if (isValidMove(&(game->state), movefrom, moveto))
        {
            oneMove(game, movefrom, moveto);
            bool gameover = checkGameOver(game);
            if (gameover)
            {
                usedTime = startTime->elapsed()/1000 + usedTime;
                gameEnd = true;

                int level = getLeftChess(game);
                if (isNewRecord(level, usedTime))
                {
                    bool ok;
                    QString name = QInputDialog::getText(this, "新记录", "请输入用户名:", QLineEdit::Normal, "izcat", &ok);
                    if (ok)
                        saveOneRecord(level, usedTime, name.toStdString().c_str());
                }
            }
        }

        movefrom.x = -1;
        movefrom.y = -1;
    }
    else
    {
        return;
    }

//    printf("to %d %d\n", x, y);

    update();
}

void DiamondBoard::timeUpdate()
{
    if (!gameStart)
        return;

    if (gameEnd)
    {
        timeLabel->setText(QString::number(usedTime)+"s");
    }
    else
    {
        QTime currentTime = QTime::currentTime();
        usedTime = startTime->secsTo(currentTime) + loadTime;
        timeLabel->setText(QString::number(usedTime)+"s");
    }
}

