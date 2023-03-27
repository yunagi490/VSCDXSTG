#include "[1] player.h"
#include "[2] enemy.h"
#include "[5] back.h"

class CONTROL
{
private:
    // プレイヤークラス
    PLAYER *player;

    // 背景クラス
    BACK *back;

    // 敵クラス
    ENEMY *enemy[ENEMY_NUM];

private:
    CONTROL();
    ~CONTROL();
public:
    void All();
    void GetPlayerPosition(double *x,double *y);
    void GetEnemyPosition(int index,double *x,double *y);
    static CONTROL& GetInstance(){
        static CONTROL control;
        return control;
    }
};