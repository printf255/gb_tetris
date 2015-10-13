#include "model.h"

const UINT8 SCORE_PER_LINE = 100;
const UINT8 GRID_WIDTH = 10;
const UINT8 GRID_HEIGHT = 20;

void reset_model(){
    UINT8 i = 0;
    UINT8 j = 0;

    for(i = 0; i < 10; i++){
        for(j = 0; j < 18; j++){
            grid[i][j] = FALSE;
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




