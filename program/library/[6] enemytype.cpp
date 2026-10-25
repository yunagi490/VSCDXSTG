#include <stdio.h>
#include <algorithm>
#include "[6] enemytype.h"

static bool EarlierIn(const ENEMYDATA &a, const ENEMYDATA &b)
{
    return a.in_time < b.in_time;
}

bool LoadEnemyCSV(const char *name, std::vector<ENEMYDATA> &out)
{
    out.clear();

    char path[512];
    if (!ResolvePath(name, path, sizeof(path)))
        return false;
    FILE *fp = fopen(path, "r");
    if (!fp)
        return false;

    char line[512];
    while (fgets(line, sizeof(line), fp))
    {
        // 13列そろった行だけ読む (ヘッダ行は数値ではないので自然に飛ばされる)
        ENEMYDATA d;
        int n = sscanf(line, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                       &d.type, &d.stype, &d.m_pattern, &d.s_pattern,
                       &d.in_time, &d.stop_time, &d.shot_time, &d.out_time,
                       &d.x, &d.y, &d.speed, &d.hp, &d.item);
        if (n == 13)
            out.push_back(d);
    }
    fclose(fp);

    std::stable_sort(out.begin(), out.end(), EarlierIn);
    return !out.empty();
}

void MakeDefaultEnemyData(std::vector<ENEMYDATA> &out)
{
    out.clear();
    for (int i = 0; i < 5; ++i)
    {
        ENEMYDATA d = {0, 0, 0, 0, 60 + 60 * i, 120 + 60 * i, 121 + 60 * i,
                       300 + 60 * i, 50 + 50 * i, -40, 4, 1, 0};
        out.push_back(d);
    }
}