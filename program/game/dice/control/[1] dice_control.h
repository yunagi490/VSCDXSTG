#ifndef _DICE_CONTROL
#define _DICE_CONTROL

#include "[4] define.h"
#include "[1] dice_1d6.h"

/* ### ダイスコントロール ###
   ・ステージ開始時 : 1d100 -> 数字が大きいほど高難易度
   ・ボス撃破時     : 1d6 と 1d9 を振る
        - 1d6 が 6 ならファンブル -> 次のステージは強制で最高難易度
        - 1d6 で 6 が3回連続(666)  -> コーミャ(KOMY)が出現
        - 1d9 で 9 が3回連続(999)  -> スズ(SUZU)が出現
     出現条件を変えたいときは RollBossClear() の中だけ直せばよい */
class DICE_CTL
{
private:
    DICE_STD d100;
    DICE_1D6 d6;
    DICE_STD d9;

    DIFFICULTY diff;
    bool fumblePending; // ファンブル済みで、次のステージが最高難易度

    int hist6[3]; // 直近3回の 1d6 (新しいほど後ろ)
    int hist9[3];
    int last6, last9; // 直近のボス撃破ダイス (未実施は0)

    EXBOSS_KIND exQueue[2];
    int exNum;

private:
    static void Push(int *h, int v);
    static int Streak(const int *h, int v);
    void MakeDiff(int roll, bool fumble);

public:
    DICE_CTL();
    void Reset();

    // ステージ開始時の 1d100
    const DIFFICULTY &RollStage();
    // ボス撃破時の 1d6 / 1d9
    void RollBossClear();
    // 出現待ちのEXボスを1体取り出す (無ければ EXB_NONE)
    EXBOSS_KIND PopExBoss();
    // デバッグ用 : EXボスを強制的に予約する
    void ForceExBoss(EXBOSS_KIND k);

    const DIFFICULTY &Diff() const { return diff; }
    int Last6() const { return last6; }
    int Last9() const { return last9; }
    bool IsFumble6() const { return last6 == 6; }
    int Streak6() const { return Streak(hist6, 6); }
    int Streak9() const { return Streak(hist9, 9); }
};
#endif