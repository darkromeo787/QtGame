#ifndef RECORD_H
#define RECORD_H

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

    int isNewRecord(int level, int costTime);
    void saveOneRecord(int level, int costTime, const char *name);
    int recCmpFunc(const void* rec1, const void* rec2);

    void saveRankingRecord(game_rank_rec *rec);
    int loadRankingRecord(game_rank_rec *rec);
    void showRankingRecord(game_rank_rec *rec);


#endif // RECORD_H
