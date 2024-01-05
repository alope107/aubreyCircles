#include "bn_core.h"
#include "bn_sprite_ptr.h"

#include "bn_sprite_items_cute.h"
#include "bn_keypad.h"
#include <bn_vector.h>
#include <bn_memory.h>
#include <bn_fixed_point.h>
#include <bn_math.h>
#include "bn_log.h"
#include "bn_string.h"
#include "fixed_point_t.h"
#include "bn_sprite_text_generator.h"

#include "common_variable_8x16_sprite_font.h"

const int PRECISION = 19;
const int MAX_ORBITERS = 200;

using fixed = bn::fixed_t<PRECISION>;
using fixed_point = circles::fixed_point_t<PRECISION>;

inline fixed_point new_point(float x, float y) { return fixed_point(fixed(x), fixed(y));}
inline fixed_point new_point(fixed x, fixed y) { return fixed_point(x, y);}

const fixed SCALE = fixed(1);
const fixed LAUNCH_SCALE = fixed(.03);

const auto current_score_loc = fixed_point(100, -60);
const auto high_score_loc = fixed_point(-100, -60);

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
        fixed length;

        Orbiter(fixed_point starting_loc, fixed_point starting_velocity, fixed sprite_length, bn::sprite_item sprite_item, Attractor attractor=ATTRACTOR) :
        loc(starting_loc),
        velocity(starting_velocity),
        length(sprite_length),
        _sprite(sprite_item.create_sprite(starting_loc)),
        _attractor(attractor)
        {};

        void update() {
            fixed_point delta = loc - _attractor.location;
            fixed_point force = new_point(delta.x(), delta.y()) * -_attractor.mass;
            velocity += force;
            loc += velocity;
            _sprite.set_position(loc);
        }

        bool overlaps(Orbiter other) {
            //currently assumes we're squares of the same size
            return (bn::abs(loc.x() - other.loc.x()) < length) &&
                   (bn::abs(loc.y() - other.loc.y()) < length);
        }
    
    private:
        bn::sprite_ptr _sprite;
        Attractor _attractor;
};

int main() {
    bn::core::init();

    bn::sprite_font font = common::variable_8x16_sprite_font;
    bn::sprite_text_generator text_generator(font);

    fixed_point vec_start(0, 0);

    bool start_set = false;

    bn::vector<Orbiter, MAX_ORBITERS> frems;
    bool to_remove[MAX_ORBITERS] = {};

    int high_score = 0;
    
    bn::sprite_ptr cursor = bn::sprite_items::cute.create_sprite(30.5, 40.5);
    while(true) {
        if (bn::keypad::a_pressed()) {
            if (!start_set) {
                vec_start = cursor.position();
            } else {
                auto starting_velocity = (fixed_point(cursor.position()) - vec_start) * LAUNCH_SCALE;
                frems.push_back(Orbiter(vec_start, starting_velocity, 14, bn::sprite_items::cute)); 
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

        for (int i = 0; i < frems.size(); i++) {
            for (int j = i+1; j < frems.size(); j++) {
                if(frems[i].overlaps(frems[j])) {
                    to_remove[i] = to_remove[j] = true;
                }
            }
        }

        for (int i = frems.size() - 1; i>= 0; i--) {
            if(to_remove[i]) {
                frems.erase(frems.begin() + i);
            }
            to_remove[i] = false;
        }

        for (auto &frem : frems) {
            frem.update();
        }

        high_score = frems.size() > high_score ? frems.size() : high_score;

         bn::vector<bn::sprite_ptr, 32> text_sprites;
         text_generator.generate(current_score_loc, bn::to_string<16>(frems.size()), text_sprites);
         text_generator.generate(high_score_loc, bn::to_string<16>(high_score), text_sprites);

        bn::core::update();
    }
}