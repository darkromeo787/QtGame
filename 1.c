#include<stdio.h>
#include<stdlib.h>

#define CHESS 'O'
#define BOUND 'X'
#define BLANK '-'
#define HISTORY_RECORD_NUM 10

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

	if (dx==0 && (dy==2||dy==-2) ||
		dy==0 && (dx==2||dx==-2))
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

void move(diamond_game *game, coordinate from, coordinate to)
{
	// 保存上一步棋盘
	int index = game->history.front;
	int rear = game->history.rear;
	int nextIndex = (index + 1) % HISTORY_RECORD_NUM;
	if (nextIndex==rear) // 循环队列已满
	{
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

void undo(diamond_game *game)
{
	int index = game->history.front;
	int rear = game->history.rear;
	int lastIndex = (index - 1) % HISTORY_RECORD_NUM;
	if (index==rear) // 循环队列为空
	{
		printf("历史记录为空/最多返回10步\n");
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

void rank(diamond_game *game)
{
	int i, j;
	int level = 0;
	for(i=0;i<7;i++)
	{
		for(j=0;j<7;j++)
		{
			if(game->state.board[i][j]==CHESS)
				++level;
		}
	}

	if (level==1 && game->state.board[3][3]==CHESS)
		level = -1;
	switch (level)
	{
		case -1:
			printf("Genius\n"); // 天才
			break;
		case 1:  
			printf("Master\n"); // 大师
			break;
		case 2:
			printf("Jianzi\n"); // 尖子
			break;
		case 3:
			printf("Smart\n"); // 聪明
			break;
		case 4:
			printf("Terrific\n"); //很好
			break;
		case 5:
			printf("Good\n"); // 较好
			break;
		case 6:
		default :
			printf("Normal\n"); // 一般
			break;
	}
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




int main()
{
	diamond_game game;
	diamond_game *g = &game;
	// initChessBoard(&(g->state));
	initGame(g);
	showChessBoard(g);

	while(!checkGameOver(g)) 
	{
		coordinate from, to;
		scanf("%d %d %d %d", &from.x, &from.y, &to.x, &to.y);

		if (isValidMove(&(g->state), from, to))
		{
			move(g, from, to);
			showChessBoard(g);
		}
		else
		{
			printf("Invalid Move\n"); // 移动无效
		}

	}
	printf("Game Over\nYou Are");
	rank(g);
	return 0;
}
