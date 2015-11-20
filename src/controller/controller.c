#include "controller.h"

#include "../../../gb_framework/src/input.h"
#include "../../../gb_framework/src/timer.h"
#include "../../../gb_framework/src/sound.h"

#include "../model/direction.h"
#include "../model/model.h"
#include "../block/block.h"

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


void update(UINT16 delta){

    handle_input();

    if(game_state == GAME_STATE_PLAYING){

        time_played += delta;
        step_time_accum += delta;
        if(step_time_accum > step_time){
            step_time_accum = 0;
            step();
        }

    }

}

void handle_input(){

    if(is_button_just_pressed(J_START) ||
            is_button_just_pressed(J_START)){

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
            rotate();
    }
    else{
        play_sound_rotate();
    }

}

void toggle_pause(){

    if(game_state == GAME_STATE_PLAYING){
        game_state = GAME_STATE_PAUSED;
        play_sound_pause();
    }
    else if(game_state == GAME_STATE_PAUSED){
        game_state = GAME_STATE_PLAYING;
        play_sound_pause();
    }

}

void new_game(){

    reset_model();
    step_time_accum = 0;

    spawn_new_block();
    game_state = GAME_STATE_PLAYING;

    ga_changed = TRUE;
    intf_changed = TRUE;
    intf_full_clear_needed = TRUE;

}

void block_to_grid(){

    UINT8 x, y, x2, y2;
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            if((*get_block_grid())[x][y]){
                x2 = current_block_pos_x + x;
                y2 = current_block_pos_y + y;
                set_grid_cell(x2, y2, TRUE);
                grid_tiles[x2][y2] = get_tile();
            }
        }
    }

    has_current_block = FALSE;

}

BOOLEAN does_collide(enum direction dir){

    INT8 x, y, x2, y2;
    for(y = 0; y < 4; y++){
        for(x = 0; x < 4; x++){
            if((*get_block_grid())[x][y]){

                x2 = current_block_pos_x + x;
                y2 = current_block_pos_y + y;

                if(dir == DIRECTION_DOWN)
                    y2++;
                else if(dir == DIRECTION_LEFT)
                    x2--;
                else if(dir == DIRECTION_RIGHT)
                    x2++;

                if(y2 < 18 && x2 >= 0 && x2 <= 9){
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

    volatile INT8 r = DIV_REG;
    r = r % 7;
    if(r < 0)
        r += 7;
    new_block(r);
    set_current_block_pos(3, 0);
    if(does_collide(DIRECTION_NONE)){
        game_state = GAME_STATE_GAMEOVER;
        intf_changed = TRUE;
        play_sound_game_over();
    }

    has_current_block = TRUE;

}

void clear_lines(){

    INT8 x, y;
    BOOLEAN play_sound = FALSE;

    for(y = 0; y < 18; y++){
        for(x = 0; x < 10; x++){
            if(!get_grid_cell(x, y))
                break;
            else if(x == 9){
                play_sound = TRUE;
                clear_line(y);
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
        set_grid_cell(x, line, FALSE);
    }

    for(y = line; y > 0; y--){
        for(x = 0; x < 10; x++){
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













