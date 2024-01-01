#include "bn_core.h"
#include "bn_sprite_ptr.h"

#include "bn_sprite_items_circle.h"
#include "bn_keypad.h"
#include <bn_vector.h>
#include <bn_fixed_point.h>
#include <bn_math.h>
#include "bn_log.h"
#include "bn_string.h"
#include "fixed_point_t.h"

const int PRECISION = 19;

using fixed = bn::fixed_t<PRECISION>;
using fixed_point = circles::fixed_point_t<PRECISION>;

inline fixed_point new_point(float x, float y) { return fixed_point(fixed(x), fixed(y));}
inline fixed_point new_point(fixed x, fixed y) { return fixed_point(x, y);}

const fixed SCALE = fixed(1);
const fixed LAUNCH_SCALE = fixed(.03);

struct Attractor {
    fixed_point location;
    fixed mass;
};

const Attractor ATTRACTOR = {
    new_point(0, 0),
    .0001,
};

template<int maxSize=10000, typename... Types>
    inline void log(const Types&... vals) {
        // Something bizarre is going on here...
        // bn::to_string<maxSize>(" ") could just be of size 1, and it works if there's more than one parameter
        // But it fails if there's just one???
        // Maybe something with += vs = compiler optimization something something...
        // This should probably be using an ostringstream anyway
        bn::string<maxSize> message = "";
        (message += ... += (bn::to_string<maxSize>(vals) + bn::to_string<maxSize>(" ")));
        bn::log(message);
    }

class Orbiter {
    fixed_point _space_location;
    fixed_point _velocity;
    bn::sprite_ptr _sprite;
    Attractor _attractor;
    fixed _scale;


    public:
        Orbiter(fixed_point starting_loc, fixed_point starting_velocity, bn::sprite_item sprite_item, Attractor attractor=ATTRACTOR, fixed scale=SCALE) :
        _space_location(starting_loc),
        _velocity(starting_velocity),
        _scale(scale),
        _sprite(sprite_item.create_sprite(bn::fixed_point(starting_loc / scale))),
        _attractor(attractor)
        {};
        
        fixed_point screen_location() {
            return _space_location / _scale;
        }

        void set_space_location(fixed_point location) {
            _space_location = location;
        }

        void set_velocity(fixed_point velocity) {
            _velocity = velocity;
        }

        void update() {
            fixed_point delta = _space_location - _attractor.location;
            fixed_point force = new_point(delta.x(), delta.y()) * -_attractor.mass;
            _velocity += force;
            _space_location += _velocity;
            // log("space_location", _space_location.x(), _space_location.y());
            // log("screen_location", screen_location().x(), screen_location().y());
            _sprite.set_position(screen_location());
        }
};

fixed scaled_squared_distance(fixed_point a, fixed_point b, fixed s) {
    fixed_point delta = a - b;
    delta /= s;
    return delta.x()*delta.x() + delta.y()*delta.y();
}

int main() {
    bn::core::init();

    fixed_point vec_start(0, 0);

    bool start_set = false;
    bn::vector<Orbiter, 200> frems;
    
    bn::sprite_ptr cursor = bn::sprite_items::circle.create_sprite(30.5, 40.5);
    while(true) {
        if (bn::keypad::a_pressed()) {
            if (!start_set) {
                vec_start = cursor.position();
            } else {
                auto starting_velocity = (fixed_point(cursor.position()) - vec_start) * LAUNCH_SCALE;
                frems.push_back(Orbiter(vec_start, starting_velocity, bn::sprite_items::circle)); 
            }

            start_set = !start_set;
        }

        if (bn::keypad::left_held()) {
            cursor.set_x(cursor.x() - 1);
        }
        if (bn::keypad::right_held()) {
            cursor.set_x(cursor.x() + 1);
        }
        if (bn::keypad::up_held()) {
            cursor.set_y(cursor.y() - 1);
        }
        if (bn::keypad::down_held()) {
            cursor.set_y(cursor.y() + 1);
        }

        if(bn::keypad::start_pressed()) {
            frems.clear();
        }
        
        for (auto &frem : frems) {
            frem.update();
        }
        
        bn::core::update();
    }
}