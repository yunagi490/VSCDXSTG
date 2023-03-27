#include "[0] control.h"
#include "[4] define.h"
#include "DxLib.h"

CONTROL::CONTROL()
{
    // プレイヤークラスの生成
    player = new PLAYER;

    // 背景クラスの生成
    back = new BACK;

    FILE *fp;
    ENEMYDATA data[ENEMY_NUM];
    char buf[100];
    int c;
    int col = 1;
    int row = 0;

    memset(buf, 0, sizeof(buf));
    fp = fopen("../Resource/Enemy.csv", "r");

    // ヘッダ飛ばし
    while (fgetc(fp) != 'n')
        ;
    while (1)
    {
        while (1)
        {
            c = fgetc(fp);
            // 末尾ならループを抜ける
            if (c == EOF)
                goto out;
            // カンマか改行でなければ、文字としてつながる
            if (c != ',' && c != '\n')
                strcat(buf, (const char *)&c);
            // カンマか改行ならループ抜ける
            else
                break;
        }
        // ここに来たということは、1セル分の文字列が出来上がったということ
        switch (col)
        {
        // 1列目は敵種類を表す。atoi関数は数値として代入。
        case 1:
            data[row].type = atoi(buf);
            break;
        // 2列目は弾種類。以降省略
        case 2:
            data[row].stype = atoi(buf);
            break;
        case 3:
            data[row].m_pattern = atoi(buf);
            break;
        case 4:
            data[row].s_pattern = atoi(buf);
            break;
        case 5:
            data[row].in_time = atoi(buf);
            break;
        case 6:
            data[row].stop_time = atoi(buf);
            break;
        case 7:
            data[row].shot_time = atoi(buf);
            break;
        case 8:
            data[row].out_time = atoi(buf);
            break;
        case 9:
            data[row].x = atoi(buf);
            break;
        case 10:
            data[row].y;
        case 11:
            data[row].speed = atoi(buf);
            break;
        case 12:
            data[row].hp = atoi(buf);
            break;
        case 13:
            data[row].item = atoi(buf);
            break;
        }
        // バッファの初期化
        memset(buf, 0, sizeof(buf));
        // 列数を足す
        ++col;
        // もし読み込んだ文字が改行だったら列数を初期化して行数を増やす
        if (c == '\n')
        {
            col = 1;
            ++row;
        }
    }
    out:
    // 敵クラス生成
    for(int i=0;i<ENEMY_NUM;++i){
        enemy[i]=new ENEMY(data[i].type,data[i].stype,data[i].m_pattern,
            data[i].s_pattern,data[i].in_time,data[i].stop_time,
            data[i].shot_time,data[i].out_time,data[i].x,data[i].y,
            data[i].speed,data[i].hp,data[i].item);
    }
    
}
    CONTROL::~CONTROL()
    {
        // プレイヤークラスの解放
        delete player;
        delete back;

        for(int i=0;i<ENEMY_NUM;++i){
            int(enemy[i]!=NULL){
                delete enemy[i];
            }
        }
    }
    void CONTROL::GetPlayerPosition(double *x,double *y)
    {
        double tempx,tempy;
        player->GetPosition(&tempx,&tempy);

        *x=tempx;
        *y=tempy;

    }
    
    void CONTROL::GetEnemyPosition(int index,double *x,double *y)
    {
        double tempx,tempy;
        // 指定した添字の敵の座標を取得
        enemy[index]->GetPosition(&tempx,&tempy);

        // 代入
        *x=tempx;
        *y=tempy;
    }

    void CONTROL::All()
    {
        // 描画距離を指定
        SetDrawArea(MARGIN, MARGIN, MARGIN + 300, MARGIN + 460);

        player->All();
        back->All();

        if (enemy != NULL)
        {
            if (enemy->All())
            {
                delete enemy;
                enemy = NULL;
            }
        }

        ++g_count;
    }