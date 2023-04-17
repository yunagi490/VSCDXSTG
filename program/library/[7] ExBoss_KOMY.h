#ifndef _ExBoss_KOMY
#define _ExBoss_KOMY

class ExBoss_KOMY
{
private:
    // 座標とグラフィックハンドル
    double x, y;
    int gh[3];

    // 画像サイズ
    int width, height;

    // 敵の種類
    int EBK_type;

    // 弾の種類
    int EBK_Stype;

    // 移動パターン
    int EBKs_pattern;
}