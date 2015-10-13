#include "main.h"

#include "model/model.h"
#include "view/view.h"
#include "../../gb_framework/src/timer.h"

#include <gb/gb.h>
#include <stdio.h>


int main(){

    initialize_timer();

    while(1){
        wait_vbl_done();
        printf("t16: %u, secs: %u\n", time16, time16/16);
        /*wait_vbl_done();*/
        /*wait_vbl_done();*/
        /*wait_vbl_done();*/
        /*wait_vbl_done();*/
        /*clk = clock();*/
        /*printf("%u\n", clk/CLOCKS_PER_SEC);*/
        /*time = TIMA_REG;*/
        /*printf("%u\n", time);*/
        /*counter = DIV_REG;*/
        /*printf("%u\n", counter);*/
    }

//mainloop



}
