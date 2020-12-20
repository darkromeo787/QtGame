#include "record.h"
#include <stdio.h>

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

int loadRankingRecord(game_rank_rec *rec)
{
    const char *gameRecFile = "gameRec.db";
    FILE *f = fopen(gameRecFile, "r");

    if (f==NULL) // 第一次载入 无数据返回0
    {
        printf("No records\n");
        return 0;
    }

    int gSize = sizeof(game_rank_rec);
    fread(rec, gSize, 1, f);

    // showRankingRecord(rec);

    fclose(f);

    return 1;
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
void saveOneRecord(int level, int costTime, const char *name)
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
