#include "model.h"

const UINT8 SCORE_PER_LINE = 100;
const UINT8 SCORE_PER_BLOCK = 10;
const UINT8 GRID_WIDTH = 10;
const UINT8 GRID_HEIGHT = 18;

BOOLEAN grid[10][18];
UINT8 grid_tiles[10][18];
volatile enum game_state game_state = GAME_STATE_GAMEOVER;
/*volatile enum game_state game_state = GAME_STATE_PLAYING;*/
UINT16 time_played = 0;
UINT32 score = 0;
UINT8 speed = 1;
UINT16 lines_cleared = 0;
INT8 current_block_pos_x = 0;
INT8 current_block_pos_y = 0;
UINT8 step_time = 16;//in 16th seconds
BOOLEAN has_current_block = FALSE;

UINT16 frame_counter = 0;
UINT16 fps = 0;

BOOLEAN ga_changed = TRUE;
BOOLEAN intf_changed = TRUE;


BOOLEAN get_grid_cell(UINT8 x, UINT8 y){
    if(x < 0 || x >= GRID_WIDTH)
        return TRUE;
    if(y < 0 || y >= GRID_HEIGHT)
        return TRUE;
    return grid[x][y];
}

void set_grid_cell(UINT8 x, UINT8 y, BOOLEAN value){
    if(x < 0 || x >= GRID_WIDTH)
        return;
    if(y < 0 || y >= GRID_HEIGHT)
        return;
    grid[x][y] = value;
}

void reset_model(){
    UINT8 i = 0;
    UINT8 j = 0;

    for(i = 0; i < 10; i++){
        for(j = 0; j < 18; j++){
            grid[i][j] = FALSE;
            grid_tiles[i][j] = 0;
        }
    }

    game_state = GAME_STATE_GAMEOVER;
    time_played = 0;
    score = 0;
    speed = 1;
    current_block_pos_x = 0;
    current_block_pos_y = 0;
    step_time = 16;
    lines_cleared = 0;





}

void increase_score(UINT32 amount){
    score += amount;
}

void increase_lines_cleared(){
    lines_cleared++;
}

void set_current_block_pos(INT8 x, INT8 y){
    current_block_pos_x = x;
    current_block_pos_y = y;
}




