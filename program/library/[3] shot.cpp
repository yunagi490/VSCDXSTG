#include "DxLib.h"
#include "[3] shot.h"

BULLETS::BULLETS() : num(0)
{
    // E_shot.png は 900x900 で円は中央の約1/3。円が約14pxになるよう40pxで描く
    img = GRP_CTL::Get("graphics/E_shot.png");
}

void BULLETS::Fire(double x, double y, double rad, double speed)
{
    if (num >= EBULLET_MAX)
        return;
    E_SHOT &s = pool[num++];
    s.x = x;
    s.y = y;
    s.vx = cos(rad) * speed;
    s.vy = sin(rad) * speed;
}

void BULLETS::FireFan(double x, double y, double centerRad, int ways, double stepRad, double speed)
{
    const double start = centerRad - stepRad * (ways - 1) * 0.5;
    for (int i = 0; i < ways; ++i)
        Fire(x, y, start + stepRad * i, speed);
}

void BULLETS::FireRing(double x, double y, int n, double offsetRad, double speed)
{
    const double step = 2.0 * VS_PI / n;
    for (int i = 0; i < n; ++i)
        Fire(x, y, offsetRad + step * i, speed);
}

void BULLETS::Update()
{
    const double l = FIELD_L - 20, r = FIELD_R + 20;
    const double t = FIELD_T - 20, b = FIELD_B + 20;
    int i = 0;
    while (i < num)
    {
        E_SHOT &s = pool[i];
        s.x += s.vx;
        s.y += s.vy;
        if (s.x < l || s.x > r || s.y < t || s.y > b)
            pool[i] = pool[--num]; // 末尾と入れ替え (iは進めない)
        else
            ++i;
    }
}

void BULLETS::Draw() const
{
    if (img.gh < 0)
    {
        for (int i = 0; i < num; ++i)
            DrawCircle((int)pool[i].x, (int)pool[i].y, 5, GetColor(255, 60, 60), TRUE);
        return;
    }
    const double scale = 40.0 / img.w;
    for (int i = 0; i < num; ++i)
        DrawRotaGraphF((float)pool[i].x, (float)pool[i].y, scale, 0.0, img.gh, TRUE);
}

bool BULLETS::Hit(double x, double y, double r) const
{
    const double rr = r + EBULLET_R;
    const double rr2 = rr * rr;
    for (int i = 0; i < num; ++i)
    {
        const double dx = pool[i].x - x;
        if (dx > rr || dx < -rr)
            continue; // 距離の2乗を求める前に軽く弾く
        const double dy = pool[i].y - y;
        if (dx * dx + dy * dy <= rr2)
            return true;
    }
    return false;
}