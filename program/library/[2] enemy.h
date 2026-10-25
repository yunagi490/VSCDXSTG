#ifndef _ENEMY_H
#define _ENEMY_H

#include "[4] define.h"
#include "[1] graph_control.h"

class ENEMY
{
private:
    // 座標と画像
    double x, y;
    VS_GRAPH img;
    double size; // 描画サイズ

    // 出現してからの停止、発射、帰還タイミング (出現時を0とした相対フレーム)
    int stop_time, shot_time, out_time;

    // 敵の種類
    int type;
    // 弾の種類
    int stype;
    // 移動パターン
    int m_pattern;
    // ショットパターン
    int s_pattern;

    // 弾スピード
    double bulletSpeed;

    // HP
    int hp;
    // アイテム
    int item;

    // 敵が出現してからのカウント
    int count;

    // 発射直後のラジアン
    double rad;

    // ショットが撃てるようになったかのフラグ
    bool sflag;
    // ショットが撃てるようになってからのカウント
    int scount;

private:
    void Move();
    void Shoot(const GAMECTX &ctx);
    bool OutCheck() const;

public:
    ENEMY(const ENEMYDATA &d, const DIFFICULTY &diff);
    // 画面外に出て不要になったらtrue
    bool Update(const GAMECTX &ctx);
    void Draw() const;
    // ダメージを与える。倒したらtrue
    bool Damage(int d);
    void GetPosition(double *px, double *py) const;
    double Radius() const { return 20.0; }
};
#endif