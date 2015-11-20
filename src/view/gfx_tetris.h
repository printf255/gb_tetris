#ifndef GFX_TETRIS_H
#define GFX_TETRIS_H

#include <gb/gb.h>

void set_ga(UINT8 x, UINT8 y, UINT8 value);
void set_ga_mul(UINT8 x, UINT8 y, UINT8 w, UINT8 h, UINT8 value);
void write_ga();

void set_intf(UINT8 x, UINT8 y, UINT8 value);
void set_intf_mul(UINT8 x, UINT8 y, UINT8 w, UINT8 h, UINT8 value);
void write_intf();

void render_text_intf(UINT8 x, UINT8 y, char *text);



#endif
