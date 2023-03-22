#include "[2] enemy.h"
#include "[4] define.h"
#include "DxLib.h"

ENEMY::ENEMY(char *c_filename,int type,int m_pattern,int x,int y,
            int in_time,int stop_time, int shot_time,int out_time,
                char *s_filename,int stype,int s_pattern,int speed)
{
    LoadGraph("../graphics/n_enemy1.png");
    width=64;
    height=64;

    // 敵の種類
    this->type=type;
    // 弾の種類
    this->stype=stype;

    // 移動パターンとショットパターン
    this->m_pattern=m_pattern;
    this->s_pattern=s_pattern;

    // 座標セット
    this->x=x;
    this->y=y;

    // 出現、停止、発射、帰還時間セット
    this->in_time=in_time;
    this->stop_time=stop_time;
    this->shot_time=shot_time;
    this->out_time=out_time;

    // 弾画像とサイズ取得
    int temp=LoadGraph(s_filename);
    int w,h;
    GetGraphSize(temp,&w,&h);

    // 弾の初期化
    for(int i=0;i<ENEMY_SNUM;++i){
        shot[i].flag=false;
        shot[i].gh=temp;
        shot[i].width=w;
        shot[i].height=h;
        shot[i].pattern=s_pattern;
        shot[i].speed=speed;
        shot[i].x=x;
        shot[i].y=y;
    }

    count=0;
    scount=0;

    x=50;
    y=50;

    in_time=180;
    stop_time=300;
    out_time=420;

    count=0;

    deadflag=false;
    endflag=false;
    sflag=false;
}
void ENEMY::Shot()
{
    
}
void ENEMY::Move()
{
    // 出てきてから止まる時間までの間なら下に移動
    if(in_time<g_count&&g_count<stop_time){
        y+=2;
    // 帰還時間を過ぎたら戻る
    }else if(g_count>out_time){
        y-=2;
        if(y<-40){
            endflag=true;
        }
    }
}

void ENEMY::Draw()
{
    int temp;
    if(!endflag){
        temp=count%40/10;
        if(temp==3)
        temp=1;
    
    DrawGraph(x,y,gh[temp],TRUE);
    }
}

bool ENEMY::All()
{
    Move();
    Draw();
    ++count;

    return endflag;
}