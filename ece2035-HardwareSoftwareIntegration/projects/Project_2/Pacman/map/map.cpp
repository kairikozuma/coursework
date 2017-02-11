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

#include "map_private.h"

GRID map[NUM_GRID];
int remaining_cookie = 0;

void map_init(){
    remaining_cookie = 0;
    int i;
    int start_x = (SIZE_X-GRID_SIZE*NUM_GRID_X)/2;
    int start_y = (SIZE_Y-GRID_SIZE*NUM_GRID_Y);
    char temp_wall_type[] = DEFAULT_MAP;

    for(i=0;i<NUM_GRID;i++){
        map[i].x = start_x + IDX2X(i)*GRID_SIZE;
        map[i].y = start_y + IDX2Y(i)*GRID_SIZE;
        switch(temp_wall_type[i]){
            case MAP_ATTRIBUTE_WALL:
                map[i].status = GRID_WALL;
                break;
            case MAP_ATTRIBUTE_COOKIE:
                map[i].status = GRID_COOKIE;
                remaining_cookie++;
                break;
            case MAP_ATTRIBUTE_SUPER_COOKIE:
                map[i].status = GRID_SUPER_COOKIE;
                remaining_cookie++;
                break;
            default:
                map[i].status = GRID_EMPTY;
                break;
        }
    }
    map_draw();
}

void map_draw(){
    int i;
    for(i=0;i<NUM_GRID;i++){
        map_draw_grid(IDX2X(i), IDX2Y(i));
    }
}

void map_draw_grid(unsigned grid_x, unsigned grid_y)
{
    unsigned i=XY2IDX(grid_x,grid_y);
    if(map[i].status==GRID_WALL)
    {
        uLCD.filled_rectangle(map[i].x, map[i].y, map[i].x+GRID_SIZE-1, map[i].y+GRID_SIZE-1, WALL_COLOR);
    }
    else if(map[i].status==GRID_COOKIE)
    {
        uLCD.filled_circle(map[i].x+GRID_RADIUS, map[i].y+GRID_RADIUS, COOKIE_RADIUS, COOKIE_COLOR);
    }
    else if(map[i].status==GRID_SUPER_COOKIE)
    {
        uLCD.filled_circle(map[i].x+GRID_RADIUS, map[i].y+GRID_RADIUS, SUPER_COOKIE_RADIUS, COOKIE_COLOR);
    }
}

GRID map_get_grid_status(int grid_x, int grid_y){
    return map[XY2IDX(grid_x,grid_y)];
}

bool map_eat_cookie(int grid_x, int grid_y){
    int idx=XY2IDX(grid_x,grid_y);
    if(map[idx].status==GRID_COOKIE || map[idx].status==GRID_SUPER_COOKIE){
        map[idx].status = GRID_EMPTY;
        remaining_cookie--;
        return 1;
    }
    return 0;
}

int map_remaining_cookie(void){
    return remaining_cookie;
}

