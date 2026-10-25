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

void DICE_CTL::MakeDiff(int roll, bool fumble)
{
    diff.roll = roll;
    diff.fumble = fumble;
    // 1d100 の出目 1〜100 を、弾速・HP は 0.8倍〜1.4倍、弾幕の追加段階は 0〜4 に変換
    diff.speedMul = 0.8 + 0.006 * roll;
    diff.hpMul = 0.8 + 0.006 * roll;
    diff.extra = roll / 25;
    if (diff.extra > 4)
        diff.extra = 4;
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