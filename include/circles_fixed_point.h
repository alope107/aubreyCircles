#include "fixed_point_t.h"

const int PRECISION = 19;

using fixed = bn::fixed_t<PRECISION>;
using fixed_point = circles::fixed_point_t<PRECISION>;

inline fixed_point new_point(float x, float y) { return fixed_point(fixed(x), fixed(y));}
inline fixed_point new_point(fixed x, fixed y) { return fixed_point(x, y);}