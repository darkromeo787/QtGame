#include "gamecore.h"
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


void saveGame(diamond_game *game, int usedTime)
{
    game->usedTime = usedTime;
    const char *gameFile = "game.db";
    FILE *f = fopen(gameFile, "w");

    int gSize = sizeof(diamond_game);
    fwrite(game, gSize, 1, f);

    fclose(f);
}

void loadGame(diamond_game *game)
{
    const char *gameFile = "game.db";
    FILE *f = fopen(gameFile, "r");

    int gSize = sizeof(diamond_game);
    fread(game, gSize, 1, f);

    fclose(f);
}
