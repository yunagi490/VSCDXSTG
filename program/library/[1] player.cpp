#include "DxLib.h"

#include "[1] player.h"
#include "[4] define.h"

#include <stdio.h>


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

    // 生きてるかどうか
    life=true;

    //  弾初期化
    memset(shot,0,sizeof(shot));


    // 弾画像読み込み
    int temp=LoadGraph("../graphics/shot.png");
    int w,h;
    GetGraphSize(temp,&w,&h);

    // フラグを全部falseにしとく
    // グラフィックハンドルと画像のサイズを代入しとく
    for (int i=0;i<PSHOT_NUM;++i){
        shot[i].flag=false;
        shot[i].gh=temp;
        shot[i].width=w;
        shot[i].height=h;
    }
    count=0;


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

<span class="stress">
        // キャラの移動制御
        if(x>400-MARGIN){
            y=480-height/2-MARGIN;
        }else if(x<MARGIN){
            x=MARGIN;
        }

        if(y>480-height/2-MARGIN){
            y=480-height/2-MARGIN;
        }else if(y<height/2+MARGIN){
            y=height/2+MARGIN;
        }
</span>

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

    // カウント数から添字を求める
    ix = abs(xcount) % 30 / 10;
    iy = abs(ycount) % 30 / 10;

    // xカウントがプラスなら右向きなので、4行目の先頭添字番号を足す
    if (xcount > 0)
    {
        ix += 3;
        result = ix;
    }
    else if (xcount < 0)
    {
        // マイナスなら左向きなので、4行目の先頭添字版後を足す
        ix = 9;
        result = ix;
    }

    // yカウントがプラスなら下向きなので3行目の先頭添字番号を足す
    if (ycount > 0)
    {
        iy += 6;
        result = iy;
    }
    else if (ycount < 0)
    {
        iy += 0;
        result = iy;
    }
    // 斜め移動の場合は横顔を優先
    if (move == 0.71f)
        result = ix;

    // 描画
    DrawGraph(x - width / 2, y - height / 2, gh[result], TRUE);

    // 押されていなければカウントをゼロにする
    if (key[KEY_INPUT_LEFT] != 1 && key[KEY_INPUT_RIGHT] != 1)
    {
        xcount = 0;
    }
    if (key[KEY_INPUT_UP] != 1 && key[KEY_INPUT_DOWN] != 1)
    {
        ycount = 0;
    }
}
