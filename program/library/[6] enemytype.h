#ifndef _ENEMYTYPE_H
#define _ENEMYTYPE_H

#include <vector>
#include "[4] define.h"

// Enemy.csv を読み込み、出現時間順に並べる。読めなければ false
bool LoadEnemyCSV(const char *name, std::vector<ENEMYDATA> &out);

// CSVが無い時の代わりの敵配置
void MakeDefaultEnemyData(std::vector<ENEMYDATA> &out);

#endif