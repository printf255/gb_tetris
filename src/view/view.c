#include "view.h"
#include "../model/model.h"
#include "../../../gb_framework/src/gfx.h"

#include <stdio.h>

const UINT8 NUM_CELLS_X = 10;
const UINT8 NUM_CELLS_Y = 18;

void render_interface();
void render_game_area();


void render(){
    char text[50];
    UINT8 keys;
    sprintf(text, "time: %u", time_played/16);
    render_text(0, 0, text);

    if(game_state == GAME_STATE_PLAYING)
        render_text(0, 1, "playing");
    else if(game_state == GAME_STATE_PAUSED)
        render_text(0, 1, "paused");
    else if(game_state == GAME_STATE_GAMEOVER)
        render_text(0, 1, "gameOver");

    /*set_bkg(0, 2, 0);*/
    set_bkg_mul(0, 2, 8, 1, 0);

    keys = joypad();
    if(keys & J_LEFT){
        render_text(0, 2, "L");
        waitpadup();
    }
    if(keys & J_RIGHT)
        render_text(1, 2, "R");
    if(keys & J_UP)
        render_text(2, 2, "U");
    if(keys & J_DOWN)
        render_text(3, 2, "D");
    if(keys & J_A)
        render_text(4, 2, "A");
    if(keys & J_B)
        render_text(5, 2, "B");
    if(keys & J_START)
        render_text(6, 2, "T");
    if(keys & J_SELECT)
        render_text(7, 2, "E");


    write_bkg();

}

void render_interface(){

}

void render_game_area(){

}




