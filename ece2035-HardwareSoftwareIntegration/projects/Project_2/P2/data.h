/** @file data.h */
#ifndef DATA_H
#define DATA_H

#include "mbed.h"
//#include "uLCD_4DGL.h"
#include "data.h"
#include "map_public.h"
#include "globals.h"

extern uLCD_4DGL uLCD;

typedef struct
{
    int score;
    int timeint;
    int level;
    int life;
    int numghost;
    int invtime;
    int invstart;
    int invstart_time;
    int invint;
    int control;
    int diff;
    int trial;
    bool play;
    bool lose;
    bool newlevel;
    bool noquit;
} data_t;

void data_init(data_t * d, int difficulty, int mode);


#endif //DATA_H
