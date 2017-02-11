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
/** @file ghost.h */
#ifndef GHOST_H
#define GHOST_H

#include "mbed.h"
//#include "uLCD_4DGL.h"
#include "ghost.h"
#include "map_public.h"
#include "globals.h"

extern uLCD_4DGL uLCD;

/// The enum defines the motion of the ghost
typedef enum {
    GHOST_UP=0, ///< move up
    GHOST_DOWN, ///< move down
    GHOST_LEFT, ///< move left
    GHOST_RIGHT ///< move right
} GHOST_MOTION;

/// This struct contains the status of a ghost
typedef struct
{
    unsigned int ghost_blk_x;   ///< horizontal position in the grid
    unsigned int ghost_blk_y;   ///< vertical position in the grid
    unsigned int ghost_color;   ///< color of the ghost
    GHOST_MOTION ghost_motion;  ///< the motion of the ghost  
} ghost_t;

/** Initialize the ghost with given position and color
    @param g The pointer to the ghost instance
    @param blk_x The horizontal position in the grid.
    @param blk_y The vertical position in the grid.
    @param color Color of the ghost
*/
void ghost_init(ghost_t * g, unsigned int blk_x, unsigned int blk_y, unsigned int color);

/** Draw the given ghost on the screen
    @brief It should be called once after the initialization of the ghost 
    @param g The pointer to the ghost instance
*/
void ghost_show(ghost_t *g);

/** Move the ghost randomly on the map for one step
    @param g The pointer to the ghost instance
*/
void ghost_random_walk(ghost_t *g);

void ghost_relocate(ghost_t *g, unsigned int blk_x, unsigned int blk_y);

#endif //GHOST_H
