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

void pacman_init(int grid_x, int grid_y){
    int move = -1;
    int lastmove = -1;
    map_eat_cookie(grid_x,grid_y); //clear the cookie on the grid.
    pacman_draw(grid_x,grid_y, move, lastmove);

}

void pacman_draw(int grid_x, int grid_y, int move, int lastmove){
    
    GRID grid_info = map_get_grid_status(grid_x,grid_y);
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
    if(move == -1) {direction = lastmove;}
    else {direction = move;}
    
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

void pacman_clear(int grid_x, int grid_y){
    
    GRID grid_info = map_get_grid_status(grid_x,grid_y);
    //Fill the grid (a rectangle) with BACKGROUND_COLOR to clear the pacman
    uLCD.filled_rectangle(grid_info.x, grid_info.y, grid_info.x+GRID_SIZE-1, grid_info.y+GRID_SIZE-1, BACKGROUND_COLOR);
}

void pacman_death_animate(int grid_x, int grid_y){}
