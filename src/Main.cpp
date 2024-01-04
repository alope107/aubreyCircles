#include "bn_core.h"
#include "bn_sprite_ptr.h"

#include "bn_sprite_items_circle.h"
#include "bn_keypad.h"
#include <bn_vector.h>
#include <bn_memory.h>
#include <bn_fixed_point.h>
#include <bn_math.h>
#include "bn_log.h"
#include "bn_string.h"
#include "fixed_point_t.h"
#include "wake_geometry.h"

const int PRECISION = 19;
const int MAX_ORBITERS = 200;

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

fixed squared_distance(fixed_point a, fixed_point b) {
    fixed_point delta = a - b;
    return delta.x()*delta.x() + delta.y()*delta.y();
}

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
    public:
        fixed_point loc;
        fixed_point velocity;
        fixed radius;
        wake::Rectangle bounding;

        Orbiter(fixed_point starting_loc, fixed_point starting_velocity, fixed sprite_radius, bn::sprite_item sprite_item, Attractor attractor=ATTRACTOR) :
        loc(starting_loc),
        velocity(starting_velocity),
        radius(sprite_radius),
        _sprite(sprite_item.create_sprite(starting_loc)),
        _attractor(attractor)
        {
            _update_bounding();
        };

        void update() {
            fixed_point delta = loc - _attractor.location;
            fixed_point force = new_point(delta.x(), delta.y()) * -_attractor.mass;
            velocity += force;
            loc += velocity;
            // log("space_location", _space_location.x(), _space_location.y());
            // log("screen_location", screen_location().x(), screen_location().y());
            _sprite.set_position(loc);
            _update_bounding();
        }

        bool overlaps(Orbiter other) {
            log("My Bounding: (", bounding.top_left.x(), ",", bounding.top_left.y(), ")  (",
                                  bounding.bottom_right.x(), ",", bounding.bottom_right.y(), ")");

            log("Their Bounding: (", other.bounding.top_left.x(), ",", other.bounding.top_left.y(), ")  (",
                                     other.bounding.bottom_right.x(), ",", other.bounding.bottom_right.y(), ")");
            return wake::rects_overlap(bounding, other.bounding);
            // const fixed min_dist = radius + other.radius;
            // const fixed squared_min_dist = (min_dist * min_dist);
            // const fixed squared_dist = squared_distance(loc, other.loc);
            // log("min_dist", min_dist, "squared_min_dist", squared_min_dist, "squared_dist", squared_dist);
            // return squared_dist <= squared_min_dist;
        }
    
    private:
        bn::sprite_ptr _sprite;
        Attractor _attractor;

        void _update_bounding() {
            bounding = wake::Rectangle(loc - fixed_point(radius, radius),
                                        loc + fixed_point(radius, radius));
        }
};

int main() {
    bn::core::init();

    fixed_point vec_start(0, 0);

    bool start_set = false;

    bn::vector<Orbiter, MAX_ORBITERS> frems;
    bool to_remove[MAX_ORBITERS] = {};
    
    bn::sprite_ptr cursor = bn::sprite_items::circle.create_sprite(30.5, 40.5);
    while(true) {
        if (bn::keypad::a_pressed()) {
            if (!start_set) {
                vec_start = cursor.position();
            } else {
                auto starting_velocity = (fixed_point(cursor.position()) - vec_start) * LAUNCH_SCALE;
                frems.push_back(Orbiter(vec_start, starting_velocity, 8, bn::sprite_items::circle)); 
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

        //bn::memory::clear(MAX_ORBITERS, to_remove);

        for (int i = 0; i < frems.size(); i++) {
            for (int j = i+1; j < frems.size(); j++) {
                if(frems[i].overlaps(frems[j])) {
                    to_remove[i] = to_remove[j] = true;
                }
            }
        }

        for (int i = MAX_ORBITERS - 1; i>= 0; i--) {
            if(to_remove[i]) {
                frems.erase(frems.begin() + i);
            }
            to_remove[i] = false;
        }

        bn::core::update();
    }
}