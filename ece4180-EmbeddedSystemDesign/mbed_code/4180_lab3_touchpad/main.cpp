#include "mbed.h"
#include "rtos.h"
#include "SDFileSystem.h"
#include "uLCD_4DGL.h"
#include "wave_player.h"
#include "Nav_Switch.h"
#include "mpr121.h"

/* LCD display */
uLCD_4DGL uLCD(p28,p27,p30); // serial tx, serial rx, reset pin;

/* Analog wav output */
AnalogOut DACout(p18);
wave_player waver(&DACout);

/* SD file */
SDFileSystem sd(p5, p6, p7, p8, "sd");
//MSCFileSystem fs ("fs");

/* RGB led */
PwmOut red_led(p22);
PwmOut green_led(p23);
PwmOut blue_led(p24);

/* Serial pc */
Serial pc(USBTX, USBRX);

/* Mutex for lcd */
Mutex lcd_mutex;

/* Navigation joystick */
Nav_Switch myNav( p15, p12, p13, p11, p14);

/* mbed leds */
BusOut mbedleds(LED1,LED2,LED3,LED4);

/* Interrupt for touchpad (IRQ) */
InterruptIn interrupt(p26);

/* Touchpad SDA, SCLK */
I2C i2c(p9, p10);

/* Touchpad constructor */
Mpr121 mpr121(&i2c, Mpr121::ADD_VSS);

/* Volatile values */
volatile int rgb_wait = 2;
volatile int mario_wait = 500;

/* Interrupt routine touchpad */
void fallInterrupt()
{
    int key_code=0;
    int i=0;
    int value=mpr121.read(0x00);
    value +=mpr121.read(0x01)<<8;
    // LED demo mod
    i=0;
    // puts key number out to LEDs for demo
    for (i=0; i<12; i++) {
        if (((value>>i)&0x01)==1) key_code=i+1;
    }
    /* 0 button */
    if (key_code == 1) {
        if (rgb_wait < 10) {
            rgb_wait++;
        }
    }

    /* 1 button */
    if(key_code == 2) {
        if (rgb_wait > 1) {
            rgb_wait--;
        }
    }

    /* 2 button */
    if(key_code == 3) {
        if (mario_wait > 0) {
            mario_wait-=50;
        }
    }

    /* 3 button */
    if(key_code == 4) {
        if (mario_wait < 2000) {
            mario_wait+=50;
        }
    }
    mbedleds = key_code;
}


/* LCD thread 1 */
void lcd_thread1(void const *args)
{
    while (true) {
        lcd_mutex.lock();
        uLCD.set_sector_address(0x0000, 0x1000);
        uLCD.display_image(0,0);
        lcd_mutex.unlock();
        Thread::wait(mario_wait);

        lcd_mutex.lock();
        uLCD.set_sector_address(0x0000, 0x1021);
        uLCD.display_image(0,0);
        lcd_mutex.unlock();
        Thread::wait(mario_wait);

        lcd_mutex.lock();
        uLCD.set_sector_address(0x0000, 0x1042);
        uLCD.display_image(0,0);
        lcd_mutex.unlock();
        Thread::wait(mario_wait);

        lcd_mutex.lock();
        uLCD.set_sector_address(0x0000, 0x1063);
        uLCD.display_image(0,0);
        lcd_mutex.unlock();
        Thread::wait(mario_wait);

        lcd_mutex.lock();
        uLCD.set_sector_address(0x0000, 0x1085);
        uLCD.display_image(0,0);
        lcd_mutex.unlock();
        Thread::wait(mario_wait);
    }
}

/* LCD thread 2 */
void lcd_thread2(void const *args)
{
    while (true) {
        lcd_mutex.lock();
        uLCD.filled_circle(90, 64, 20, 0xFF0000);
        lcd_mutex.unlock();
        Thread::wait(125);

        lcd_mutex.lock();
        uLCD.filled_circle(90, 64, 20, 0xFFFFFF);
        lcd_mutex.unlock();
        Thread::wait(250);
    }
}

/* Generate random number */
inline float random_number()
{
    return (rand()/(float(RAND_MAX)));
}

/* LED output */
void rgb_write(float r, float g, float b)
{
    red_led.write(r);
    green_led.write(g);
    blue_led.write(b);
}

/* RGB thread */
void rgb_thread(void const *args)
{
    while (true) {

        /* Pulse some random color */
        float r = random_number();
        float g = random_number();
        float b = random_number();

        for (float i = 0.0; i < 1.0; i += 0.01) {
            rgb_write(r*i, g*i, b*i);
            Thread::wait(rgb_wait);
        }
        for (float i = 1.0; i > 0.0; i -= 0.01) {
            rgb_write(r*i, g*i, b*i);
            Thread::wait(rgb_wait);
        }

    }
}

void joystick_thread(void const *args)
{
    while(true) {
        mbedleds = ~(myNav & 0x0F);
        /* Increase wait time between rgb led glow */
        /* DOWN */
        if(myNav & 0x01) {
            if (rgb_wait < 10) {
                rgb_wait++;
            }
        }

        /* Decrease wait time between rgb led glow */
        /* UP */
        if(myNav & 0x02) {
            if (rgb_wait > 1) {
                rgb_wait--;
            }
        }

        /* RIGHT */
        if(myNav & 0x04) {
            if (mario_wait > 0) {
                mario_wait-=50;
            }
        }

        /* LEFT */
        if(myNav & 0x08) {
            if (mario_wait < 2000) {
                mario_wait+=50;
            }
        }

        if(myNav.fire()) {
            wait(10);
        }
        Thread::wait(200);
    }
}

int main()
{

    /* Set frequency of PWM signal */
    red_led.period(0.01f);
    green_led.period(0.01f);
    blue_led.period(0.01f);

    /* Initialize LCD */
    uLCD.media_init();

    /* Declare four threads */
    Thread lcd_thr1(lcd_thread1);
    Thread lcd_thr2(lcd_thread2);
    Thread rgb_thr(rgb_thread);
    Thread js_thr(joystick_thread);

    /* Set interrupt mode for i2c*/
    interrupt.fall(&fallInterrupt);
    interrupt.mode(PullUp);

    while (true) {
        /* Read wav from file */
        FILE *wave_file = fopen("/sd/super_mario.wav", "r");
        if(wave_file == NULL) {
            pc.printf("Could not open file for read\n");
        }
        /* Play wave file */
        waver.play(wave_file);
        fclose(wave_file);
        Thread::wait(500);
    }
}
