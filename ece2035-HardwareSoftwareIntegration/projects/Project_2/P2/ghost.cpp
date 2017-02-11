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

#include "ghost.h"
//================================================================//
// Private functions
void clean_blk(unsigned int blk_x, unsigned int blk_y)
{
    GRID grid_info = map_get_grid_status(blk_x,blk_y);
    uLCD.filled_rectangle(grid_info.x, grid_info.y, grid_info.x+GRID_SIZE-1, grid_info.y+GRID_SIZE-1, BACKGROUND_COLOR);
}
bool check_blk_occupied(unsigned int blk_x, unsigned int blk_y)
{
    if (map_get_grid_status(blk_x,blk_y).status==GRID_WALL)
        return true;
    else
        return false;
}
void draw_ghost(unsigned int blk_x, unsigned int blk_y, unsigned int ghost_color)
{
    GRID grid = map_get_grid_status(blk_x, blk_y);
    unsigned pos_x = grid.x + GRID_RADIUS;
    unsigned pos_y = grid.y + GRID_RADIUS;
    uLCD.filled_circle(pos_x,pos_y,GRID_RADIUS,ghost_color);
    uLCD.filled_rectangle(pos_x-GRID_RADIUS,pos_y,pos_x+GRID_RADIUS,pos_y+GRID_RADIUS,ghost_color);
    uLCD.filled_circle(pos_x+1,pos_y-1,1,BLACK);
    uLCD.filled_circle(pos_x-1,pos_y-1,1,BLACK);
}
void ghost_move(ghost_t * g, unsigned int new_blk_x, unsigned int new_blk_y)
{
    // clean up ghost at old position
    clean_blk(g->ghost_blk_x, g->ghost_blk_y);
    // clean the block at new position
    clean_blk(new_blk_x, new_blk_y);
    // draw the ghost at new position
    draw_ghost(new_blk_x, new_blk_y, g->ghost_color);

    // recover map component
    map_draw_grid(g->ghost_blk_x, g->ghost_blk_y);

    g->ghost_blk_x = new_blk_x;
    g->ghost_blk_y = new_blk_y;
}
// move ghost up/down/left/right
// return false if failed. true if success
bool ghost_up(ghost_t * g)
{
    if (check_blk_occupied(g->ghost_blk_x, g->ghost_blk_y-1)) return false;
    if (g->ghost_blk_y==0)
        ghost_move(g, g->ghost_blk_x, (NUM_GRID_Y-1));
    else
        ghost_move(g, g->ghost_blk_x, g->ghost_blk_y-1);
    return true;
}
bool ghost_down(ghost_t * g)
{
    if (check_blk_occupied(g->ghost_blk_x, g->ghost_blk_y+1)) return false;

    if (g->ghost_blk_y==(NUM_GRID_Y-1))
        ghost_move(g, g->ghost_blk_x, 0);
    else
        ghost_move(g, g->ghost_blk_x, g->ghost_blk_y+1);
    return true;
}
bool ghost_left(ghost_t * g)
{
    if (check_blk_occupied(g->ghost_blk_x-1, g->ghost_blk_y)) return false;
    if (g->ghost_blk_x==0) 
        ghost_move(g, (NUM_GRID_X-1), g->ghost_blk_y);
    else
        ghost_move(g, g->ghost_blk_x-1, g->ghost_blk_y);
    return true;
}
bool ghost_right(ghost_t * g)
{
    if (check_blk_occupied(g->ghost_blk_x+1, g->ghost_blk_y)) return false;
    if (g->ghost_blk_x==(NUM_GRID_X-1)) 
        ghost_move(g, 0, g->ghost_blk_y);
    else
        ghost_move(g, g->ghost_blk_x+1, g->ghost_blk_y);
    return true;
}

void gen_random_direction(ghost_t * g)
{
    bool blocked[4];
    blocked[0] = check_blk_occupied(g->ghost_blk_x, g->ghost_blk_y-1);    //up
    blocked[1] = check_blk_occupied(g->ghost_blk_x, g->ghost_blk_y+1);    //down
    blocked[2] = check_blk_occupied(g->ghost_blk_x-1, g->ghost_blk_y);    //left
    blocked[3] = check_blk_occupied(g->ghost_blk_x+1, g->ghost_blk_y);    //right
    
    unsigned int npath=0;
    unsigned int outcome=0;
    if (blocked[0]==false) npath++;
    if (blocked[1]==false) npath++;
    if (blocked[2]==false) npath++;
    if (blocked[3]==false) npath++;
    
    unsigned curr=(unsigned int) g->ghost_motion;
    unsigned reverse;
    if ((curr%2)==0)
        reverse = curr+1;
    else 
        reverse = curr-1;
    
    unsigned off=rand();
    for (int i=0;i<4;i++)
    {
        outcome=(off+i)%4;
        // skip the reverse path if possible
        if (npath>1 && reverse==outcome) continue;
        if (blocked[outcome]==false) break;
    }
    g->ghost_motion = (GHOST_MOTION)outcome;
    
    return;
}
//======================================================================================//
// Public functions
void ghost_init(ghost_t * g, unsigned int blk_x, unsigned int blk_y, unsigned int color)
{
    g->ghost_blk_x = blk_x;
    g->ghost_blk_y = blk_y;
    g->ghost_color = color;
    g->ghost_motion = GHOST_UP;
}
void ghost_show(ghost_t *g)
{
    clean_blk(g->ghost_blk_x, g->ghost_blk_y);
    draw_ghost(g->ghost_blk_x, g->ghost_blk_y, g->ghost_color);
}
void ghost_random_walk(ghost_t *g)
{
    gen_random_direction(g);
    
    switch (g->ghost_motion) {
        case GHOST_UP:
            ghost_up(g);
            break;
        case GHOST_DOWN:
            ghost_down(g);
            break;
        case GHOST_LEFT:
            ghost_left(g);
            break;
        case GHOST_RIGHT:
            ghost_right(g);
            break;
        default:
            break;
    }
    return;
}

void ghost_relocate(ghost_t *g, unsigned int blk_x, unsigned int blk_y) {
    ghost_move(g, blk_x, blk_y);
}

//=======================================================================//

