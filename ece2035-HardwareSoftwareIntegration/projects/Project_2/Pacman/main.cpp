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
/** @file main.cpp */
// Include header files for platform
#include "mbed.h"
#include "wave_player.h"
#include "SDFileSystem.h"

// Include header files for pacman project
#include "globals.h"
#include "map_public.h"
#include "pacman.h"
#include "ghost.h"
#include "MMA8452.h"

// Platform initialization
DigitalIn left_pb(p21);  // push bottem
DigitalIn right_pb(p22); // push bottem
DigitalIn up_pb(p23);    // push bottem
DigitalIn down_pb(p24);  // push bottem
uLCD_4DGL uLCD(p9,p10,p11); // LCD (serial tx, serial rx, reset pin;)
Serial pc(USBTX,USBRX);     // used by Accelerometer
MMA8452 acc(p28, p27, 100000); // Accelerometer
AnalogOut DACout(p18);      // speaker
wave_player waver(&DACout); // wav player
SDFileSystem sd(p5, p6, p7, p8, "sd"); // SD card and filesystem (mosi, miso, sck, cs)

// Example of the declaration of your implementation
void playSound(char * wav);
void drawLife(int numLife);
void pacman_move(int move, int * loc, int * locn, Timer timer);

// Declare global variables
int score = 0;

// Time variable
int timeint = 500;

// Level variable
int level = 1;

// Lives variable
int life = 3;

// Invincibility time
int invtime = 0;
int invstart = 6;
int invstart_time = 0;
int invint = 800;

// Variables for ghosts
int x1, x2, x3, x4;
int y1, y2, y3, y4;

bool play = true;
bool lose = false;

/** Main() is where you start your implementation
    @brief The hints of implementation are in the comments. <br>
    @brief You are expected to implement your code in main.cpp and pacman.cpp. But you could modify any code if you want to make the game work better.
*/
int main()
{   
    // Initialize the buttons        
    left_pb.mode(PullUp);  // The variable left_pb will be zero when the pushbutton for moving the player left is pressed    
    right_pb.mode(PullUp); // The variable rightt_pb will be zero when the pushbutton for moving the player right is pressed        
    up_pb.mode(PullUp);    //the variable fire_pb will be zero when the pushbutton for firing a missile is pressed
    down_pb.mode(PullUp);  //the variable fire_pb will be zero when the pushbutton for firing a missile is pressed
    
    
    // Start Menu
    uLCD.text_string("PACMAN", 1, 4, FONT_7X8, WHITE);
    while (left_pb != 0 || right_pb != 0) {
    
    }

    while(!lose) {
    
    // Initialize the timer
    /// [Example of time control implementation]
        /// Here is a rough example to implement the timer control <br><br>
    int tick, pre_tick;
    double x,y,z;
    srand (time(NULL));
    Timer timer;
    timer.start();
    tick = timer.read_ms();
    pre_tick = tick;
    
    /// [Example of the game control implementation]
        /// Here is the example to initialize the game <br><br>
    uLCD.cls();
    map_init();
    
    pacman_init(8,9); // Center of the map
    
    ghost_t ghost_blinky;
    ghost_t ghost_pinky;
    ghost_t ghost_inky;
    ghost_t ghost_clyde;

    // Add ghosts
    ghost_init(&ghost_blinky,8,7,RED);
    ghost_show(&ghost_blinky);

    ghost_init(&ghost_pinky,7,7,PINK);
    ghost_show(&ghost_pinky);
    
    ghost_init(&ghost_inky,9,7,CYAN);
    ghost_show(&ghost_inky);
    
    ghost_init(&ghost_clyde,8,6,ORANGE);
    ghost_show(&ghost_clyde);
    
    // [Demo of play sound file]
    playSound("/sd/wavfiles/START.wav");
    
    // Variables for Pacman location
    int loc[2];;
    int locn[2];
    int move = -1;
    int lastmove = -1;

    // Location variables
    loc[0] = 8;
    loc[1] = 9;
    locn[0] = 8;
    locn[1] = 9;
    
    // End condition variables
    bool lrd[4];
    bool udd[4];
    bool collide;
    bool same;
    
    drawLife(life);
    uLCD.locate(6,1);
    uLCD.printf("Level %d", level);
    play = true;
    
    // 1. Begin the game loop
    while(play){
        tick = timer.read_ms(); // Read current time
        
        /// 2. Implement the code to get user input and update the Pacman
            /// -[Hint] Implement the code to move Pacman. You could use either push-button or accelerometer. <br>       

        // [Some hints for user-input detection]
        acc.readXYZGravity(&x,&y,&z); //read accelerometer
        //uLCD.locate(0,1);
        //uLCD.printf("sensor x%4.1f y%4.1f\n",x,y); //You could remove this code if you already make the accelerometer work.
            /// -[Hint] Here is a simple way to utilize the readings of accelerometer:
            ///         If x is larger than certain value (ex:0.3), then make the Pacman move right.
            ///         If x<-0.3, then make it move left. <br>
        /*
        if (abs(x) > abs(y))
            if (x > 0.3) move = 3;   
            else if (x < -0.3) move = 2;
            else move = -1;
        else
            if (y > 0.3) move = 1;
            else if (y < -0.3) move = 0;
            else move = -1;
        */
        if(right_pb == 0 && left_pb != 0){move = 0;}
        else if(left_pb == 0 && right_pb != 0){move = 1;}
        else if(up_pb == 0 && down_pb != 0){move = 2;} 
        else if(down_pb == 0 && up_pb != 0){move = 3;}
        else move = -1;
        
        // Store last move
        if(move != -1) {lastmove = move;}
        
        // Special condition to force advance to next level (press all buttons)
        if(right_pb == 0 && left_pb == 0 && up_pb == 0 && down_pb == 0) {
            play = false;
            level++;
            life++;
        }
    
        if(invtime != 0 && ((tick - invstart_time) > invint)) {
                invtime--;
                invstart_time = tick;
            }
        
        if((tick-pre_tick)> timeint){ // Time step control
            pre_tick = tick; // update the previous tick
            
            
        /// 3. Update the Pacman on the screen
            /// -[Hint] You could update the position of Pacman (draw it on the screen) here based on the user-input at step 2. <br>
            
            pacman_move(move, loc, locn, timer);
            
            pacman_clear(loc[0],loc[1]);
            wait(0.1);
            pacman_draw(locn[0],locn[1], move, lastmove);
            map_eat_cookie(locn[0],locn[1]);
            
            // [demo of the ghost movement]
            ghost_random_walk(&ghost_blinky);
            ghost_random_walk(&ghost_pinky);
            ghost_random_walk(&ghost_inky);
            ghost_random_walk(&ghost_clyde);
            
            
            uLCD.locate(0,0);
            uLCD.printf("%d %d", score, map_remaining_cookie());
            uLCD.locate(0,1);
            uLCD.printf("%d", invtime);
            
            lrd[0] = (y1 == locn[1] && ((x1 == loc[0] && ghost_blinky.ghost_motion == GHOST_LEFT) || (x1 == loc[0] && ghost_blinky.ghost_motion == GHOST_RIGHT)));
            udd[0] = (x1 == locn[0] && ((y1 == loc[1] && ghost_blinky.ghost_motion == GHOST_UP)   || (y1 == loc[1] && ghost_blinky.ghost_motion == GHOST_DOWN))); 
            lrd[1] = (y2 == locn[1] && ((x2 == loc[0] && ghost_blinky.ghost_motion == GHOST_LEFT) || (x2 == loc[0] && ghost_blinky.ghost_motion == GHOST_RIGHT)));
            udd[1] = (x2 == locn[0] && ((y2 == loc[1] && ghost_blinky.ghost_motion == GHOST_UP)   || (y2 == loc[1] && ghost_blinky.ghost_motion == GHOST_DOWN))); 
            lrd[2] = (y3 == locn[1] && ((x3 == loc[0] && ghost_blinky.ghost_motion == GHOST_LEFT) || (x3 == loc[0] && ghost_blinky.ghost_motion == GHOST_RIGHT)));
            udd[2] = (x3 == locn[0] && ((y3 == loc[1] && ghost_blinky.ghost_motion == GHOST_UP)   || (y3 == loc[1] && ghost_blinky.ghost_motion == GHOST_DOWN))); 
            lrd[3] = (y4 == locn[1] && ((x4 == loc[0] && ghost_blinky.ghost_motion == GHOST_LEFT) || (x4 == loc[0] && ghost_blinky.ghost_motion == GHOST_RIGHT)));
            udd[3] = (x4 == locn[0] && ((y4 == loc[1] && ghost_blinky.ghost_motion == GHOST_UP)   || (y4 == loc[1] && ghost_blinky.ghost_motion == GHOST_DOWN))); 
            
            loc[0] = locn[0];
            loc[1] = locn[1];
        }
        
        /// 4. Implement the code to check the end of game.
            /// -[Hint] Check whether the ghost catch the Pacman. Make sure you could always detect that the ghost and Pacman meet on the screen.
            /// One tricky scenario is that: Pacman is at grid (3,3) and is moving to (3,4), while the ghost is at grid (3,4) and is moving to (3,3).
            /// Either at time t or t+1, you will not see the Pacman and the ghost on the same grid.
            /// No matter what, the Pacman should be caught by ghost in above scenario.
            /// Make sure your code could deal with above scenario correctly.
            /// -[Hint] Check whether Pacman win the game <br>
        
        x1 = ghost_blinky.ghost_blk_x;
        x2 = ghost_pinky.ghost_blk_x;
        x3 = ghost_inky.ghost_blk_x;
        x4 = ghost_clyde.ghost_blk_x;
        
        y1 = ghost_blinky.ghost_blk_y;
        y2 = ghost_pinky.ghost_blk_y;
        y3 = ghost_inky.ghost_blk_y;
        y4 = ghost_clyde.ghost_blk_y;
        
        // Check if pacman should die
        // Pacman should die if ghost passes pacman or pacman and ghost land on the same space
        
        collide = (lrd[0] || lrd[1] || lrd[2] || lrd[3] || udd[0] || udd [1] || udd[2] || udd[3]);
        same = ((x1 == loc[0] && y1 == loc[1]) || (x2 == loc[0] && y2 == loc[1]) || (x3 == loc[0] && y3 == loc[1]) || (x4 == loc[0] && y4 == loc[1]));
        
        if(same || collide) {
            life--;
            drawLife(life);
           
            if (life == 0) {
            play = false;
            lose = true;
            }
            else{
                pacman_clear(loc[0], loc[1]);
                loc[0] = 8;
                loc[1] = 9;
                pacman_draw(loc[0], loc[1], move, lastmove);
                ghost_relocate(&ghost_blinky, 8, 7);
                ghost_relocate(&ghost_pinky, 7, 7);
                ghost_relocate(&ghost_inky, 9, 7);
                ghost_relocate(&ghost_clyde, 8, 6);}
        } 
        
        // Advance to Next Level after all the cookies eaten
        if (map_remaining_cookie() == 0) {
            play = false;
            level++;
            life++;
        }
    }
    }
    
    // Dialog at the end    
    uLCD.filled_rectangle(19, 50, 108, 94, 0x000000);
    uLCD.rectangle(19, 50, 108, 94, 0xffffff);
    uLCD.locate(4,7);
    uLCD.printf("Game  Over");
    uLCD.locate(4,9);
    uLCD.printf("Score: %d", score);
}


// Example of implementation of your functions
void playSound(char * wav)
{
    // open wav file
    FILE *wave_file;
    wave_file=fopen(wav,"r");

    if(wave_file == NULL){
        uLCD.locate(9,0);
        uLCD.printf("ERROR_SD");
        return;
    }
    
    // play wav file
    waver.play(wave_file);

    // close wav file
    fclose(wave_file);
}

void drawLife(int numLife)
{
    int xloc = 120;
    int yloc = 4;
    for (int x = 0; x < numLife; x++) {
        uLCD.filled_circle(xloc - x*8, yloc, GRID_RADIUS, PACMAN_COLOR);
        uLCD.filled_rectangle(xloc - x*8, yloc-1,xloc+4 - x*8,yloc+1, BACKGROUND_COLOR);
    }
    for (int x = numLife; x <= 12; x++) {
        uLCD.filled_circle(xloc - x*8, yloc, GRID_RADIUS, BACKGROUND_COLOR);
    }
}

void pacman_move(int move, int *loc, int *locn, Timer timer) {
    int cshift = 0;
    int rshift = 0;
    GRID_STATUS locinf;
    switch(move) {
        case -1: // no move
            return;
        case 0: // right
            rshift = 0;
            if(loc[0] == 16) {cshift = -16;}
            else {cshift = 1;}
            break;
        case 1: // left
            rshift = 0;
            if(loc[0] == 0) {cshift = 16;}
            else {cshift = -1;}
            break;
        case 2: // up
            cshift = 0;
            if(loc[1] == 0) {rshift = 15;}
            else {rshift = -1;}
            break;
        case 3: // down
            cshift = 0;
            if(loc[1] == 15) {rshift = -15;}
            else {rshift = 1;}
            break;
        default:
            break;
        }
    locinf = (map_get_grid_status(loc[0]+cshift, loc[1]+rshift)).status;
    switch(locinf) {
        case GRID_WALL:
            break; 
        case GRID_SUPER_COOKIE:
            score = score + 4;
            invtime = invstart - level;
            invstart_time = timer.read_ms();
        case GRID_COOKIE:
            score++;
        case GRID_EMPTY:
            locn[0] = loc[0] + cshift;
            locn[1] = loc[1] + rshift;
    }
    return;
}