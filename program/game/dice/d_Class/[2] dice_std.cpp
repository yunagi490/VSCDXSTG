#include "[2] dice_std.h"
#include "[2] cls_std.h"

int DICE_STD::Roll()
{
    last = 0;
    for (int i = 0; i < num; ++i)
        last += DICE_RNG::Range(1, sides);
    return last;
}