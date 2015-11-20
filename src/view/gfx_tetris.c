#include "gfx_tetris.h"
#include <ctype.h>

unsigned char ga[18][10];
unsigned char intf[18][10];

void render_char_intf(UINT8 x, UINT8 y, char c);


void set_ga(UINT8 x, UINT8 y, UINT8 value){
    ga[y][x] = value;
}

void set_ga_mul(UINT8 x, UINT8 y, UINT8 w, UINT8 h, UINT8 value){
    UINT8 i, j;
    for(i = x; i < x+w; i++){
        for(j = y; j < y+h; j++){
            ga[j][i] = value;
        }
    }
}

void write_ga(){
    set_bkg_tiles(0, 0, 10, 18, ga);
}

void set_intf(UINT8 x, UINT8 y, UINT8 value){
    intf[y][x] = value;
}

void set_intf_mul(UINT8 x, UINT8 y, UINT8 w, UINT8 h, UINT8 value){
    UINT8 i, j;
    for(i = x; i < x+w; i++){
        for(j = y; j < y+h; j++){
            intf[j][i] = value;
        }
    }
}

void write_intf(){
    set_bkg_tiles(10, 0, 10, 18, intf);
}

void render_char_intf(UINT8 x, UINT8 y, char c){
    UINT8 tileNum = 0;
    UINT8 ascii = (UINT8) c;

    if(isdigit(c)){
        tileNum = ascii - 46;
    }
    else{
        c = tolower(c);
        ascii = (UINT8) c;
        tileNum = ascii - 85;
    }

    intf[y][x] = tileNum;
}

void render_text_intf(UINT8 x, UINT8 y, char *text){
    INT8 i = 0;
    while(1){
        if(text[i] == 0)
            break;
        render_char_intf(x, y, text[i]);
        x++;
        i++;
    }
}












