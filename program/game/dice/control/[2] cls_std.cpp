/* ### ダイスコントロールクラス ###*/
#include <time.h>
#include "[2] cls_std.h"

std::mt19937 &DICE_RNG::Engine()
{
    static std::mt19937 engine = []() {
        std::random_device rd;
        std::seed_seq seq{rd(), static_cast<unsigned int>(time(NULL)),
                          static_cast<unsigned int>(clock())};
        return std::mt19937(seq);
    }();
    return engine;
}

int DICE_RNG::Range(int lo, int hi)
{
    std::uniform_int_distribution<int> dist(lo, hi);
    return dist(Engine());
}