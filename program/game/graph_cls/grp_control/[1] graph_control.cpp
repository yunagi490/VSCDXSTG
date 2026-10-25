#include <stdio.h>
#include <map>
#include <string>
#include "DxLib.h"
#include "[4] define.h"
#include "[1] graph_control.h"

bool ResolvePath(const char *name, char *out, size_t outSize)
{
    // 実行場所が program/system でも、リポジトリ直下でも見つかるようにする
    static const char *prefix[] = {"", "../", "../../", "../../../",
                                   "program/", "../program/", "../../program/"};
    for (size_t i = 0; i < sizeof(prefix) / sizeof(prefix[0]); ++i)
    {
        snprintf(out, outSize, "%s%s", prefix[i], name);
        FILE *fp = fopen(out, "rb");
        if (fp)
        {
            fclose(fp);
            return true;
        }
    }
    return false;
}

const VS_GRAPH &GRP_CTL::Get(const char *name)
{
    static std::map<std::string, VS_GRAPH> cache;
    std::map<std::string, VS_GRAPH>::iterator it = cache.find(name);
    if (it != cache.end())
        return it->second;

    VS_GRAPH g = {-1, 1, 1};
    char path[512];
    if (ResolvePath(name, path, sizeof(path)))
    {
        g.gh = LoadGraph(path);
        if (g.gh >= 0)
            GetGraphSize(g.gh, &g.w, &g.h);
    }
    return cache.insert(std::make_pair(std::string(name), g)).first->second;
}

void GRP_CTL::DrawCenter(const VS_GRAPH &g, double x, double y, double size)
{
    if (g.gh < 0)
    {
        // 画像が無い時は丸で代用
        DrawCircle((int)x, (int)y, (int)(size / 2), GetColor(255, 0, 255), FALSE);
        return;
    }
    DrawRotaGraphF((float)x, (float)y, size / g.w, 0.0, g.gh, TRUE);
}