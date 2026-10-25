#ifndef _ExBoss_SUZU
#define _ExBoss_SUZU

#include "[6] BossEnemy.h"

// EXボス スズ (ExBEnemy_M)
// 攻撃: 無限に遠距離から狙ってくる。たまに消えて、油断したところを撃ってくる。
// 出現条件: ボス撃破時のダイス 1d9 で 9 が3回連続 (999)
class ExBoss_SUZU : public BOSS
{
private:
    int alpha; // 0〜255 (消えている間は0)

public:
    ExBoss_SUZU(const DIFFICULTY &d);
    void Draw() const override;
    // 消えている間は攻撃が当たらない
    bool Vulnerable() const override { return alpha >= 128; }

protected:
    void Move(const GAMECTX &ctx) override;
    void Shoot(const GAMECTX &ctx) override;
};
#endif