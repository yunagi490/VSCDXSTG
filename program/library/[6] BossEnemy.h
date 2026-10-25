#ifndef _BOSS_ENEMY
#define _BOSS_ENEMY

#include "[4] define.h"
#include "[1] graph_control.h"

// ボス共通の土台クラス。移動(Move)と攻撃(Shoot)だけ派生クラスで作る。
// 通常ボス(BOSS_NORMAL)も、EXボス(KOMY / SUZU)もこれを継承する。
class BOSS
{
protected:
    // 座標と画像
    double x, y;
    VS_GRAPH img;
    double size;   // 描画サイズ(px)
    double radius; // 当たり判定半径

    int hp, maxhp;
    // ボスが出現してからのカウント
    int count;
    bool dead;

    DIFFICULTY diff;
    const char *name;

protected:
    // 現在地から目標へ rate(0〜1) の割合だけ近づく
    void MoveToward(double tx, double ty, double rate);
    virtual void Move(const GAMECTX &ctx) = 0;
    virtual void Shoot(const GAMECTX &ctx) = 0;

public:
    BOSS(const DIFFICULTY &d, const char *imgName, double drawSize,
         double hitRadius, int baseHp, const char *bossName);
    virtual ~BOSS() {}

    void Update(const GAMECTX &ctx);
    virtual void Draw() const;
    void DrawHpBar() const;

    // ダメージを与える (無敵中は無効)
    void Damage(int d);
    void Kill() { hp = 0; dead = true; }
    virtual bool Vulnerable() const { return true; }

    bool IsDead() const { return dead; }
    const char *Name() const { return name; }
    void GetPosition(double *px, double *py) const
    {
        *px = x;
        *py = y;
    }
    double Radius() const { return radius; }
};

// 通常のステージボス
class BOSS_NORMAL : public BOSS
{
public:
    BOSS_NORMAL(const DIFFICULTY &d);

protected:
    void Move(const GAMECTX &ctx) override;
    void Shoot(const GAMECTX &ctx) override;
};

#endif