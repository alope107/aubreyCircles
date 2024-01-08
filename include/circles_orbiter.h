#ifndef CIRCLES_ORBITER
#define CIRCLES_ORBITER

#include "circles_fixed_point.h"
#include "bn_sprite_ptr.h"
#include "bn_sprite_item.h"
#include "circles_attractor.h"

class Orbiter {
    public:
        fixed_point loc;
        fixed_point velocity;
        fixed length;

        Orbiter(fixed_point starting_loc, 
                fixed_point starting_velocity,
                fixed sprite_length,
                bn::sprite_item sprite_item,
                int tile_idx,
                Attractor attractor);

        void update();
        bool overlaps(Orbiter other);
    
    private:
        bn::sprite_item _sprite_item;
        bn::sprite_ptr _sprite;
        int _tile_idx;
        Attractor _attractor;
};

#endif