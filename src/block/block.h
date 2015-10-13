#ifndef BLOCK_H
#define BLOCK_H

#include <gb/gb.h>

extern const SHAPE_TYPE_I;
extern const SHAPE_TYPE_J;
extern const SHAPE_TYPE_L;
extern const SHAPE_TYPE_O;
extern const SHAPE_TYPE_S;
extern const SHAPE_TYPE_T;
extern const SHAPE_TYPE_Z;

extern const BOOLEAN shape_grids[7][4][4][4];

void rotate();
void new_block(UINT8 type);

BOOLEAN** get_grid();
UINT8 get_tile();



#endif
