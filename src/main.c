#include "main.h"

#include <gb/gb.h>

#include "../../gb_framework/src/timer.h"
#include "../../gb_framework/src/sound.h"
#include "../../gb_framework/src/gfx.h"
#include "../../gb_framework/src/input.h"

#include "model/model.h"
#include "view/view.h"
#include "controller/controller.h"


int main(){

    UINT16 time16_last_frame = time16;
    UINT16 time16_diff;

    initialize_timer();
    initialize_sound();
    initialize_font();
    initialize_view();
    reset_model();

    while(1){

        time16_diff = time16 - time16_last_frame;
        time16_last_frame = time16;

        refresh_button_states();
        update(time16_diff);
        render();

    }

}

