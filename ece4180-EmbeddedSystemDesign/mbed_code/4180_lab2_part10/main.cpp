#include "mbed.h"
#include "SongPlayer.h"

// Song test program - plays a song using PWM and timer interrupts
// for documentation see http://mbed.org/users/4180_1/notebook/using-a-speaker-for-audio-output/
// can be used to play a song, if you have the notes and durations
// for musical note frequencies see http://en.wikipedia.org/wiki/Piano_key_frequencies

//Set up notes and durations for sample song to play
// A 0.0 duration note at end terminates song play
float note[18]= {1568.0,1396.9,1244.5,1244.5,1396.9,1568.0,1568.0,1568.0,1396.9,
                 1244.5,1396.9,1568.0,1396.9,1244.5,1174.7,1244.5,1244.5, 0.0
                };
float duration[18]= {0.48,0.24,0.72,0.48,0.24,0.48,0.24,0.24,0.24,
                     0.24,0.24,0.24,0.24,0.48,0.24,0.48,0.48, 0.0
                    };

DigitalOut led1(LED1);
int main()
{
// setup instance of new SongPlayer class, mySpeaker using pin 24
// the pin must be a PWM output pin
    SongPlayer mySpeaker(p24);
// Start song and return once playing starts
    mySpeaker.PlaySong(note,duration);
    // loops forever while song continues to play to end using interrupts
    while(1) {
        led1 = !led1;
        wait(.1);
    }
}

