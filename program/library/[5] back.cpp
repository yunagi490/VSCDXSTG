#include "[5] back.h"
#include "DxLib.h"
#include "[4] define.h"

BACK::BACK() : offset(0)
{
    img = GRP_CTL::Get("graphics/back1.png");
}

void BACK::Draw() const
{
    if (img.gh < 0)
        return;
    // 1枚目描画
    DrawGraph(FIELD_L, FIELD_T + offset, img.gh, FALSE);
    // 2枚目描画 (1枚目の上に繋げる)
    DrawGraph(FIELD_L, FIELD_T + offset - FIELD_H, img.gh, FALSE);
}

void BACK::Move()
{
    // 一番下までスクロールしたら初期値に戻す
    offset = (offset + SCROLL_SPEED) % FIELD_H;
}

void BACK::All()
{
    Draw();
    Move();
}