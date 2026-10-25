#ifndef _PLAYER_H
#define _PLAYER_H

#include "[4] define.h"
#include "[1] graph_control.h"

class PLAYER
{
private:
    // x座標, y座標
    double x, y;

    // 描画サイズ(px)
    double size;

    // 画像
    VS_GRAPH img, shotImg;

    // 残機 / 無敵残りフレーム
    int life;
    int invincible;
    bool alive;
    bool slow; // 低速移動中か

    // 弾
    SHOT shot[PSHOT_NUM];

    // カウント
    int count;

private:
    void Move();
    void Shot();

public:
    PLAYER();
    void Reset();
    void Update();
    void Draw() const;
    void GetPosition(double *x, double *y) const;

    // 被弾処理 (無敵中は何もしない)。被弾したらtrue
    bool Hit();
    int Life() const { return life; }
    int Invincible() const { return invincible; }
    bool Alive() const { return alive; }

    // 円(x,y,r)に当たった自機弾を消費し、命中数を返す
    int ConsumeShots(double x, double y, double r);
};
#endif