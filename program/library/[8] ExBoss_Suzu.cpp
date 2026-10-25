#include "DxLib.h"
#include "[8] ExBoss_Suzu.h"
#include "[3] shot.h"

// 600フレームで1サイクル
//   0〜419   : 遠くから狙い撃ち
//   420〜449 : 消えていく
//   450〜539 : 透明 (無敵・攻撃なし)
//   540      : プレイヤーの近くに出現して奇襲
//   540〜569 : 現れる
//   570〜599 : 遠くへ戻る
ExBoss_SUZU::ExBoss_SUZU(const DIFFICULTY &d)
    : BOSS(d, "graphics/suzu.png", 200.0, 28.0, 280, "SUZU"), alpha(255)
{
}

void ExBoss_SUZU::Move(const GAMECTX &ctx)
{
    const double cx = FIELD_L + FIELD_W / 2.0;

    // 登場
    if (count < 60)
    {
        MoveToward(cx, FIELD_T + 70.0, 0.05);
        return;
    }

    const int cyc = count % 600;
    if (cyc < 420 || cyc >= 570)
    {
        alpha = 255;
        MoveToward(cx + sin(count * 0.02) * 120.0, FIELD_T + 60.0, 0.04);
    }
    else if (cyc < 450)
    {
        alpha = 255 - (cyc - 420) * 255 / 30;
    }
    else if (cyc < 540)
    {
        alpha = 0;
    }
    else
    {
        if (cyc == 540)
        {
            // プレイヤーの横に瞬間移動
            x = VS_Clamp(ctx.px + (VS_Rand(2) ? 70.0 : -70.0), FIELD_L + 30.0, FIELD_R - 30.0);
            y = VS_Clamp(ctx.py - 150.0, FIELD_T + 100.0, FIELD_B - 200.0);
        }
        alpha = (cyc - 540) * 255 / 30;
    }
}

void ExBoss_SUZU::Shoot(const GAMECTX &ctx)
{
    if (count < 60)
        return;

    BULLETS *b = ctx.bullets;
    const double s = diff.speedMul;
    const double aim = atan2(ctx.py - y, ctx.px - x);
    const int cyc = count % 600;

    if (cyc < 420 || cyc >= 570)
    {
        // 遠くから狙い続ける
        if (count % 14 == 0)
            b->Fire(x, y, aim, 5.5 * s);
        if (count % 70 == 0)
            b->FireFan(x, y, aim, 3 + 2 * diff.extra, DEG2RAD(12), 4.0 * s);
    }
    else if (cyc == 540)
    {
        // 出現直後の奇襲
        b->FireFan(x, y, aim, 7 + 2 * diff.extra, DEG2RAD(8), 6.0 * s);
    }
}

void ExBoss_SUZU::Draw() const
{
    if (alpha <= 0)
        return;
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
    GRP_CTL::DrawCenter(img, x, y, size);
    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}