#include<stdio.h>
#include<stdlib.h>
#include<string.h>


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



void showRankingRecord(game_rank_rec *rec)
{
	printf("level | used time | name\n");

	int lev, cnt;
	game_record *gRec;

	for (lev=0;lev<LEVEL_NUM;lev++)
	{
		for (cnt=0;cnt<rec->tot[lev];cnt++)
		{
			gRec = &rec->records[lev][cnt];
			printf("%5d | %9d | %s\n", lev, gRec->costTime, gRec->name);
		}
	}

	printf("===============================\n");
}


// qsort排序比较函数
int recCmpFunc(const void* rec1, const void* rec2)
{
	if ((*(game_record*)rec1).level == (*(game_record*)rec2).level)
		return (*(game_record*)rec1).costTime > (*(game_record*)rec2).costTime;

	return (*(game_record*)rec1).level > (*(game_record*)rec2).level;
}

void loadRankingRecord(game_rank_rec *rec)
{
	const char *gameRecFile = "gameRec.db";
    FILE *f = fopen(gameRecFile, "r");

    if (f==NULL) // 第一次载入
    {
    	printf("No records\n");
    	return;
    }

    int gSize = sizeof(game_rank_rec);
    fread(rec, gSize, 1, f);

    // showRankingRecord(rec);

    fclose(f);
}

void saveRankingRecord(game_rank_rec *rec)
{
	const char *gameRecFile = "gameRec.db";
    FILE *f = fopen(gameRecFile, "w");

    int gSize = sizeof(game_rank_rec);
    fwrite(rec, gSize, 1, f);

    fclose(f);
}

int isNewRecord(int level, int costTime)
{
	game_rank_rec rec = {0};
	loadRankingRecord(&rec);

	int cnt = rec.tot[level]; // 记录条数
	if (cnt<RECORD_NUM)
		return 1;

	// 已有10条 判断是否比最后的记录小
	printf("10 full\n");
	if (costTime < rec.records[level][cnt-1].costTime)
		return 1;
	else
		return 0;
}

// 先调用 isNewRecord 判断是否需要新的记录
void saveOneRecord(int level, int costTime, char *name)
{
	if (!isNewRecord(level, costTime))
	{
		printf("不是新记录\n");
		return;
	}

	// game->usedTime = costTime;
	game_rank_rec rec = {0};
	loadRankingRecord(&rec);

	// 插入新的记录
	int tot = rec.tot[level];
	// 覆盖末尾的记录
	if (tot==RECORD_NUM) --tot;
	// rec.records[level][tot] = (game_record){level, costTime, name};
	rec.records[level][tot] = (game_record){level, costTime};
	strcpy(rec.records[level][tot].name, name);
	rec.tot[level] = ++tot;;

	// 该等级进行排序
	qsort(rec.records[level], tot, sizeof(game_record), recCmpFunc);

	// showRankingRecord(&rec);
	// 保存记录
    saveRankingRecord(&rec);
}



int main() 
{
	game_rank_rec rec = {0};
	// rec.tot[1] = 1;
	// rec.records[1][0] = (game_record){1, 25};
	// strcpy(rec.records[1][0].name, "zong");

	// rec.tot[3] = 2;
	// rec.records[3][0] = (game_record){3, 525, "fazong"};
	// rec.records[3][1] = (game_record){3, 325, "zfewong"};

	saveOneRecord(5, 32325, "zfewong");
	// loadRankingRecord(&rec);
	// showRankingRecord(&rec);

	saveOneRecord(1, 51325, "zfewong");
	// loadRankingRecord(&rec);
	// showRankingRecord(&rec);

	saveOneRecord(5, 25, "zfewong");
	loadRankingRecord(&rec);
	showRankingRecord(&rec);

	return 0;
}

