#include "controller.h"

#include "../../../gb_framework/src/input.h"

/*#include <stdlib.h>*/
#include <rand.h>

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


void update(UINT16 delta){

    handle_input();

    //TODO: remove later
    /*game_state = GAME_STATE_PLAYING;*/

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

    if(is_button_just_pressed(J_START)){

        if(game_state == GAME_STATE_PLAYING ||
                game_state == GAME_STATE_PAUSED){
            toggle_pause();
        }
        else{
            new_game();
        }
    }

    if(game_state == GAME_STATE_PLAYING){

        if(is_button_just_pressed(J_LEFT)){
            move_block_left();
        }
        if(is_button_just_pressed(J_LEFT)){
            move_block_right();
        }
        if(is_button_just_pressed(J_LEFT) || is_button_just_pressed(J_B)){
            rotate_block();
        }
        if(is_button_just_pressed(J_LEFT) || is_button_just_pressed(J_A)){
            drop_block_down();
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

        //TODO: play sound land
    }


}

void move_block_left(){

    if(!has_current_block)
        return;
    if(!does_collide(DIRECTION_LEFT)){
        current_block_pos_x -= 1;
        //TODO: play sound move
    }

}

void move_block_right(){

    if(!has_current_block)
        return;
    if(!does_collide(DIRECTION_RIGHT)){
        current_block_pos_x += 1;
        //TODO: play sound move
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

    rotate_block();
    if(!handle_collision_after_rot()){
        for(i = 0; i < 3; i++)
            rotate_block();
    }
    else{
        //TODO: play sound rotate
    }

}

void toggle_pause(){

    if(game_state == GAME_STATE_PLAYING){
        game_state = GAME_STATE_PAUSED;
        //TODO: stop playing background music
        //TODO: play sound pause
    }
    else if(game_state == GAME_STATE_PAUSED){
        game_state = GAME_STATE_PLAYING;
        //TODO: continue playing background music
    }
}

void new_game(){

    reset_model();
    step_time_accum = 0;

    spawn_new_block();
    game_state = GAME_STATE_PLAYING;

    //TODO: restart background music if already running

}

void block_to_grid(){

    UINT8 x, y, x2, y2;
    for(x = 0; x < 4; x++){
        for(y = 0; y < 4; y++){
            if(get_grid()[x][y]){
                x2 = current_block_pos_x + x;
                y2 = current_block_pos_y + y;
                grid[x2][y2] = TRUE;
                grid_tiles[x2][y2] = get_tile();
            }
        }
    }

}

BOOLEAN does_collide(enum direction dir){

    UINT8 x, y, x2, y2;
    for(y = 0; y < 4; y++){
        for(x = 0; x < 4; x++){
            if(get_grid()[x][y]){

                x2 = current_block_pos_x + x;
                y2 = current_block_pos_y + y;

                if(dir == DIRECTION_DOWN)
                    y2--;
                else if(dir == DIRECTION_LEFT)
                    x2--;
                else if(dir == DIRECTION_RIGHT)
                    x2++;

                if(y2 >= 0 && x2 >= 0 && x2 <= 9){
                    if(grid[x2][y2])
                        return TRUE;
                }
                else
                    return TRUE;

            }
        }
    }

}

BOOLEAN handle_collision_after_rot(){

    UINT8 x, y;
    BOOLEAN possible = TRUE;
    BOOLEAN corrected_left = FALSE;
    UINT8 new_pos_x = current_block_pos_x;
    UINT8 new_pos_y = current_block_pos_y;

    //collision left
    for(x = 0; x < 2; x++){

        for(y = 0; y < 4; y++){
            if(get_grid()[x][y]){
                if(grid[new_pos_x+x][new_pos_y+y]){
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
            if(get_grid()[x][y]){
                if(grid[new_pos_x+x][new_pos_y+y]){
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

    INT8 r = rand() % 7;
    new_block(r);
    set_current_block_pos(3, 2);
    if(does_collide(DIRECTION_NONE)){
        game_state = GAME_STATE_GAMEOVER;
        //TODO: stop playing background music
        //TODO: play gameOver sound
    }

}

void clear_lines(){

    UINT8 x, y;
    BOOLEAN play_sound = FALSE;

    for(y = 0; y < 18; y++){
        for(x = 0; x < 10; x++){
            if(!grid[x][y])
                break;
            else if(x == 9){
                play_sound = TRUE;
                clear_line(y);
                y = -1;
                break;
            }
        }
    }

    /*if(play_sound)*/
        //TODO: play sound line

}

void clear_line(UINT8 line){

    UINT8 x, y;
    for(x = 0; x < 10; x++){
        grid[x][line] = FALSE;
    }

    for(y = line; y < 19; y++){
        for(x = 0; x < 10; x++){
            grid[x][y] = grid[x][y+1];
            grid_tiles[x][y] = grid_tiles[x][y+1];
        }
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














