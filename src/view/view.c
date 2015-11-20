#include "view.h"

#include "../../../gb_framework/src/gfx.h"
#include "../../../gb_framework/src/log.h"
#include "../../../gb_framework/src/tiles/colorRects.h"
#include "../../../gb_framework/src/log.h"

#include "../model/model.h"
#include "../block/block.h"
#include "gfx_tetris.h"

#include "../tiles/blocks.h"

#include <stdio.h>

const UINT8 NUM_CELLS_X = 10;
const UINT8 NUM_CELLS_Y = 18;
BOOLEAN debug_rendering = FALSE;
/*BOOLEAN debug_rendering = TRUE;*/

UINT8 next_tile = FIRST_FREE_TILE;
UINT8 TILE_WHITE;
UINT8 TILE_LIGHT_GREY;
UINT8 TILE_DARK_GREY;
UINT8 TILE_BLACK;
UINT8 TILE_BLOCKS_START;

BOOLEAN render_ga = FALSE;
BOOLEAN render_intf = TRUE;

void render_interface();
void render_game_area();
void render_debug_interface();


void initialize_view(){

    /*char text[20];*/

    TILE_WHITE = next_tile++;
    TILE_LIGHT_GREY = next_tile++;
    TILE_DARK_GREY = next_tile++;
    TILE_BLACK = next_tile++;

    set_bkg_data(TILE_WHITE, 4, colorRects);

    TILE_BLOCKS_START = next_tile;
    /*sprintf(text, "bla %u", TILE_BLOCKS_START);*/
    /*add_log_line(text);*/
    next_tile += 7;
    set_bkg_data(TILE_BLOCKS_START, 7, blocks);

}

void render(){

    UINT8 i;
    char text[20];

    /*if(render_ga){*/
        /*render_ga = !render_ga;*/
        /*set_bkg_mul(1, 0, 10, 18, 0);*/
        /*render_game_area();*/


    /*}*/
    /*else{*/
        /*render_ga = !render_ga;*/
        /*set_bkg_mul(12, 0, 7, 18, 0);*/
        /*set_bkg_mul(0, 0, 1, 18, 0);*/
        /*if(debug_rendering){*/
            /*render_debug_interface();*/
        /*}*/
        /*render_interface();*/

    /*}*/


    /*set_bkg_mul(0, 0, 20, 18, TILE_BLACK);*/
    /*set_bkg_mul(1, 0, 10, 18, TILE_BLACK);*/

    /*if(debug_rendering){*/
        /*render_debug_interface();*/
        /*for(i = 0; i < 18; i++){*/
            /*set_bkg_tiles(0, i, 11, 1, bkg[i]);*/
        /*}*/
    /*}*/
    /*else{*/
        /*render_game_area();*/
        /*//write game_area bkg*/
        /*[>set_bkg_tiles(1, 0, 10, 18, bkg);<]*/
        /*[>set_bkg_tiles(0, 0, 20, 8, bkg);<]*/
        /*for(i = 0; i < 18; i++){*/
            /*set_bkg_tiles(0, i, 11, 1, bkg[i]);*/
        /*}*/

        /*if(render_int){*/
            /*render_int = !render_int;*/
            /*[>render_interface();<]*/
            /*[>set_bkg_tiles(12, 0, 7, 18, bkg);<]*/
            /*[>set_bkg_tiles(0, 0, 1, 18, bkg);<]*/
        /*}*/
        /*else{*/
            /*render_int = !render_int;*/

        /*}*/
    /*}*/


    /*if(debug_rendering){*/
        /*//TODO*/
    /*}*/
    /*else{*/
        /*set_ga_mul(0, 0, 10, 18, 0);*/
        /*render_game_area();*/
        /*write_ga();*/
        /*if(render_intf){*/
            /*render_intf = !render_intf;*/
            /*set_intf_mul(0, 10, 10, 8, 0);*/
            /*render_interface();*/
            /*write_intf();*/
        /*}*/
        /*else{*/
            /*render_intf = !render_intf;*/
        /*}*/
    /*}*/


    /*frame_counter++;*/
    /*fps = frame_counter / (time16/16);*/

    if(debug_rendering){
        set_bkg_mul(0, 0, 20, 18, 0);
        render_debug_interface();
        write_bkg();
    }
    else{

        if(ga_changed){
            ga_changed = FALSE;

            set_ga_mul(0, 0, 10, 18, 0);
            render_game_area();
            write_ga();
        }
        if(intf_changed){
            intf_changed = FALSE;

            if(intf_full_clear_needed){
                sprintf(text, "full clear");
                add_log_line(text);
                intf_full_clear_needed = FALSE;
                set_intf_mul(0, 0, 10, 18, TILE_WHITE);
            }
            else{
                set_intf_mul(0, 10, 10, 8, TILE_WHITE);
            }

            render_interface();
            write_intf();
        }


    }





    /*write_bkg();*/

}

void render_debug_interface(){

    char text[20];
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

    sprintf(text, "block x %u", current_block_pos_x);
    render_text(0, 2, text);
    sprintf(text, "block y %u", current_block_pos_y);
    render_text(0, 3, text);

    render_log_text(8);
    /*render_log_text(0);*/


}

void render_interface(){

    char text[20];

    //game area bounds

    //info text
    /*text = "lines";*/

    //time played
    sprintf(text, "time");
    render_text_intf(2, 1, text);
    sprintf(text, "%u", time_played/16);
    render_text_intf(2, 2, text);

    //lines
    sprintf(text, "lines");
    render_text_intf(2, 3, text);
    sprintf(text, "%u", lines_cleared);
    render_text_intf(2, 4, text);

    //score
    sprintf(text, "score");
    render_text_intf(2, 5, text);
    sprintf(text, "%u", score);
    render_text_intf(2, 6, text);

    //speed
    sprintf(text, "speed");
    render_text_intf(2, 7, text);
    sprintf(text, "%u", speed);
    render_text_intf(2, 8, text);

    //fps
    /*sprintf(text, "fps");*/
    /*render_text_intf(2, 9, text);*/
    /*sprintf(text, "%u", fps);*/
    /*render_text_intf(2, 10, text);*/

    //game state
    /*game_state = GAME_STATE_PAUSED;*/
    /*game_state = GAME_STATE_GAMEOVER;*/
    if(game_state == GAME_STATE_PAUSED){
        sprintf(text, "paused");
        render_text_intf(2, 12, text);
    }
    else if(game_state == GAME_STATE_GAMEOVER){
        sprintf(text, "game");
        render_text_intf(1, 12, text);
        sprintf(text, "over");
        render_text_intf(6, 12, text);

        sprintf(text, "press");
        render_text_intf(3, 13, text);
        sprintf(text, "start");
        render_text_intf(1, 14, text);
        sprintf(text, "for");
        render_text_intf(7, 14, text);

        sprintf(text, "a");
        render_text_intf(3, 15, text);
        sprintf(text, "new");
        render_text_intf(5, 15, text);
        sprintf(text, "game");
        render_text_intf(4, 16, text);
    }

}

void render_game_area(){

    UINT8 x, y, rect_x, rect_y;
    char text[20];

    /*blocks that have fallen down*/
    for(x = 0; x < NUM_CELLS_X; x++){
        for(y = 0; y < NUM_CELLS_Y; y++){
            if(grid[x][y] && grid_tiles[x][y] != 0){
                /*set_bkg(x+1, y, grid_tiles[x][y]);*/
                set_ga(x, y, grid_tiles[x][y]);
            }
        }
    }

    //current block
    if(has_current_block){
        for(x = 0; x < 4; x++){
            for(y = 0; y < 4; y++){
                if((*get_block_grid())[x][y]){
                    rect_x = (current_block_pos_x + x);
                    rect_y = (current_block_pos_y + y);
                    /*set_bkg(rect_x, rect_y, get_tile());*/
                    /*set_bkg(rect_x, rect_y, get_shape_type() + TILE_BLOCKS_START);*/
                    set_ga(rect_x, rect_y, get_shape_type() + TILE_BLOCKS_START);
                }
            }
        }
    }

}




