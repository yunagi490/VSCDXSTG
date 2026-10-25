#ifndef _ExBoss_KOMY
#define _ExBoss_KOMY

#include "[6] BossEnemy.h"

// EXボス コーミャ (ExBEnemy_Y)
// 攻撃: 近づいて撃つ -> 離れて撃つ の繰り返し。たまにリスキルもする。
// 出現条件: ボス撃破時のダイス 1d6 で 6 が3回連続 (666)
class ExBoss_KOMY : public BOSS
{
private:
    int prevInv; // 前フレームのプレイヤー無敵残り (復活の瞬間の検出用)

public:
    ExBoss_KOMY(const DIFFICULTY &d);

protected:
    void Move(const GAMECTX &ctx) override;
    void Shoot(const GAMECTX &ctx) override;
};
#endif