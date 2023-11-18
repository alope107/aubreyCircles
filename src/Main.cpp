#include "bn_core.h"
#include "bn_sprite_ptr.h"

#include "bn_sprite_items_circle.h"


int main() {
    bn::core::init();

    bn::sprite_ptr circle_spr = bn::sprite_items::circle.create_sprite(-72, -31);
    
    circle_spr.set_scale(circle_spr.vertical_scale() * 2);

    while(true) {
        circle_spr.set_x(circle_spr.x() + 1);
        
        bn::core::update();
    }
}