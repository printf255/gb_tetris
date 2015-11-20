#include "view.h"

#include "../../../gb_framework/src/gfx.h"
#include "../../../gb_framework/src/tiles/colorRects.h"

#include "../model/model.h"
#include "../block/block.h"
#include "gfx_tetris.h"

#include "../tiles/blocks.h"

#include <stdio.h>

const UINT8 NUM_CELLS_X = 10;
const UINT8 NUM_CELLS_Y = 18;

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


void initialize_view(){

    TILE_WHITE = next_tile++;
    TILE_LIGHT_GREY = next_tile++;
    TILE_DARK_GREY = next_tile++;
    TILE_BLACK = next_tile++;

    set_bkg_data(TILE_WHITE, 4, colorRects);

    TILE_BLOCKS_START = next_tile;
    next_tile += 7;
    set_bkg_data(TILE_BLOCKS_START, 7, blocks);

}

void render(){

    if(ga_changed){
        ga_changed = FALSE;

        set_ga_mul(0, 0, 10, 18, 0);
        render_game_area();
        write_ga();
    }
    if(intf_changed){
        intf_changed = FALSE;

        if(intf_full_clear_needed){
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


void render_interface(){

    char text[20];

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

    //game state
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

    /*blocks that have fallen down*/
    for(x = 0; x < NUM_CELLS_X; x++){
        for(y = 0; y < NUM_CELLS_Y; y++){
            if(grid[x][y] && grid_tiles[x][y] != 0){
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
                    set_ga(rect_x, rect_y, get_shape_type() + TILE_BLOCKS_START);
                }
            }
        }
    }

}




