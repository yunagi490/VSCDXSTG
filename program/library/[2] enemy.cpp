#include <stdio.h>
#include "DxLib.h"
#include "[2] enemy.h"
#include "[3] shot.h"

ENEMY::ENEMY(const ENEMYDATA &d, const DIFFICULTY &diff)
{
    size = 56.0;

    // 敵の種類と弾の種類
    type = d.type;
    stype = d.stype;

    // 移動パターンとショットパターン
    m_pattern = d.m_pattern;
    s_pattern = d.s_pattern;

    // 座標セット
    x = d.x;
    y = d.y;

    // 停止、発射、帰還時間セット (出現時間を基準にした相対値にする)
    stop_time = d.stop_time - d.in_time;
    shot_time = d.shot_time - d.in_time;
    out_time = d.out_time - d.in_time;

    // hpとアイテム (HPは難易度で増減)
    hp = (int)(d.hp * diff.hpMul + 0.5);
    if (hp < 1)
        hp = 1;
    item = d.item;

    bulletSpeed = d.speed > 0 ? d.speed : 3;

    // 敵画像読み込み
    img = GRP_CTL::Get("graphics/n_enemy1.png");

    count = 0;
    scount = 0;
    rad = 0;
    sflag = false;
}

void ENEMY::Shoot(const GAMECTX &ctx)
{
    // 発射タイミングになったら、フラグを立てる
    if (!sflag && count >= shot_time)
    {
        sflag = true;
        scount = 0;
    }
    if (!sflag)
        return;

    const DIFFICULTY &df = *ctx.diff;
    const double spd = bulletSpeed * df.speedMul;
    BULLETS *b = ctx.bullets;

    // 敵とプレイヤーとの座標の差から逆正接を求める (初回だけ)
    if (scount == 0)
        rad = atan2(ctx.py - y, ctx.px - x);

    switch (s_pattern)
    {
    // 狙い撃ち : 10ループに一回。scount<=40なので5回
    case 0:
        if (scount % 10 == 0 && scount <= 40)
            b->FireFan(x, y, rad, 1 + 2 * df.extra, DEG2RAD(8), spd);
        break;
    // プレイヤーに向かって直線ショット : 6ループに一回、10回
    case 1:
        if (scount % 6 == 0 && scount <= 54)
            b->FireFan(x, y, rad, 1 + 2 * (df.extra / 2), DEG2RAD(6), spd);
        break;
    // 3直線ショット (±10度) : 10ループに一回、5回
    case 2:
        if (scount % 10 == 0 && scount <= 40)
            b->FireFan(x, y, rad, 3 + 2 * df.extra, DEG2RAD(10), spd);
        break;
    // 乱射ショット : プレイヤー方向±60度にランダム
    case 3:
    {
        int interval = 4 - df.extra;
        if (interval < 1)
            interval = 1;
        if (scount % interval == 0 && scount <= 180)
        {
            const double a = atan2(ctx.py - y, ctx.px - x) - DEG2RAD(60) + DEG2RAD(VS_Rand(121));
            b->Fire(x, y, a, spd);
        }
        break;
    }
    }
    ++scount;
}

void ENEMY::Move()
{
    switch (m_pattern)
    {
    // 途中で止まって、そのまま後ろに帰るパターン
    case 0:
        // 出てきてから止まる時間までの間なら下に移動
        if (count < stop_time)
            y += 2;
        // 帰還時間を過ぎたら戻る
        else if (count > out_time)
            y -= 2;
        break;
    // そのまま止まらずに下に行くパターン
    case 1:
        y += 2;
        break;
    // ちょっとずつ左に移動しながら消えていく
    case 2:
        y += 1;
        if (count % 10 == 0)
            x -= 1;
        break;
    // ちょっとずつ右に移動しながら消えていく
    case 3:
        y += 1;
        if (count % 10 == 0)
            x += 1;
        break;
    }
}

void ENEMY::Draw() const
{
    GRP_CTL::DrawCenter(img, x, y, size);
}

bool ENEMY::OutCheck() const
{
    return x < FIELD_L - 50 || x > FIELD_R + 50 || y < FIELD_T - 50 || y > FIELD_B + 50;
}

bool ENEMY::Update(const GAMECTX &ctx)
{
    Move();
    Shoot(ctx);
    ++count;

    // 画面からはみ出したら不要 (出現直後は画面外なので、停止時間を過ぎてから判定)
    return count > stop_time && OutCheck();
}

bool ENEMY::Damage(int d)
{
    hp -= d;
    return hp <= 0;
}

void ENEMY::GetPosition(double *px, double *py) const
{
    *px = x;
    *py = y;
}