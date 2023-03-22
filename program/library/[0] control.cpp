#include "[0] control.h"
#include "[4] define.h"
#include "DxLib.h"

CONTROL::CONTROL()
{
    // プレイヤークラスの生成
    player=new PLAYER;

    // 背景クラスの生成
    back=new BACK;
}

CONTROL::~CONTROL()
{
    // プレイヤークラスの解放
    delete player;
}

void CONTROL::All()
{
    // 描画距離を指定
    SetDrawArea(MARGIN,MARGIN,MARGIN+300,MARGIN+460);

    player->All();
    back->All();

    if(enemy!=NULL){
        if(enemy->All()){
            delete enemy;
            enemy=NULL;
        }
    }

    ++g_count;
}