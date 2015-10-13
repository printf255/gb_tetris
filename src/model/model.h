#ifndef MODEL_H
#define MODEL_H

#include <gb/gb.h>
#include "game_state.h"

extern const UINT8 SCORE_PER_LINE;
extern const UINT8 GRID_WIDTH;
extern const UINT8 GRID_HEIGHT;

BOOLEAN grid[10][18];
UINT8 grid_tiles[10][18];
enum game_state game_state = GAME_STATE_GAMEOVER;
float time_played = 0.0f;
UINT32 score = 0;
UINT8 speed = 1;
UINT16 lines_cleared = 0;
INT8 current_block_pos_x = 0;
INT8 current_block_pos_y = 0;
//currentBlock?
UINT8 step_time = 16;//in 16th seconds


void reset_model();

void increase_score(UINT32 amount);
void increase_lines_cleared();
void set_current_block_pos(INT8 x, INT8 y);






#endif
