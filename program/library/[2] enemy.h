class ENEMY{
    double x,y;
    int gh[3];
    
    int width,height;
    int in_time,stop_time,out_time;
    int type;
    int stype;
    int m_pattern;
    int s_pattern;
    int count;
    // 敵消滅フラグ
    bool deadflag;
    bool endflag;

    // 弾構造体
    E_SHOT shot[ENEMY_SNUM];
    // ショットが撃てるようになったかのフラグ
    bool sflag;
    // ショットが撃てるようになってからのカウント
    int scount;

public:
    bool All();
    void Move();
    void Draw();
    ENEMY();
    ENEMY(char *c_filename,int type,int m_pattern,int x,int y,
            int in_time,int stop_time, int shot_time,int out_time,
                char *s_filename,int stype,int s_pattern,int speed);
};
