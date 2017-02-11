#include "data.h"
// Public functions
void data_init(data_t * d, int difficulty, int control) {
    d->score = 0;
    d->timeint = 500;
    d->level = 1;
    d->invtime = 0;
    d->invstart_time = 0;
    d->invstart = 6;
    d->play = true;
    d->lose = false;
    d->newlevel = true;
    d->control = control;
    d->diff = difficulty;
    d->noquit = true;
    d->trial = 0;
    
    switch(difficulty) {
        case 0: //easy
            d->invint = 1500;
            d->life = 5;
            d->numghost = 3;
            break;
        case 1: //medium
            d->invint = 800;
            d->life = 3;
            d->numghost = 3;
            break;
        case 2: //difficult
            d->invint = 300;
            d->life = 2;
            d->numghost = 3;
            break;
        default:
            d->invint = 800;
            d->life = 3;
            d->numghost = 2;
            break;
    }
}