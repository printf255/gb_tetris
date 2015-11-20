#ifndef MODEL_H
#define MODEL_H

#include <gb/gb.h>
#include "game_state.h"

extern const UINT8 SCORE_PER_LINE;
extern const UINT8 SCORE_PER_BLOCK;
extern const UINT8 GRID_WIDTH;
extern const UINT8 GRID_HEIGHT;

extern BOOLEAN grid[10][18];
extern UINT8 grid_tiles[10][18];
extern volatile enum game_state game_state;
extern UINT16 time_played;
extern UINT32 score;
extern UINT8 speed;
extern UINT16 lines_cleared;
extern INT8 current_block_pos_x;
extern INT8 current_block_pos_y;
extern UINT8 step_time;//in 16th seconds
extern BOOLEAN has_current_block;

extern UINT16 frame_counter;
extern UINT16 fps;

extern BOOLEAN ga_changed;
extern BOOLEAN intf_changed;


void reset_model();

BOOLEAN get_grid_cell(UINT8 x, UINT8 y);
void set_grid_cell(UINT8 x, UINT8 y, BOOLEAN value);

void increase_score(UINT32 amount);
void increase_lines_cleared();
void set_current_block_pos(INT8 x, INT8 y);






#endif
