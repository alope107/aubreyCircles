#ifndef WAKEGEOM
#define WAKEGEOM

#include "bn_fixed_point.h"

namespace wake {
    struct Rectangle {
        bn::fixed_point top_left;
        bn::fixed_point bottom_right;
    };

    template <typename T>
    inline bool point_in_rect(T x, T y, T left, T right, T top, T bottom) {
        bool in = x >= left && x <= right && y >= top && y <= bottom;
        return in;
    }

    inline bool point_in_rect(bn::fixed_point point, 
                              bn::fixed_point top_left,
                              bn::fixed_point bottom_right) {
        return point_in_rect(point.x(), point.y(), top_left.x(), bottom_right.x(), top_left.y(), bottom_right.y());
    };

    inline bool point_in_rect(bn::fixed_point point, Rectangle rect) {
        return point_in_rect(point.x(), point.y(), rect.top_left.x(), rect.bottom_right.x(), rect.top_left.y(), rect.bottom_right.y());
    }

    template <typename T>
    inline bool spans_overlap(T min1, T max1, T min2, T max2) {
        return (min1 <= max2 && max1 >= min2) || (min2 <= max1 && max2 >= min1);
    }

    inline bool rects_overlap(Rectangle r1, Rectangle r2) {
        return spans_overlap(r1.top_left.x(), r1.bottom_right.x(), r2.top_left.x(), r2.bottom_right.x()) &&
               spans_overlap(r1.top_left.y(), r1.bottom_right.y(), r2.top_left.y(), r2.bottom_right.y());
    }
}

#endif // WAKEGEOM