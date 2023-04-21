#include "[7] ExBoss_KOMY.h"
#include "[4] define.h"
#include "[6] BossEnemy.h"
#include "DxLib.h"

ExBoss_KOMY::KOMY(char *c_filename, int type, int m_pattern, int x, int y,
                  int in_time, int stop_time, int shot_time, int out_time,
                  char *s_filename, int stype, int s_pattern, int speed)
{
    // サイズ
    width = 128;
    height = 128;

    // 敵の種類と弾の種類
    this->EBK_type = type;
    this->EBK_Stype = stype;

    // 移動パターンとショットパターン
    this->EBKm_pattern = m_pattern;
    this->EBKs_pattern = s_pattern;

    // 座標セット
    this->x = x;
    this->y = y;
    /*
        // 出現、停止、発射、帰還時間セット
        this->in_time = in_time;
        this->stop_time = stop_time;
        this->shot_time = shot_time;
        this->out_time = out_time;
    */
    this->hp = hp;
    this->item = item;

    // 敵画像読み込み
    if (type == 0)
    {
        LoadGraph("");
    }
    int temp;
    // 弾画像読み込み
    if (stype == 0)
    {
        temp == LoadGraph("../graphics/E_shot.png");
    }
}
