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
//AnalogOut DACout(p18);
//wave_player waver(&DACout);

/* SD file */
SDFileSystem sd(p5, p6, p7, p8, "sd");

/* RGB led */
PwmOut red_led(p22);
PwmOut green_led(p23);
PwmOut blue_led(p24);

/* Mutex for lcd */
Mutex lcd_mutex;

/* Serial pc */
Serial pc(USBTX, USBRX);

/* mbed leds */
BusOut mbedleds(LED1,LED2,LED3,LED4);

/* Analog input of light sensor */
AnalogIn light_sensor(p19);
    
/* Volatile values */
volatile int rgb_wait = 2;
volatile int mario_wait = 500;
volatile float light_scale = 1.0f;


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
        float r = random_number() * light_scale;
        float g = random_number() * light_scale;
        float b = random_number() * light_scale;

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

void lightsensor_thread(void const *args) {
    while (true) {
        light_scale = (light_sensor * 15);
        Thread::wait(100);
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
    Thread ls_thr(lightsensor_thread);


    while (true) {
        /* Read wav from file */
//        FILE *wave_file = fopen("/sd/super_mario.wav", "r");
//        if(wave_file == NULL) {
//            pc.printf("Could not open file for read\n");
//        }
//        /* Play wave file */
//        waver.play(wave_file);
//        fclose(wave_file);
        Thread::wait(500);
    }
}
