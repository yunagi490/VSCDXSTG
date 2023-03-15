#include "[0] control.h"

CONTROL::CONTROL()
{
    // プレイヤークラスの生成
    player=new PLAYER;
}

CONTROL::~CONTROL()
{
    // プレイヤークラスの解放
    delete player;
}

void CONTROL::All()
{
    // プレイヤークラスのAll関数実行
    player->All();
}