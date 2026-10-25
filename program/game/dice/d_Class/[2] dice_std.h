#ifndef _DICE_STD
#define _DICE_STD

// 汎用ダイス : NdM (M面ダイスをN個振った合計)
class DICE_STD
{
protected:
    int num;   // ダイスの個数
    int sides; // 面数
    int last;  // 直前の出目 (合計)

public:
    DICE_STD(int n, int s) : num(n), sides(s), last(0) {}
    virtual ~DICE_STD() {}
    int Roll();
    int Last() const { return last; }
    int Sides() const { return sides; }
};
#endif