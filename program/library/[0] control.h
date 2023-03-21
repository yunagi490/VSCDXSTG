#include "[1] player.h"

class CONTROL
{
private:
    // プレイヤークラス
    PLAYER *player;

    // 背景クラス
    BACK *back;

public:
    CONTROL();
    ~CONTROL();
    void All();
};