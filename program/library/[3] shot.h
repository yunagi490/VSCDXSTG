#ifndef _SHOT_H
#define _SHOT_H

#include "[4] define.h"
#include "[1] graph_control.h"

// 敵弾プール : 生きている弾だけを配列の先頭に詰めて持つ
// (死んだ弾は末尾の弾と入れ替えるので、空きスロットの走査が不要)
class BULLETS
{
private:
    E_SHOT pool[EBULLET_MAX];
    int num;
    VS_GRAPH img;

public:
    BULLETS();
    void Clear() { num = 0; }
    int Count() const { return num; }

    // 1発発射
    void Fire(double x, double y, double rad, double speed);
    // 扇状に発射 (ways=弾数, stepRad=弾同士の角度差)
    void FireFan(double x, double y, double centerRad, int ways, double stepRad, double speed);
    // 全方位に発射
    void FireRing(double x, double y, int n, double offsetRad, double speed);

    void Update();
    void Draw() const;
    // 円(x,y,r)に当たっている弾があるか
    bool Hit(double x, double y, double r) const;
};
#endif