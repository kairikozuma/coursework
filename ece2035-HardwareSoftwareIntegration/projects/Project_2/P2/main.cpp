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
#include "data.h"
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
PwmOut led1(LED1);
PwmOut led2(LED2);
PwmOut led3(LED3);
PwmOut led4(LED4);

// Example of the declaration of your implementation
void playSound(char * wav);
void drawLife(int numLife);
bool game_end(data_t * info);
int start_menu();
int comp_data(const void *a,const void *b);
void high_score(data_t * info, int scoreboard[], int k);

/** Main() is where you start your implementation
    @brief The hints of implementation are in the comments. <br>
    @brief You are expected to implement your code in main.cpp and pacman.cpp. But you could modify any code if you want to make the game work better.
*/
int main()
{   
    int lifeorigin;
    int difficulty = 0;
    int control = 0;
    int scoreboard[7];
    int k = 0;
    
    // Data variable
    data_t info;
    // Initialize the buttons        
    left_pb.mode(PullUp);  // The variable left_pb will be zero when the pushbutton for moving the player left is pressed    
    right_pb.mode(PullUp); // The variable rightt_pb will be zero when the pushbutton for moving the player right is pressed        
    up_pb.mode(PullUp);    //the variable fire_pb will be zero when the pushbutton for firing a missile is pressed
    down_pb.mode(PullUp);  //the variable fire_pb will be zero when the pushbutton for firing a missile is pressed
    
    
    // Start Menu
    int diffmod = start_menu();
    difficulty = diffmod/10;
    control = diffmod % 10;
    
    data_init(&info, difficulty, control);
    lifeorigin = info.life;
    while(info.noquit) {
    while(!info.lose) {
    
    // Initialize the timer
    int tick, pre_tick;
    double x,y,z;
    srand (time(NULL));
    Timer timer;
    timer.start();
    tick = timer.read_ms();
    pre_tick = tick;
    
    // Initialize map
    uLCD.cls();
    map_init(info.level, info.newlevel);
    
    pacman_t pacman;
    pacman_init(8,9, &pacman); // Center of the map
    
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
    
    // Play Start Sound
    if(info.level == 1) {playSound("/sd/wavfiles/START.wav");}
    else {playSound("/sd/wavfiles/START.wav");}
    
    // End condition variables
    bool s1[4];
    bool s2[4];
    int xl[4];
    int yl[4];
    
    
    
    drawLife(info.life);
    uLCD.locate(6,1);
    uLCD.printf("Level %d", info.level);
    uLCD.locate(14,1);
    switch(info.diff) {
        case 0:
        uLCD.printf("Easy");
        break;
        case 1:
        uLCD.printf("Norm");
        break;
        case 2:
        uLCD.printf("Hard");
        break;
    }
    info.play = true;
    
    int lc[2];
    int lcn[2];
    
    // 1. Begin the game loop
    while(info.play){
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
        
        if(info.control == 0) {
            if (abs(x) > abs(y))
                if (x > 0.3) pacman.move = 2;   
                else if (x < -0.3) pacman.move = 3;
                else pacman.move = -1;
            else
                if (y > 0.3) pacman.move = 1;
                else if (y < -0.3) pacman.move = 0;
                else pacman.move = -1;
        }
        else{
            if(right_pb == 0 && left_pb != 0){pacman.move = 0;}
            else if(left_pb == 0 && right_pb != 0){pacman.move = 1;}
            else if(up_pb == 0 && down_pb != 0){pacman.move = 2;} 
            else if(down_pb == 0 && up_pb != 0){pacman.move = 3;}
            else pacman.move = -1;
        }
        
        // Store last move
        if(pacman.move != -1) {pacman.lastmove = pacman.move;}
        
        // Special condition to force advance to next level (press up and right)
        if(right_pb == 0 && up_pb == 0 ){
            info.play = false;
            info.level++;
            info.life++;
        }
        
        // Special condition to pause (press down and left)
        if(down_pb == 0 && left_pb == 0 ){
            uLCD.locate(6,0);
            uLCD.color(YELLOW);
            uLCD.printf("PAUSE");
            wait(1.5);
            while(down_pb != 0 || left_pb != 0 ){}
            uLCD.color(BLACK);
            uLCD.locate(6,0);
            uLCD.printf("PAUSE");
            uLCD.color(WHITE);
        }
        
        
        if(info.invtime != 0) {
            led1 = 1;
            led2 = 1;
            led3 = 1;
            led4 = 1;
            wait(0.02*info.invtime);
            led1 = 0;
            led2 = 0;
            led3 = 0;
            led4 = 0;
            wait(0.02*info.invtime);
        }
        else {
            led1 = 0;
            led2 = 0;
            led3 = 0;
            led4 = 0;
            pacman.inv = false;
        }
        if(info.invtime != 0 && ((tick - info.invstart_time) > info.invint)) {
            info.invtime--;
            info.invstart_time = tick;
            pacman.inv = true;
        }
        
        if((tick-pre_tick)> info.timeint){ // Time step control
            pre_tick = tick; // update the previous tick
            
            
        /// 3. Update the Pacman on the screen
            /// -[Hint] You could update the position of Pacman (draw it on the screen) here based on the user-input at step 2. <br>
            
            pacman_move(&pacman, timer, &info);
            pacman_clear(&pacman);
            wait(0.1);
            pacman_draw(&pacman);
            lc[0] = pacman.loc[0];
            lc[1] = pacman.loc[1];
            lcn[0] = pacman.loc[0];
            lcn[1] = pacman.loc[1];
            map_eat_cookie(pacman.locn[0], pacman.locn[1]);
            
            uLCD.locate(0,0);
            uLCD.printf("%d", info.score);
            uLCD.locate(0,1);
            uLCD.printf("Score");
            //uLCD.printf("%d %d %d", info.score, info.invtime, pacman.inv);
            //uLCD.printf("%d", map_remaining_cookie());
            //uLCD.locate(0,1);
            //uLCD.printf("%d", info.invtime);
            
            xl[0] = ghost_blinky.ghost_blk_x;
            yl[0] = ghost_blinky.ghost_blk_y;
            xl[1] = ghost_pinky.ghost_blk_x;
            yl[1] = ghost_pinky.ghost_blk_y;
            xl[2] = ghost_inky.ghost_blk_x;
            yl[2] = ghost_inky.ghost_blk_y;
            xl[3] = ghost_clyde.ghost_blk_x;
            yl[3] = ghost_clyde.ghost_blk_y;
            
            s1[0] = (xl[0] == lcn[0] && yl[0] == lcn[1]);
            s1[1] = (xl[1] == lcn[0] && yl[1] == lcn[1]);
            s1[2] = (xl[2] == lcn[0] && yl[2] == lcn[1]);
            s1[3] = (xl[3] == lcn[0] && yl[3] == lcn[1]);
            
            if(pacman.inv || !(s1[0] || s1[1] || s1[2] || s1[3])) { // Pacman is invincible or alive
                // Move ghost
                ghost_random_walk(&ghost_blinky);
                ghost_random_walk(&ghost_pinky);
                ghost_random_walk(&ghost_inky);
                ghost_random_walk(&ghost_clyde);
                
                xl[0] = ghost_blinky.ghost_blk_x;
                yl[0] = ghost_blinky.ghost_blk_y;
                xl[1] = ghost_pinky.ghost_blk_x;
                yl[1] = ghost_pinky.ghost_blk_y;
                xl[2] = ghost_inky.ghost_blk_x;
                yl[2] = ghost_inky.ghost_blk_y;
                xl[3] = ghost_clyde.ghost_blk_x;
                yl[3] = ghost_clyde.ghost_blk_y;
                
                s2[0] = (xl[0] == lcn[0] && yl[0] == lcn[1]);
                s2[1] = (xl[1] == lcn[0] && yl[1] == lcn[1]);
                s2[2] = (xl[2] == lcn[0] && yl[2] == lcn[1]);
                s2[3] = (xl[3] == lcn[0] && yl[3] == lcn[1]);
            }
            pacman.loc[0] = pacman.locn[0];
            pacman.loc[1] = pacman.locn[1];
        }
        
        /// 4. Implement the code to check the end of game.
            /// -[Hint] Check whether the ghost catch the Pacman. Make sure you could always detect that the ghost and Pacman meet on the screen.
            /// One tricky scenario is that: Pacman is at grid (3,3) and is moving to (3,4), while the ghost is at grid (3,4) and is moving to (3,3).
            /// Either at time t or t+1, you will not see the Pacman and the ghost on the same grid.
            /// No matter what, the Pacman should be caught by ghost in above scenario.
            /// Make sure your code could deal with above scenario correctly.
            /// -[Hint] Check whether Pacman win the game <br>
        
        if(pacman.inv && (s1[0] || s1[1] || s1[2] || s1[3] || s2[0] || s2[1] || s2[2] || s2[3])) {
            info.score = info.score + 10;
            playSound("/sd/wavfiles/EATGHOST.wav");
            if(s1[0] || s2[0]) {
                ghost_relocate(&ghost_blinky, 8, 7);  
            }
            if(s1[1] || s2[1]) {
                ghost_relocate(&ghost_pinky, 8, 7);
            }
            if(s1[2] || s2[2]) {
                ghost_relocate(&ghost_inky, 8, 7);
            }
            if(s1[3] || s2[3]) {
                ghost_relocate(&ghost_clyde, 8, 7);
            }
        }
        else if(!pacman.inv && (s1[0] || s1[1] || s1[2] || s1[3] || s2[0] || s2[1] || s2[2] || s2[3])) {
            info.life--;
            drawLife(info.life);
            
            playSound("/sd/wavfiles/DEATH.wav");
            pacman_deathanimate(&pacman);
            
            if (info.life == 0) {
            info.play = false;
            info.lose = true;
            }
            else{
                pacman_clear(&pacman);
                pacman.loc[0] = 8;
                pacman.loc[1] = 9;
                pacman.locn[0] = 8;
                pacman.locn[1] = 9;
                pacman_draw(&pacman);
                ghost_relocate(&ghost_blinky, 8, 7);
                ghost_relocate(&ghost_pinky, 7, 7);
                ghost_relocate(&ghost_inky, 9, 7);
                ghost_relocate(&ghost_clyde, 8, 6);}
        } 
        
        // Advance to Next Level after all the cookies eaten
        if (map_remaining_cookie() == 0) {
            info.play = false;
            info.level++;
            info.life++;
        }
    }
    }
    info.noquit = game_end(&info);
    scoreboard[k] = info.score;
        if(info.noquit) {
        info.lose = false;
        info.play = true;
        info.level = 1;
        info.life = lifeorigin;
        info.score = 0;
        //if(info.trial == 0) {info.trial = 1;}
        //else {info.trial++;}
        k++;
    }
    else {high_score(&info, scoreboard, k);}
    }
}

int comp_data(const void *a, const void *b) {
    int *x = (int *) a;
    int *y = (int *) b;
    return *x - *y;
}

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

void drawLife(int numLife) //Draw number of lives in pacman icons in upper right corner
{
    int xloc = 120;
    int yloc = 4;
    for (int x = 0; x < numLife; x++) {
        uLCD.filled_circle(xloc - x*8, yloc, GRID_RADIUS, PACMAN_COLOR);
        uLCD.filled_rectangle(xloc - x*8, yloc-1,xloc+4 - x*8,yloc+1, BACKGROUND_COLOR);
    }
    for (int x = numLife; x <= 10; x++) {
        uLCD.filled_circle(xloc - x*8, yloc, GRID_RADIUS, BACKGROUND_COLOR);
    }
}

void high_score(data_t * info, int scoreboard[], int k) {
    qsort (scoreboard, sizeof(scoreboard)/sizeof(*scoreboard), sizeof(*scoreboard), comp_data);
    
    uLCD.filled_rectangle(19, 20, 108, 114, BLACK);
    uLCD.rectangle(19, 20, 108, 114, WHITE);
    uLCD.locate(4,4);
    uLCD.printf("Scores");
    
    int n;
    for (n = 0; n <= k; n++) {
        uLCD.locate(4,6 + n);
        uLCD.printf("%d", scoreboard[n]);
    }
    
        //uLCD.printf("%d", info->trial);
}
bool game_end(data_t * info) //Display Game Over Info
{
    int locchoice = 0;
    uLCD.filled_rectangle(19, 20, 108, 114, BLACK);
    uLCD.rectangle(19, 20, 108, 114, WHITE);
    uLCD.locate(4,4);
    uLCD.printf("Game  Over");
    uLCD.locate(4,6);
    uLCD.printf("Score: %d", info->score);
    
    uLCD.color(WHITE);
    uLCD.locate(4,8);
    uLCD.printf("PLAY AGAIN?");
    uLCD.locate(4,10);  
    uLCD.printf("YES");
    uLCD.locate(4,12);  
    uLCD.printf("NO");
    uLCD.filled_rectangle(22, 81, 26, 85, WHITE);
    while(right_pb != 0) {
        if(up_pb == 0 && down_pb != 0) { //Move selection box up
            switch(locchoice) {
            case 0:
                uLCD.filled_rectangle(22, 81, 26, 85, BLACK);
                uLCD.filled_rectangle(22, 97, 26, 102, WHITE);
                locchoice = 1;
                break;
            case 1:
                uLCD.filled_rectangle(22, 97, 26, 102, BLACK);
                uLCD.filled_rectangle(22, 81, 26, 85, WHITE);
                locchoice = 0;
                break;
            }
            wait(0.2);
        }
        else if(down_pb == 0 && up_pb != 0) { //Move selection box down
            switch(locchoice) {
            case 0:
                uLCD.filled_rectangle(22, 81, 26, 85, BLACK);
                uLCD.filled_rectangle(22, 97, 26, 102, WHITE);
                locchoice = 1;
                break;
            case 1:
                uLCD.filled_rectangle(22, 97, 26, 102, BLACK);
                uLCD.filled_rectangle(22, 81, 26, 85, WHITE);
                locchoice = 0;
                break;
            }
            wait(0.2);
        }
    }
    if(locchoice == 0) {return true;}
    else {return false;}
}

int start_menu() //Display start menu, return difficulty in tens place, control in ones
{
    int diff;
    int control;
    int locdiff = 0;
    int loccontrol = 0;
    
    // PACMAN title
    uLCD.text_width(3);
    uLCD.text_height(3);  
    uLCD.filled_rectangle(19, 68, 108, 114, BLACK);
    uLCD.rectangle(19, 68, 108, 114, WHITE);
    uLCD.locate(0,1);
    uLCD.color(YELLOW);
    uLCD.printf("PACMAN");
    
    // Difficulty select screen
    uLCD.text_width(1);
    uLCD.text_height(1);
    uLCD.color(WHITE);
    uLCD.locate(4,7);
    uLCD.printf("DIFFICULTY");
    uLCD.locate(4,9);  
    uLCD.printf("EASY");
    uLCD.locate(4,11);  
    uLCD.printf("NORMAL");
    uLCD.locate(4,13);  
    uLCD.printf("HARD");
    uLCD.filled_rectangle(22, 73, 26, 77, WHITE);
    while(right_pb != 0) {
        if(up_pb == 0 && down_pb != 0) { //Move selection box up
            switch(locdiff) {
            case 0:
                uLCD.filled_rectangle(22, 73, 26, 77, BLACK);
                uLCD.filled_rectangle(22, 105, 26, 109, WHITE);
                locdiff = 2;
                break;
            case 1:
                uLCD.filled_rectangle(22, 89, 26, 94, BLACK);
                uLCD.filled_rectangle(22, 73, 26, 77, WHITE);
                locdiff = 0;
                break;
            case 2:
                uLCD.filled_rectangle(22, 105, 26, 109, BLACK);
                uLCD.filled_rectangle(22, 89, 26, 94, WHITE);
                locdiff = 1;
                break;
            }
            wait(0.2);
        }
        else if(down_pb == 0 && up_pb != 0) { //Move selection box down
            switch(locdiff) {
            case 0:
                uLCD.filled_rectangle(22, 73, 26, 77, BLACK);
                uLCD.filled_rectangle(22, 89, 26, 94, WHITE);
                locdiff = 1;
                break;
            case 1:
                uLCD.filled_rectangle(22, 89, 26, 94, BLACK);
                uLCD.filled_rectangle(22, 105, 26, 109, WHITE);
                locdiff = 2;
                break;
            case 2:
                uLCD.filled_rectangle(22, 105, 26, 109, BLACK);
                uLCD.filled_rectangle(22, 73, 26, 77, WHITE);
                locdiff = 0;
                break;
            }
            wait(0.2);
        }
    }
    
    // control select screen
    // clear bottom screen
    uLCD.color(BLACK);
    uLCD.locate(4,7);
    uLCD.printf("DIFFICULTY");
    uLCD.filled_rectangle(19, 68, 108, 114, BLACK);
    uLCD.rectangle(19, 68, 108, 114, WHITE);
    uLCD.color(WHITE);
    uLCD.locate(6,7);
    uLCD.printf("CONTROL");
    uLCD.locate(4,10);  
    uLCD.printf("ACCELEROM.");
    uLCD.locate(4,12);  
    uLCD.printf("QUAD STICK");
    uLCD.filled_rectangle(22, 81, 26, 85, WHITE);
    while(right_pb != 0) {
        if(up_pb == 0 && down_pb != 0) { //Move selection box up
            switch(loccontrol) {
            case 0:
                uLCD.filled_rectangle(22, 81, 26, 85, BLACK);
                uLCD.filled_rectangle(22, 97, 26, 102, WHITE);
                loccontrol = 1;
                break;
            case 1:
                uLCD.filled_rectangle(22, 97, 26, 102, BLACK);
                uLCD.filled_rectangle(22, 81, 26, 85, WHITE);
                loccontrol = 0;
                break;
            }
            wait(0.2);
        }
        else if(down_pb == 0 && up_pb != 0) { //Move selection box down
            switch(loccontrol) {
            case 0:
                uLCD.filled_rectangle(22, 81, 26, 85, BLACK);
                uLCD.filled_rectangle(22, 97, 26, 102, WHITE);
                loccontrol = 1;
                break;
            case 1:
                uLCD.filled_rectangle(22, 97, 26, 102, BLACK);
                uLCD.filled_rectangle(22, 81, 26, 85, WHITE);
                loccontrol = 0;
                break;
            }
            wait(0.2);
        }
    }
    return 10*locdiff + loccontrol;
    
}