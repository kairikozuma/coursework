/* Gatech ECE2035 2015 SPRING PAC MAN
 * Copyright (c) 2015 Gatech ECE2035
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#include "mbed.h"
#include "globals.h"
#include "map_public.h"
#include "pacman.h"

// Following code will only draw the Pacman on the screen. You are expected to modify most of the functions here.
// All other necessary functions will be implemented by you. Ex: the movement of Pacman, calculate the score ... etc.

void pacman_init(int grid_x, int grid_y, pacman_t * p){
    p->move = -1;
    p->lastmove = -1;
    p->loc[0] = grid_x;
    p->loc[1] = grid_y;
    p->locn[0] = grid_x;
    p->locn[1] = grid_y;
    p->inv = false;
    map_eat_cookie(grid_x,grid_y); //clear the cookie on the grid.
    pacman_draw(p);
}

void pacman_draw(pacman_t * p){
    
    GRID grid_info = map_get_grid_status(p->locn[0],p->locn[1]);
    // Calculate the actual position of the grid
    int screen_x = grid_info.x + GRID_RADIUS;
    int screen_y = grid_info.y + GRID_RADIUS;
    
    // Direction of movement (or last movement if pacman is not moving)
    int direction;
    
    // Integers that indicate shift for rectangle for mouth
    int shift[4];
    
    //Draw a circle
    uLCD.filled_circle(screen_x, screen_y, GRID_RADIUS, PACMAN_COLOR);
    //Draw a rectangle (the mouth) of Pacman. It faces right by default.
    //You should add code to let the Pacman face to the direction it moves toward.
    if(p->move == -1) {direction = p->lastmove;}
    else {direction = p->move;}
    
    switch(direction) {
        case 0:
            shift[0] = 0;
            shift[1] = -1;
            shift[2] = GRID_RADIUS;
            shift[3] = 1;  
            break;
        case 1:
            shift[0] = 0;;
            shift[1] = -1;
            shift[2] = -GRID_RADIUS;
            shift[3] = 1;  
            break;
        case 2:
            shift[0] = -1;;
            shift[1] = 0;
            shift[2] = 1;
            shift[3] = -GRID_RADIUS;  
            break;
        case 3:
            shift[0] = -1;;
            shift[1] = 0;
            shift[2] = 1;
            shift[3] = GRID_RADIUS;  
            break;
        default:
            shift[0] = 0;;
            shift[1] = -1;
            shift[2] = GRID_RADIUS;
            shift[3] = 1;  
            break;
    }
    uLCD.filled_rectangle(screen_x + shift[0],screen_y + shift[1],screen_x + shift[2],screen_y + shift[3], BACKGROUND_COLOR);
}

void pacman_clear(pacman_t * p){
    
    GRID grid_info = map_get_grid_status(p->loc[0],p->loc[1]);
    //Fill the grid (a rectangle) with BACKGROUND_COLOR to clear the pacman
    uLCD.filled_rectangle(grid_info.x, grid_info.y, grid_info.x+GRID_SIZE-1, grid_info.y+GRID_SIZE-1, BACKGROUND_COLOR);
}

void pacman_deathanimate(pacman_t * p){
    GRID grid_info = map_get_grid_status(p->locn[0],p->locn[1]);
    // Calculate the actual position of the grid
    int screen_x = grid_info.x + GRID_RADIUS;
    int screen_y = grid_info.y + GRID_RADIUS;
    
    // Integers that indicate shift for rectangle for mouth
    int shift[4];

    uLCD.filled_circle(screen_x, screen_y, GRID_RADIUS, PACMAN_COLOR);
    shift[0] = -1;;
    shift[1] = 0;
    shift[2] = 1;
    shift[3] = -GRID_RADIUS;  
    int n;
    for(n = 0;n < 3; n++) {
        uLCD.filled_rectangle(screen_x + shift[0],screen_y + shift[1],screen_x + shift[2],screen_y + shift[3], BACKGROUND_COLOR);
        shift[0]--;
        shift[2]++; 
        wait(0.1);
    }
    for(n = 0;n < 4; n++) {
        uLCD.filled_rectangle(screen_x + shift[0],screen_y + shift[1],screen_x + shift[2],screen_y + shift[3], BACKGROUND_COLOR);
        shift[1]++;
        wait(0.1);
    }
}

void pacman_move(pacman_t * p, Timer timer, data_t * info) {
    int cshift = 0;
    int rshift = 0;
    GRID_STATUS locinf;
    switch(p->move) {
        case -1: // no move
            return;
        case 0: // right
            rshift = 0;
            if(p->loc[0] == 16) {cshift = -16;}
            else {cshift = 1;}
            break;
        case 1: // left
            rshift = 0;
            if(p->loc[0] == 0) {cshift = 16;}
            else {cshift = -1;}
            break;
        case 2: // up
            cshift = 0;
            if(p->loc[1] == 0) {rshift = 15;}
            else {rshift = -1;}
            break;
        case 3: // down
            cshift = 0;
            if(p->loc[1] == 15) {rshift = -15;}
            else {rshift = 1;}
            break;
        default:
            break;
        }
    locinf = (map_get_grid_status(p->loc[0]+cshift, p->loc[1]+rshift)).status;
    switch(locinf) {
        case GRID_WALL:
            break; 
        case GRID_SUPER_COOKIE:
            p->inv = true;
            info->score = info->score + 4;
            info->invtime = info->invstart - info->level;
            info->invstart_time = timer.read_ms();
        case GRID_COOKIE:
            info->score++;
        case GRID_EMPTY:
            p->locn[0] = p->loc[0] + cshift;
            p->locn[1] = p->loc[1] + rshift;
    }
    return;
}