#include "controller.h"

#include "../../../gb_framework/src/input.h"
#include "../../../gb_framework/src/timer.h"
#include "../../../gb_framework/src/log.h"
#include "../../../gb_framework/src/sound.h"

/*#include <stdlib.h>*/
/*#include <rand.h>*/
#include <stdio.h>

#include "../model/direction.h"
#include "../model/model.h"
#include "../block/block.h"
#include "../view/view.h"


UINT8 step_time_accum = 0;

void handle_input();
void step();
void move_block_left();
void move_block_right();
void drop_block_down();
void rotate_block();
void toggle_pause();
void new_game();
void block_to_grid();
BOOLEAN does_collide(enum direction dir);
BOOLEAN handle_collision_after_rot();
void spawn_new_block();
void clear_lines();
void clear_line(UINT8 line);
void increase_speed_if_needed();

void play_sound_game_over();
void play_sound_land();
void play_sound_line();
void play_sound_move();
void play_sound_pause();
void play_sound_rotate();


//TODO: remove later
void debug_stuff(){
    UINT8 x, y;
    add_log_line("debug_stuff");
    /*for(x = 0; x < 10; x++){*/
        /*for(y = 0; y < 10; y++)*/
            /*[>set_grid_cell(x, y, TRUE);<]*/
            /*[>grid[x][y] = TRUE;<]*/
    /*}*/
    /*set_grid_cell(0, 0, TRUE);*/
    /*grid_tiles[0][0] = 14;*/

    /*for(x = 0; x < 4; x++){*/
        /*set_grid_cell(x, 17, TRUE);*/
        /*grid_tiles[x][17] = 14;*/

        /*set_grid_cell(9-x, 17, TRUE);*/
        /*grid_tiles[9-x][17] = 14;*/

        /*set_grid_cell(x, 16, TRUE);*/
        /*grid_tiles[x][16] = 14;*/

        /*set_grid_cell(9-x, 16, TRUE);*/
        /*grid_tiles[9-x][16] = 14;*/
    /*}*/

    /*for(y = 17; y >= 10; y--){*/
        /*for(x = 0; x < 4; x++){*/
            /*set_grid_cell(x, y, TRUE);*/
            /*grid_tiles[x][y] = 14;*/
        /*}*/
        /*for(x = 9; x >= 5; x--){*/
            /*set_grid_cell(x, y, TRUE);*/
            /*grid_tiles[x][y] = 14;*/
        /*}*/
    /*}*/





}


void update(UINT16 delta){

    char text[20];


    handle_input();

    /*sprintf(text, "tp: %u", time_played);*/
    /*add_log_line(text);*/


    //TODO: remove later
    /*game_state = GAME_STATE_PLAYING;*/

    if(game_state == GAME_STATE_PLAYING){

        /*frame_counter++;*/
        /*fps = frame_counter / (time16/16);*/

        time_played += delta;
        step_time_accum += delta;
        if(step_time_accum > step_time){
            step_time_accum = 0;
            step();
        }
    }




}

void handle_input(){

    if(is_button_just_pressed(J_SELECT)){
        /*debug_rendering = !debug_rendering;*/

        /*play_sound_game_over();*/
        /*play_sound_land();*/
        /*play_sound_line();*/
        /*play_sound_move();*/
        /*play_sound_pause();*/
        /*play_sound_rotate();*/
    }

    if(is_button_just_pressed(J_START)){

        if(game_state == GAME_STATE_PLAYING ||
                game_state == GAME_STATE_PAUSED){
            toggle_pause();
            intf_changed = TRUE;
        }
        else{
            new_game();
            intf_changed = TRUE;
            ga_changed = TRUE;
        }
    }

    if(game_state == GAME_STATE_PLAYING){

        if(is_button_just_pressed(J_LEFT)){
            move_block_left();
            ga_changed = TRUE;
        }
        if(is_button_just_pressed(J_RIGHT)){
            move_block_right();
            ga_changed = TRUE;
        }
        if(is_button_just_pressed(J_UP) || is_button_just_pressed(J_B)){
            rotate_block();
            ga_changed = TRUE;
        }
        if(is_button_just_pressed(J_DOWN) || is_button_just_pressed(J_A)){
            drop_block_down();
            ga_changed = TRUE;
        }

    }


}


void step(){


    if(!has_current_block)
        return;
    if(!does_collide(DIRECTION_DOWN))
        current_block_pos_y += 1;
    else{
        block_to_grid();
        clear_lines();
        spawn_new_block();
        score += SCORE_PER_BLOCK;

        play_sound_land();
    }

    ga_changed = TRUE;
    intf_changed = TRUE;

}

void move_block_left(){

    if(!has_current_block)
        return;
    if(!does_collide(DIRECTION_LEFT)){
        current_block_pos_x -= 1;
        play_sound_move();
    }

}

void move_block_right(){

    if(!has_current_block)
        return;
    if(!does_collide(DIRECTION_RIGHT)){
        current_block_pos_x += 1;
        play_sound_move();
    }

}

void drop_block_down(){

    if(!has_current_block)
        return;
    while(!does_collide(DIRECTION_DOWN)){
        current_block_pos_y += 1;
    }

    step();
    step_time_accum = 0;

}

void rotate_block(){

    int i;

    rotate();
    if(!handle_collision_after_rot()){
        for(i = 0; i < 3; i++)
            /*rotate_block();*/
            rotate();
    }
    else{
        play_sound_rotate();
    }

}

void toggle_pause(){

    if(game_state == GAME_STATE_PLAYING){
        game_state = GAME_STATE_PAUSED;
        //TODO: stop playing background music
        play_sound_pause();
    }
    else if(game_state == GAME_STATE_PAUSED){
        game_state = GAME_STATE_PLAYING;
        //TODO: continue playing background music
        play_sound_pause();
    }
}

void new_game(){

    /*initrand(1);*/
    /*initrand( ( ((UINT16)DIV_REG) << 8) | DIV_REG);*/

    reset_model();
    step_time_accum = 0;

    spawn_new_block();
    game_state = GAME_STATE_PLAYING;

    ga_changed = TRUE;
    intf_changed = TRUE;
    intf_full_clear_needed = TRUE;

    //TODO: restart background music if already running

    //TODO: remove later
    debug_stuff();

}

void block_to_grid(){

    UINT8 x, y, x2, y2;
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            if((*get_block_grid())[x][y]){
                x2 = current_block_pos_x + x;
                y2 = current_block_pos_y + y;
                /*grid[x2][y2] = TRUE;*/
                set_grid_cell(x2, y2, TRUE);
                /*grid_tiles[x2][y2] = get_tile();*/
                grid_tiles[x2][y2] = get_tile();
            }
        }
    }

    has_current_block = FALSE;

}

BOOLEAN does_collide(enum direction dir){

    //TODO: debug
    /*return FALSE;*/

    INT8 x, y, x2, y2;
    for(y = 0; y < 4; y++){
        for(x = 0; x < 4; x++){
            if((*get_block_grid())[x][y]){

                x2 = current_block_pos_x + x;
                y2 = current_block_pos_y + y;

                if(dir == DIRECTION_DOWN)
                    /*y2--;*/
                    y2++;
                else if(dir == DIRECTION_LEFT)
                    x2--;
                else if(dir == DIRECTION_RIGHT)
                    x2++;

                /*if(y2 >= 0 && x2 >= 0 && x2 <= 9){*/
                if(y2 < 18 && x2 >= 0 && x2 <= 9){
                    /*if(grid[x2][y2])*/
                    if(get_grid_cell(x2, y2))
                        return TRUE;
                }
                else
                    return TRUE;

            }
        }
    }

    return FALSE;

}

BOOLEAN handle_collision_after_rot(){

    INT8 x, y;
    BOOLEAN possible = TRUE;
    BOOLEAN corrected_left = FALSE;
    UINT8 new_pos_x = current_block_pos_x;
    UINT8 new_pos_y = current_block_pos_y;

    //collision left
    for(x = 0; x < 2; x++){

        for(y = 0; y < 4; y++){
            if((*get_block_grid())[x][y]){
                if(get_grid_cell(new_pos_x+x, new_pos_y+y)){
                    new_pos_x++;
                    corrected_left = TRUE;
                    y = -1;
                }
            }
        }

    }

    //collision right
    for(x = 3; x > 1; x--){
        for(y = 0; y < 4; y++){
            if((*get_block_grid())[x][y]){
                if(get_grid_cell(new_pos_x+x, new_pos_y+y)){
                    if(corrected_left){
                        possible = FALSE;
                        x = -1;
                        break;
                    }
                    new_pos_x--;
                    y = -1;
                }
            }
        }
    }

    if(possible){
        current_block_pos_x = new_pos_x;
        current_block_pos_y = new_pos_y;
    }

    return possible;

}

void spawn_new_block(){

    /*initrand( ((UINT16)DIV_REG << 8) | DIV_REG);*/
    /*INT8 r = rand() % 7;*/
    /*char text[20];*/
    volatile INT8 r = DIV_REG;
    r = r % 7;
    if(r < 0)
        r += 7;
    /*TODO: debug only */
    /*r = 0;*/
    /*sprintf(text, "r %d", r);*/
    /*add_log_line(text);*/
    new_block(r);
    set_current_block_pos(3, 0);
    if(does_collide(DIRECTION_NONE)){
        game_state = GAME_STATE_GAMEOVER;
        intf_changed = TRUE;
        //TODO: stop playing background music
        play_sound_game_over();
    }

    has_current_block = TRUE;

}

void clear_lines(){

    INT8 x, y;
    BOOLEAN play_sound = FALSE;

    for(y = 0; y < 18; y++){
        for(x = 0; x < 10; x++){
            /*if(!grid[x][y])*/
            if(!get_grid_cell(x, y))
                break;
            else if(x == 9){
                play_sound = TRUE;
                clear_line(y);
                /*return;*/
                y = -1;
                break;
            }
        }
    }

    if(play_sound)
        play_sound_line();

}

void clear_line(UINT8 line){

    INT8 x, y;
    for(x = 0; x < 10; x++){
        /*grid[x][line] = FALSE;*/
        set_grid_cell(x, line, FALSE);
    }

    for(y = line; y > 0; y--){
        for(x = 0; x < 10; x++){
            /*grid[x][y] = grid[x][y+1];*/
            set_grid_cell(x, y, get_grid_cell(x, y-1));
            grid_tiles[x][y] = grid_tiles[x][y-1];
        }
    }

    for(x = 0; x < 10; x++){
        set_grid_cell(x, 0, FALSE);
        grid_tiles[x][y] = 0;
    }

    increase_score(SCORE_PER_LINE);

    increase_lines_cleared();
    increase_speed_if_needed();

}

void increase_speed_if_needed(){

    if(lines_cleared % 5 == 0){
        speed = lines_cleared/5 + 1;
        step_time -= 1;
        if(step_time < 1)
            step_time = 1;
    }

}

void play_sound_game_over(){

    UINT8 volume = 15;
    UINT16 frequency = 400;
    UINT8 envInc = 0;
    UINT8 envIntensity = 7;
    UINT8 length = 64;
    UINT8 sweepTime = 7;
    UINT8 freqDec = 1;
    UINT8 sweepIntensity = 6;

    sound1_envelope_length_swee(volume, frequency,
            envInc, envIntensity, length,
            sweepTime, freqDec, sweepIntensity);

}

void play_sound_land(){

    UINT8 volume = 10;
    UINT16 frequency = 500;
    UINT8 envInc = 0;
    UINT8 envIntensity = 0;
    UINT8 length = 30;
    UINT8 sweepTime = 0;
    UINT8 freqDec = 0;
    UINT8 sweepIntensity = 0;

    /*sound1_envelope_length_swee(volume, frequency,*/
            /*envInc, envIntensity, length,*/
            /*sweepTime, freqDec, sweepIntensity);*/

    sound2_length(volume, frequency, length);

}

void play_sound_line(){

    UINT8 volume = 15;
    UINT16 frequency = 500;
    UINT8 envInc = 0;
    UINT8 envIntensity = 3;
    UINT8 length = 64;
    UINT8 sweepTime = 7;
    UINT8 freqDec = 0;
    UINT8 sweepIntensity = 6;

    sound1_envelope_length_swee(volume, frequency,
            envInc, envIntensity, length,
            sweepTime, freqDec, sweepIntensity);

}

void play_sound_move(){

    UINT8 volume = 15;
    UINT16 frequency = 180;
    UINT8 envInc = 0;
    UINT8 envIntensity = 0;
    UINT8 length = 15;
    UINT8 sweepTime = 0;
    UINT8 freqDec = 0;
    UINT8 sweepIntensity = 0;

    sound1_envelope_length_swee(volume, frequency,
            envInc, envIntensity, length,
            sweepTime, freqDec, sweepIntensity);

}

void play_sound_pause(){

    UINT8 volume = 15;
    UINT16 frequency = 400;
    UINT8 envInc = 0;
    UINT8 envIntensity = 2;
    UINT8 length = 64;
    UINT8 sweepTime = 0;
    UINT8 freqDec = 0;
    UINT8 sweepIntensity = 0;

    sound1_envelope_length_swee(volume, frequency,
            envInc, envIntensity, length,
            sweepTime, freqDec, sweepIntensity);


}

void play_sound_rotate(){

    UINT8 volume = 15;
    UINT16 frequency = 500;
    UINT8 envInc = 0;
    UINT8 envIntensity = 1;
    UINT8 length = 64;
    UINT8 sweepTime = 4;
    UINT8 freqDec = 0;
    UINT8 sweepIntensity = 7;

    sound1_envelope_length_swee(volume, frequency,
            envInc, envIntensity, length,
            sweepTime, freqDec, sweepIntensity);

}













