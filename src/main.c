#include "main.h"

#include <gb/gb.h>
#include <stdio.h>
/*#include <rand.h>*/

#include "../../gb_framework/src/timer.h"
#include "../../gb_framework/src/sound.h"
#include "../../gb_framework/src/gfx.h"
#include "../../gb_framework/src/input.h"
#include "../../gb_framework/src/log.h"

#include "model/model.h"
#include "view/view.h"
#include "controller/controller.h"



int main(){

    UINT16 time16_last_frame = time16;
    UINT16 time16_diff;
    char text[20];
    UINT8 i;
    /*UINT16 frame_counter = 0;*/
    /*UINT16 fps = 0;*/

    /*initrand( ((UINT16)DIV_REG << 8) | DIV_REG);*/

    initialize_timer();
    initialize_sound();
    initialize_font();
    initialize_view();
    set_log_num_lines(10);
    reset_model();



    /*for(i = 0; i < 12; i++){*/
        /*sprintf(text, "%u", i);*/
        /*add_log_line(text);*/
    /*}*/

    /*HIDE_WIN;*/
    /*HIDE_SPRITES;*/

    while(1){
        /*char text[64];*/

        time16_diff = time16 - time16_last_frame;
        time16_last_frame = time16;

        /*wait_vbl_done();*/

        refresh_button_states();
        update(time16_diff);

        render();
        /*wait_vbl_done();*/
        /*set_bkg_mul(0, 0, 20, 18, 0);*/
        /*wait_vbl_done();*/
        /*write_bkg();*/

        frame_counter++;
        fps = frame_counter / (time16/16);
        sprintf(text, "fps %u", fps);
        /*add_log_line(text);*/




    }

//mainloop



}
