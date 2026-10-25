#ifndef _DICE_1D6
#define _DICE_1D6

#include "[2] dice_std.h"

// 1d6 : ボスステージ終了時に振る。6が出ればファンブル。
class DICE_1D6 : public DICE_STD
{
public:
    DICE_1D6() : DICE_STD(1, 6) {}
    bool IsFumble() const { return last == 6; }
};
#endif