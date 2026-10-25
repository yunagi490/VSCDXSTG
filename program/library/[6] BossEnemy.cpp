#include "DxLib.h"
#include "[6] BossEnemy.h"
#include "[3] shot.h"

BOSS::BOSS(const DIFFICULTY &d, const char *imgName, double drawSize,
           double hitRadius, int baseHp, const char *bossName)
    : x(FIELD_L + FIELD_W / 2.0), y(FIELD_T - 60.0),
      img(GRP_CTL::Get(imgName)), size(drawSize), radius(hitRadius),
      hp(0), maxhp(0), count(0), dead(false), diff(d), name(bossName)
{
    hp = (int)(baseHp * d.hpMul + 0.5);
    if (hp < 1)
        hp = 1;
    maxhp = hp;
}

void BOSS::MoveToward(double tx, double ty, double rate)
{
    x += (tx - x) * rate;
    y += (ty - y) * rate;
}

void BOSS::Update(const GAMECTX &ctx)
{
    Move(ctx);
    Shoot(ctx);
    ++count;
}

void BOSS::Draw() const
{
    GRP_CTL::DrawCenter(img, x, y, size);
}

void BOSS::DrawHpBar() const
{
    const int l = FIELD_L + 4, r = FIELD_R - 4, t = FIELD_T + 4;
    const int w = (int)((r - l) * (double)hp / maxhp);
    DrawBox(l, t, r, t + 6, GetColor(60, 0, 0), TRUE);
    DrawBox(l, t, l + w, t + 6, GetColor(255, 60, 60), TRUE);
    DrawBox(l, t, r, t + 6, GetColor(255, 255, 255), FALSE);
    DrawFormatString(l, t + 10, GetColor(255, 255, 255), "%s", name);
}

void BOSS::Damage(int d)
{
    if (dead || !Vulnerable())
        return;
    hp -= d;
    if (hp <= 0)
    {
        hp = 0;
        dead = true;
    }
}

// ===================== 通常ボス =====================

BOSS_NORMAL::BOSS_NORMAL(const DIFFICULTY &d)
    : BOSS(d, "graphics/n_enemy1.png", 110.0, 30.0, 150, "BOSS")
{
}

void BOSS_NORMAL::Move(const GAMECTX &)
{
    // 出てきたら左右にゆっくり揺れる
    const double cx = FIELD_L + FIELD_W / 2.0;
    MoveToward(cx + sin(count * 0.02) * 100.0, FIELD_T + 100.0, 0.03);
}

void BOSS_NORMAL::Shoot(const GAMECTX &ctx)
{
    if (count < 60)
        return;
    BULLETS *b = ctx.bullets;
    const double s = diff.speedMul;
    const double aim = atan2(ctx.py - y, ctx.px - x);

    // 狙い撃ち
    if (count % 45 == 0)
        b->FireFan(x, y, aim, 3 + 2 * diff.extra, DEG2RAD(12), 3.5 * s);
    // 回転弾
    if (count % 6 == 0)
        b->Fire(x, y, count * 0.2, 2.5 * s);
    // 全方位
    if (count % 120 == 0)
        b->FireRing(x, y, 16 + 4 * diff.extra, 0.0, 2.0 * s);
}