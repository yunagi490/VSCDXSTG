#include "DxLib.h"

#include "[1] player.h"
#include "[4] define.h"

#include <string.h>

PLAYER::PLAYER() : size(48.0)
{
    // 画像読み込み (同じ画像は GRP_CTL が使い回す)
    img = GRP_CTL::Get("graphics/player1.png");
    shotImg = GRP_CTL::Get("graphics/shot.png");
    Reset();
}

void PLAYER::Reset()
{
    // 初期位置
    x = FIELD_L + FIELD_W / 2.0;
    y = 400;

    life = PLAYER_LIFE;
    invincible = 0;
    alive = true;
    slow = false;
    count = 0;

    // 弾初期化
    memset(shot, 0, sizeof(shot));
}

void PLAYER::Shot()
{
    // Zキーが押されててかつ、6ループに一回、左右2発ずつ発射
    if (key[KEY_INPUT_Z] == 1 && count % 6 == 0)
    {
        int made = 0;
        for (int i = 0; i < PSHOT_NUM && made < 2; ++i)
        {
            if (!shot[i].flag)
            {
                shot[i].flag = true;
                shot[i].x = x + (made == 0 ? -8 : 8);
                shot[i].y = y - 10;
                ++made;
            }
        }
    }

    // 弾を移動させる処理
    for (int i = 0; i < PSHOT_NUM; ++i)
    {
        if (!shot[i].flag)
            continue;
        shot[i].y -= PSHOT_SPEED;

        // 画面の外にはみ出したらフラグを戻す
        if (shot[i].y < FIELD_T - 10)
            shot[i].flag = false;
    }
}

void PLAYER::Move()
{
    slow = (key[KEY_INPUT_LSHIFT] == 1 || key[KEY_INPUT_RSHIFT] == 1);
    const double speed = slow ? PLAYER_SLOW_SPEED : PLAYER_SPEED;

    const int dx = (key[KEY_INPUT_RIGHT] == 1 ? 1 : 0) - (key[KEY_INPUT_LEFT] == 1 ? 1 : 0);
    const int dy = (key[KEY_INPUT_DOWN] == 1 ? 1 : 0) - (key[KEY_INPUT_UP] == 1 ? 1 : 0);

    // 斜め移動は速くならないように補正
    const double move = (dx != 0 && dy != 0) ? 0.7071 : 1.0;
    x += dx * speed * move;
    y += dy * speed * move;

    // キャラの移動制御
    x = VS_Clamp(x, FIELD_L + 12, FIELD_R - 12);
    y = VS_Clamp(y, FIELD_T + 16, FIELD_B - 16);
}

void PLAYER::Update()
{
    if (!alive)
        return;
    if (invincible > 0)
        --invincible;
    Move();
    Shot();
    ++count;
}

void PLAYER::Draw() const
{
    // 弾描画
    for (int i = 0; i < PSHOT_NUM; ++i)
    {
        if (shot[i].flag)
            GRP_CTL::DrawCenter(shotImg, shot[i].x, shot[i].y, 24.0);
    }

    // 生きていれば描画 (無敵中は点滅)
    if (alive && !(invincible > 0 && (invincible / 3) % 2 == 0))
    {
        GRP_CTL::DrawCenter(img, x, y, size);
        if (slow)
            DrawCircle((int)x, (int)y, 3, GetColor(255, 255, 255), TRUE); // 当たり判定表示
    }
}

void PLAYER::GetPosition(double *px, double *py) const
{
    *px = x;
    *py = y;
}

bool PLAYER::Hit()
{
    if (!alive || invincible > 0)
        return false;
    --life;
    if (life <= 0)
    {
        alive = false;
        return true;
    }
    invincible = PLAYER_INVINCIBLE;
    x = FIELD_L + FIELD_W / 2.0;
    y = 400;
    return true;
}

int PLAYER::ConsumeShots(double ex, double ey, double r)
{
    const double rr = r + 8.0;
    int hits = 0;
    for (int i = 0; i < PSHOT_NUM; ++i)
    {
        if (!shot[i].flag)
            continue;
        const double dx = shot[i].x - ex;
        const double dy = shot[i].y - ey;
        if (dx * dx + dy * dy <= rr * rr)
        {
            shot[i].flag = false;
            ++hits;
        }
    }
    return hits;
}