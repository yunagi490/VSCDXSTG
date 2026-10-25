#include "DxLib.h"
#include "[7] ExBoss_KOMY.h"
#include "[3] shot.h"

ExBoss_KOMY::ExBoss_KOMY(const DIFFICULTY &d)
    : BOSS(d, "graphics/komy.png", 200.0, 28.0, 300, "KOMY"), prevInv(0)
{
}

void ExBoss_KOMY::Move(const GAMECTX &ctx)
{
    const double cx = FIELD_L + FIELD_W / 2.0;

    // 登場
    if (count < 60)
    {
        MoveToward(cx, FIELD_T + 90.0, 0.05);
        return;
    }

    if (count % 480 < 240)
    {
        // 近づく : プレイヤーの少し上まで寄ってくる
        const double ty = VS_Clamp(ctx.py - 130.0, FIELD_T + 90.0, FIELD_B - 220.0);
        MoveToward(ctx.px, ty, 0.03);
    }
    else
    {
        // 離れる : 上に戻って左右に揺れる
        MoveToward(cx + sin(count * 0.03) * 110.0, FIELD_T + 70.0, 0.04);
    }
    x = VS_Clamp(x, FIELD_L + 30.0, FIELD_R - 30.0);
}

void ExBoss_KOMY::Shoot(const GAMECTX &ctx)
{
    // リスキル : プレイヤーの無敵が切れた瞬間に、たまに(1/3)濃い狙い撃ちをする
    const bool respawnKill = (prevInv > 0 && ctx.pinv == 0 && count >= 60 && VS_Rand(3) == 0);
    prevInv = ctx.pinv;

    if (count < 60)
        return;

    BULLETS *b = ctx.bullets;
    const double s = diff.speedMul;
    const double aim = atan2(ctx.py - y, ctx.px - x);

    if (respawnKill)
    {
        for (int i = 0; i < 3; ++i)
            b->FireFan(x, y, aim, 5 + 2 * diff.extra, DEG2RAD(7), (4.0 + i * 0.8) * s);
        return;
    }

    if (count % 480 < 240)
    {
        // 近づいている間 : 狙い撃ちの扇
        if (count % 18 == 0)
            b->FireFan(x, y, aim, 3 + 2 * diff.extra, DEG2RAD(9), 4.5 * s);
    }
    else
    {
        // 離れている間 : 回転する全方位弾 + 牽制の狙い撃ち
        if (count % 30 == 0)
            b->FireRing(x, y, 14 + 4 * diff.extra, count * 0.07, 2.2 * s);
        if (count % 10 == 0)
            b->Fire(x, y, aim, 5.0 * s);
    }
}