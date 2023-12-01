#include "bn_core.h"
#include "bn_sprite_ptr.h"

#include "bn_sprite_items_circle.h"
#include "bn_keypad.h"
#include <bn_vector.h>
#include <bn_fixed_point.h>
#include <bn_math.h>

bn::fixed scaled_squared_distance(bn::fixed_point a, bn::fixed_point b, bn::fixed s) {
    bn::fixed_point delta = a - b;
    delta /= s;
    return delta.x()*delta.x() + delta.y()*delta.y();
}
int main() {
    bn::core::init();

    bn::fixed_point delta(10, 0);
    bn::fixed_point mass_loc(0, 0);

    bn::fixed_point vec_start(0, 0);

    bool start_set = false;
    // int x = -72;
    // int y = -31;
    // int dx = 10;
    // int dy = 10;

    bn::sprite_ptr circle_spr = bn::sprite_items::circle.create_sprite(-72, -31);
    bn::vector<bn::sprite_ptr, 10> frems;
    //bn::sprite_ptr new_spr = bn::sprite_items::circle.create_sprite(circle_spr.x(), circle_spr.y());
    
    
    bn::sprite_ptr cursor = bn::sprite_items::circle.create_sprite(0, 0);

    circle_spr.set_scale(circle_spr.vertical_scale() * 2);

    while(true) {
        bn::fixed scale = 1 / scaled_squared_distance(circle_spr.position(), mass_loc, 8);
        bn::fixed_point force = (mass_loc - circle_spr.position()) * scale;

        delta += force;

        circle_spr.set_position(circle_spr.position() + delta);

        if (bn::keypad::a_pressed()) {
            if (!start_set) {
                vec_start = cursor.position();
            } else {
                delta = (cursor.position() - vec_start) / 10;
                circle_spr.set_position(vec_start);
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

        // if (bn::keypad::a_pressed()) {
        //     circle_spr.set_x(circle_spr.x() + 1);
        //     bn::sprite_ptr new_spr = bn::sprite_items::circle.create_sprite(circle_spr.x(), circle_spr.y());
        //     frems.push_back(new_spr);
        // }
        
        
        bn::core::update();
    }
}