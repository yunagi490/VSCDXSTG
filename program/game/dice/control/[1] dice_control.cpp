#include <string.h>
#include "[1] dice_control.h"

DICE_CTL::DICE_CTL() : d100(1, 100), d6(), d9(1, 9)
{
    Reset();
}

void DICE_CTL::Reset()
{
    memset(hist6, 0, sizeof(hist6));
    memset(hist9, 0, sizeof(hist9));
    last6 = last9 = 0;
    fumblePending = false;
    exNum = 0;
    MakeDiff(50, false);
}

void DICE_CTL::Push(int *h, int v)
{
    h[0] = h[1];
    h[1] = h[2];
    h[2] = v;
}

// 直近から数えて、v が何回連続しているか (最大3)
int DICE_CTL::Streak(const int *h, int v)
{
    int n = 0;
    for (int i = 2; i >= 0 && h[i] == v; --i)
        ++n;
    return n;
}

// 1d100 の出目 -> 難易度ランク (境目はここで調整する)
static int TierFromRoll(int roll)
{
    if (roll >= 100)
        return TIER_APOTHEOSIS;
    if (roll >= 96)
        return TIER_RAGNAROK;
    if (roll >= 90)
        return TIER_LUNATIC;
    if (roll >= 80)
        return TIER_HARD;
    if (roll >= 60)
        return TIER_NORMAL;
    if (roll >= 30)
        return TIER_EASY;
    return TIER_EGG;
}

void DICE_CTL::MakeDiff(int roll, bool fumble)
{
    diff.roll = roll;
    diff.fumble = fumble;
    diff.tier = fumble ? TIER_APOTHEOSIS : TierFromRoll(roll);

    // 弾速・HP は 0.8倍〜1.4倍。Ragnarok は +0.1、Apotheosis は +0.2 の上乗せ
    double bonus = 0.0;
    if (diff.tier > TIER_LUNATIC)
        bonus = 0.1 * (diff.tier - TIER_LUNATIC);
    diff.speedMul = 0.8 + 0.006 * roll + bonus;
    diff.hpMul = 0.8 + 0.006 * roll + bonus;

    // 弾幕の追加段階 : EGG と EASY は 0、そこから1ランクごとに +1 (最大5)
    diff.extra = diff.tier > TIER_EASY ? diff.tier - TIER_EASY : 0;
}


const DIFFICULTY &DICE_CTL::RollStage()
{
    const int r = d100.Roll();
    if (fumblePending)
    {
        // ファンブルの罰 : 出目に関係なく最高難易度
        MakeDiff(100, true);
        fumblePending = false;
    }
    else
    {
        MakeDiff(r, false);
    }
    return diff;
}

void DICE_CTL::RollBossClear()
{
    last6 = d6.Roll();
    last9 = d9.Roll();
    Push(hist6, last6);
    Push(hist9, last9);

    // 6が出たら強制ファンブル
    if (d6.IsFumble())
        fumblePending = true;

    // 666 -> コーミャ
    if (Streak(hist6, 6) == 3)
    {
        ForceExBoss(EXB_KOMY);
        memset(hist6, 0, sizeof(hist6));
    }
    // 999 -> スズ
    if (Streak(hist9, 9) == 3)
    {
        ForceExBoss(EXB_SUZU);
        memset(hist9, 0, sizeof(hist9));
    }
}

void DICE_CTL::ForceExBoss(EXBOSS_KIND k)
{
    if (exNum < 2)
        exQueue[exNum++] = k;
}

EXBOSS_KIND DICE_CTL::PopExBoss()
{
    if (exNum == 0)
        return EXB_NONE;
    const EXBOSS_KIND k = exQueue[0];
    exQueue[0] = exQueue[1];
    --exNum;
    return k;
}