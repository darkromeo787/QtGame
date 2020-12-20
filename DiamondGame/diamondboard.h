#ifndef DIAMONDBOARD_H
#define DIAMONDBOARD_H

#ifdef __cplusplus

extern "C" {
#include "gamecore.h"
#include "record.h"
#endif

#ifdef __cplusplus
}

#include <QBasicTimer>
#include <QWidget>
#include <QMouseEvent>
#include <QPushButton>
#include <QLabel>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <time.h>


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
    const int leftGap = 50;     // 棋盘左边空白大小
    const int topGap = 50;      // 棋盘顶部空白大小
    const int chessSpan = 70;   // 棋子间隔空白大小
    const int chessRadius = 18; // 棋子大小

    diamond_game *game;
    coordinate movefrom, moveto;

    QTime *startTime;
    int usedTime, loadTime;  // 本局游戏用时 载入游戏用时
    bool gameStart;
    bool gameEnd;

    QPushButton *backButton, *exitButton, *saveButton;
    QLabel *timeLabel;

    // 初始化界面
    void initSet();
    void setMenuBar();
    void setTimeLabel();
    void setButtons();


    void aboutAuthor();
    void aboutGame();
    void getRankBoard();

    void paintChess(QPainter &painter, chess_state *cs);
    void paintCoin(QPainter &painter,int x, int y, QColor color);
    bool getRowCol(QPoint pt, int &x, int &y);
    QPoint chessPosition(int x, int y)
    { return QPoint(leftGap+x*chessSpan+chessSpan/2, topGap+y*chessSpan+chessSpan/2); }
signals:

public slots:

};


#endif

#endif // DIAMONDBOARD_H
