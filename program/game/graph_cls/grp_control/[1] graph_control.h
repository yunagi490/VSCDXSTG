#ifndef _GRAPH_CONTROL
#define _GRAPH_CONTROL

// 読み込み済み画像の情報
struct VS_GRAPH
{
    int gh; // グラフィックハンドル (読み込み失敗時は -1)
    int w, h;
};

// 画像管理クラス : 同じファイルは1回しか読み込まない
class GRP_CTL
{
public:
    static const VS_GRAPH &Get(const char *name);
    // 中心(x,y)に横幅 size ピクセルで描画する
    static void DrawCenter(const VS_GRAPH &g, double x, double y, double size);
};
#endif