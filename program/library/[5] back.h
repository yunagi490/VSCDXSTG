// class BACK{
// private:
//     // 座標
//     double x,y;
//     // グラフィックハンドル
//     int gh;

// private:
//     void Draw();
//     void Move();
// public:
//     void All();
//     BACK();
// }
#ifndef _BACK_H
#define _BACK_H

#include "[1] graph_control.h"

class BACK
{
private:
    // スクロール量
    int offset;
    // 画像
    VS_GRAPH img;

private:
    void Draw() const;
    void Move();

public:
    void All();
    BACK();
};
#endif