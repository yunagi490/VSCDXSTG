#ifndef _CLS_STD
#define _CLS_STD

#include <random>

/* ### ダイス用の乱数クラス ###
   全てのダイスが同じ乱数エンジンを共有する */
class DICE_RNG
{
public:
    static std::mt19937 &Engine();
    // lo〜hi (両端を含む)
    static int Range(int lo, int hi);
};
#endif