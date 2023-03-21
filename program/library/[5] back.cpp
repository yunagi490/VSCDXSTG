#include "[5] back.h"
#include "DxLib.h"
#include "[4] define.h"

BACK::BACK(){
    gh=LoadGraph("../graphics/back1.png");
    x=y=MARGIN;
}

void BACK::Draw(){
    // 1枚目描画
    DrawGraph(x,y,gh,FALSE);
    // 2枚目描画
    DrawGraph(x,y-460,gh,FALSE);
    // 一番下までまでスクロールしたら初期値に戻す
    if(y==460+MARGIN)
        y=10;


}

void BACK::Move()
{
    y+=SCROLL_SPEED;
}

void BACK::All(){
    Draw();
    Move();
}
