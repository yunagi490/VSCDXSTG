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
    int EBKm_pattern;

    // ショットパターン
    int EBKs_pattern;

    // HP
    int hp;
    // アイテム
    int item;

    // ボスが出現してからのカウント
    int EBK_count;
    // 発射した段数
    int EBK_num;

    // 発射直後のラジアン
    double rad;

    // ボス消滅デフラグ
    bool dead_EXBFlag;
    bool end_EXBFlag;

    // 弾構造体
    EBK_SHOT ebk_shot[ExBossK_SNUM];

    // ショットが撃てるようになったかのデフラグ
    bool EXB_sflag;
    // ショットが撃てるようになってからのカウント
    int EXB_scount;

private:
    bool ExB_All();
    void ExB_Move();
    void ExB_Draw();
    bool ExB_OutCheck();
    bool ExB_sOutCheck();

public:
    KOMY();
    KOMY(char *c_filename, int type, int m_pattern, int x, int y,
         int in_time, int stop_time, int shot_time, int out_time,
         char *s_filename, int stype, int s_pattern, int speed);
};
#endif