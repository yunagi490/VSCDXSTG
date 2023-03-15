#include "DxLib.h"
#include "[1] player.h"
#include "[4] define.h"

PLAYER::PLAYER()
{
    // 画像読み込み
    /// DrawGraph("../graphics/player1.png");

    width = 64;
    height = 64;

    // 移動係数
    move = 1.0f;

    // 横方向と縦方向のカウント数
    xcount = 0, ycount = 0;
    // 添字用変数
    ix = 0, iy = 0, result = 0;

    // 初期位置
    x = 180;
    y = 400;
}
void PLAYER::Move()
{
    if (key[KEY_INPUT_LEFT] || key[KEY_INPUT_RIGHT] == 1)
    {
        if (key[KEY_INPUT_UP] == 1 || key[KEY_INPUT_DOWN] == 1)
        {
            // 移動係数を0.71に設定
            move = 0.71f;
        }
        else
        {
            // 斜めじゃなければ1.0に設定
            move = 1.0f;
        }
    }
    else if (key[KEY_INPUT_UP] == 1 || key[KEY_INPUT_DOWN] == 1)
    {
        move = 1.0f;
    }

    if (key[KEY_INPUT_LEFT] == 1)
    {
        x -= (int)PLAYER_SPEED * move;
    }
    if (key[KEY_INPUT_RIGHT] == 1)
    {
        x += (int)PLAYER_SPEED * move;
    }
    if (key[KEY_INPUT_UP] == 1)
    {
        y -= (int)PLAYER_SPEED * move;
    }
    if (key[KEY_INPUT_DOWN] == 1)
    {
        y += (int)PLAYER_SPEED * move;
    }

    if (key[KEY_INPUT_LEFT] == 1)
    {
        if (xcount > 0)
            xcount = 0;
        --xcount;
    }
    if (key[KEY_INPUT_RIGHT] == 1)
    {
        if (xcount < 0)
            xcount = 0;
        ++xcount;
    }
    if (key[KEY_INPUT_UP] == 1)
    {
        if (xcount > 0)
            xcount = 0;
        --ycount;
    }
    if (key[KEY_INPUT_DOWN] == 1)
    {
        if (xcount < 0)
            xcount = 0;
        ++ycount;
    }
}
