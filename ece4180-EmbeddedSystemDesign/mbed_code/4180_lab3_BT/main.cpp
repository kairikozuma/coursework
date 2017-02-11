#include "mbed.h"
#include "rtos.h"
#include "SDFileSystem.h"
#include "uLCD_4DGL.h"
#include "wave_player.h"

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

/* LED */
DigitalOut led1(LED1);

/* Mutex for lcd */
Mutex lcd_mutex; 

/* Bluetooth */
Serial  bt_module(p9,p10);

/* Volatile values changed by bluetooth */
volatile float r_val = 0.0;
volatile float g_val = 0.0;
volatile float b_val = 0.0;

void bt_recv() {
    led1 = 1;
//    while(bt_module.readable()) {
//        led1 = 1;
//        pc.putc(bt_module.getc());
//    }
//    char bred=0;
//    char bgreen=0;
//    char bblue=0;
//    if (bt_module.getc()=='!') {
//        if (bt_module.getc()=='C') { //color data packet
//            bred = bt_module.getc(); // RGB color values
//            bgreen = bt_module.getc();
//            bblue = bt_module.getc();
//            if (bt_module.getc()==char(~('!' + 'C' + bred + bgreen + bblue))) { //checksum OK?
//                r_val = bred/255.0; //send new color to RGB LED PWM outputs
//                g_val = bgreen/255.0;
//                b_val = bblue/255.0;
//            }
//        }
//    }
    led1 = 0;
}

void bt_thread(void const * args) {
    led1 = 1;
    char bred=0;
    char bgreen=0;
    char bblue=0;
    if (bt_module.getc()=='!') {
        if (bt_module.getc()=='C') { //color data packet
            bred = bt_module.getc(); // RGB color values
            bgreen = bt_module.getc();
            bblue = bt_module.getc();
            if (bt_module.getc()==char(~('!' + 'C' + bred + bgreen + bblue))) { //checksum OK?
                r_val = bred/255.0; //send new color to RGB LED PWM outputs
                g_val = bgreen/255.0;
                b_val = bblue/255.0;
            }
        }
    }
    led1 = 0;
}

/* LCD thread 1 */ 
void lcd_thread1(void const *args) {
    while (true) {
        lcd_mutex.lock();
        uLCD.set_sector_address(0x0000, 0x1000);
        uLCD.display_image(0,0);
        lcd_mutex.unlock();
        Thread::wait(500);
        
        lcd_mutex.lock();
        uLCD.set_sector_address(0x0000, 0x1021);
        uLCD.display_image(0,0);
        lcd_mutex.unlock();
        Thread::wait(500);
        
        lcd_mutex.lock();
        uLCD.set_sector_address(0x0000, 0x1042);
        uLCD.display_image(0,0);
        lcd_mutex.unlock();
        Thread::wait(500);
        
        lcd_mutex.lock();
        uLCD.set_sector_address(0x0000, 0x1063);
        uLCD.display_image(0,0);
        lcd_mutex.unlock();
        Thread::wait(500);
        
        lcd_mutex.lock();
        uLCD.set_sector_address(0x0000, 0x1085);
        uLCD.display_image(0,0);
        lcd_mutex.unlock();
        Thread::wait(500);
    }
}

/* LCD thread 2 */
void lcd_thread2(void const *args) {
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
inline float random_number() {
    return (rand()/(float(RAND_MAX)));
}

/* LED output */
void rgb_write(float r, float g, float b) {
    red_led.write(r);
    green_led.write(g);
    blue_led.write(b);    
}

/* RGB thread */
void rgb_thread(void const *args) {
    while (true) {
        
        /* Pulse some random color */
        float r = r_val;
        float g = g_val;
        float b = b_val;
        
        for (float i = 0.0; i < 1.0; i += 0.01) {
            rgb_write(r*i, g*i, b*i);
            Thread::wait(1);
        }
        for (float i = 1.0; i > 0.0; i -= 0.01) {
            rgb_write(r*i, g*i, b*i);
            Thread::wait(1);
        }
            
        if (random_number() > 0.5) {
            Thread::wait(404);
        } else {
            Thread::wait(202);
        }
    }
}
 
int main() {
        
    /* Set frequency of PWM signal */
    red_led.period(0.01f);
    green_led.period(0.01f);
    blue_led.period(0.01f);

    uLCD.media_init();
    
    /* Attach interrupt */
    bt_module.baud(9600);
//    bt_module.attach(&bt_recv, Serial::RxIrq);
        
    Thread lcd_thr1(lcd_thread1);
    Thread lcd_thr2(lcd_thread2);
    Thread rgb_thr(rgb_thread);
    Thread bt_thr(bt_thread);
    
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
