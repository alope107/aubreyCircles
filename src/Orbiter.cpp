#include <bn_math.h>
#include <bn_fixed_point.h>
#include "bn_sprite_tiles_ptr.h"
#include "circles_orbiter.h"

Orbiter::Orbiter(fixed_point starting_loc, 
                fixed_point starting_velocity,
                fixed sprite_length,
                bn::sprite_item sprite_item,
                int tile_idx,
                Attractor attractor) :
    loc(starting_loc),
        velocity(starting_velocity),
        length(sprite_length),
        _sprite_item(sprite_item),
        _sprite(sprite_item.create_sprite(starting_loc)),
        _tile_idx(tile_idx),
        _attractor(attractor)
        {
            _sprite.set_tiles(_sprite_item.tiles_item().create_tiles(tile_idx));
        };

void Orbiter::update() {
            fixed_point delta = loc - _attractor.location;
            fixed_point force = new_point(delta.x(), delta.y()) * -_attractor.mass;
            velocity += force;
            loc += velocity;
            _sprite.set_position(loc);
        }
bool Orbiter::overlaps(Orbiter other) {
            //currently assumes we're squares of the same size
            return (bn::abs(loc.x() - other.loc.x()) < length) &&
                   (bn::abs(loc.y() - other.loc.y()) < length);
        }