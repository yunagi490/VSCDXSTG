#ifndef _ENEMY
#define _ENEMY

class ENEMY
{
private:
    // 座標とグラフィックハンドル
    double x, y;
    int gh[3];

    // 画像サイズ
    int width, height;
    // 出現、停止、帰還、発射タイミング
    int in_time, stop_time, out_time;

    // 敵の種類
    int type;

    // 弾の種類
    int stype;

    // 移動パターン
    int m_pattern;

    // ショットパターン
    int s_pattern;

    // HP
    int hp;
    // アイテム
    int item;

    // 敵が出現してからのカウント
    int count;
    // 発射した弾数
    int num;

    // 発射直後のラジアン
    double rad;

    // 敵消滅フラグ
    bool deadflag;
    bool endflag;

    // 弾構造体
    E_SHOT shot[ENEMY_SNUM];
    // ショットが撃てるようになったかのフラグ
    bool sflag;
    // ショットが撃てるようになってからのカウント
    int scount;

private:
    bool All();
    void Move();
    void Draw();
    bool OutCheck();
    bool ShotOutCheck(int i);

public:
    ENEMY();
    ENEMY(char *c_filename, int type, int m_pattern, int x, int y,
          int in_time, int stop_time, int shot_time, int out_time,
          char *s_filename, int stype, int s_pattern, int speed);
};
#endif