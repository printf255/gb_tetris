#ifndef BLOCK_H
#define BLOCK_H

#include <gb/gb.h>

extern const UINT8 SHAPE_TYPE_I;
extern const UINT8 SHAPE_TYPE_J;
extern const UINT8 SHAPE_TYPE_L;
extern const UINT8 SHAPE_TYPE_O;
extern const UINT8 SHAPE_TYPE_S;
extern const UINT8 SHAPE_TYPE_T;
extern const UINT8 SHAPE_TYPE_Z;

extern const BOOLEAN shape_grids[7][4][4][4];

void rotate();
void new_block(UINT8 type);

void asdf(BOOLEAN (*x)[4][4]);
//BOOLEAN** get_block_grid();
BOOLEAN (*get_block_grid())[4][4];
UINT8 get_tile();
UINT8 get_shape_type();



#endif
