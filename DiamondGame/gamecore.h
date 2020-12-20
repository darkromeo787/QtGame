#ifndef GAMECORE_H
#define GAMECORE_H

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


void initChessBoard(chess_state *cs);
void initGame(diamond_game *game);

int isValidMove(chess_state *cs, coordinate from, coordinate to);
void oneMove(diamond_game *game, coordinate from, coordinate to);
void undoMove(diamond_game *game);

int checkGameOver(diamond_game *game);
int getLeftChess(diamond_game *game);
void showChessBoard(diamond_game *game);

void saveGame(diamond_game *game, int usedTime);
void loadGame(diamond_game *game);


#endif // GAMECORE_H
