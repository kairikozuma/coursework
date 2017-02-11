#include "mbed.h"
#include "Servo.h"
#include "rtos.h"
#include "ESP8266.h"
#include "motordriver.h"

/* Left and right motors */
Motor motor_l(p22, p11, p12, 1); // pwm, fwd, rev, can brake
Motor motor_r(p21, p13, p14, 1); // pwm, fwd, rev, can brake

/* Virtual USB serial port */
RawSerial pc(USBTX, USBRX); // tx, rx

/* ESP8266 wifi chip */
ESP8266 esp(p28, p27, p26, "SSID", "password"); // tx, rx, reset, SSID, password
#define RCV_BUF_SIZE 1024
char rcv[RCV_BUF_SIZE];
volatile int rcv_tail = 0;
volatile int rcv_head = 0;
    
/* Speaker */
PwmOut speaker(p24);

/* Servo with IR sensor mounted */
Servo servo_ir(p23);

/* IR sensor */
AnalogIn ir_sensor(p20);

/* mbed LEDs */
DigitalOut led1(LED1);
DigitalOut led4(LED4);

/* IR sensor thread */
void ir_thread(void const * args) {
    while (1) {
        if (ir_sensor > 0.8) {
            speaker = 1;
        } else {
            speaker = 0;
        }
        Thread::wait(50);
    }
}

/* Circular buffer to hold commands */
static int COM_BUF_SIZE = 100;
volatile char * command_queue;
volatile int queue_tail = 0;
volatile int queue_head = 0;

/* Command execute thread */
void command_exec_thread(void const *args) {
    while (1) {
        if (queue_tail < queue_head) {
            /* Save comand in temporary char variable */
            char command = command_queue[queue_tail];
            
            /* Null the command consumed */
            command_queue[queue_tail] = '\0';
            
            /* Execute the command */
            switch(command) {
            case '\0': /* null command */
                break;
            case 'F': /* Move forward */
                motor_r.speed(1); 
                motor_l.speed(1); 
                Thread::wait(400);
                motor_r.stop(1);
                motor_l.stop(1);
                break;
            case 'B': /* Move backwards */
                motor_r.speed(-1); 
                motor_l.speed(-1); 
                Thread::wait(400);
                motor_r.stop(1);
                motor_l.stop(1);
                break;
            case 'R': /* Move right */
                motor_l.speed(1); 
                Thread::wait(400);
                motor_l.stop(1);
                break;
            case 'L': /* Move left */
                motor_r.speed(1); 
                Thread::wait(400);
                motor_r.stop(1);
                break;
            case 'S': /* Turn servo right */
                if (servo_ir > 0.0) {
                    float val = (servo_ir - .2);
                    servo_ir = val > 0.0 ? val : 0.0;
                }
                break;
            case 'P': /* Turn servo left */
                if (servo_ir < 1.0) {
                    float val = (servo_ir + .2);
                    servo_ir = val < 1.0 ? val : 1.0;
                }
                break;
            default:
                break;
            }
            /* Increment command index in circular buffer */
            queue_tail = (queue_tail + 1) % COM_BUF_SIZE;
        }
        Thread::wait(200);
    }
}
 
 
/* Queue command thread*/
void command_queue_thread(void const *args) {
    while(1) {
        if (rcv_tail < rcv_head) {
            if (rcv[rcv_tail] == 'Z') {
                /* Loop until valid value found */
                while (rcv[(rcv_tail + 1) % RCV_BUF_SIZE] == '\0') {
                    Thread::wait(100);    
                }
                char let = rcv[(rcv_tail + 1) % RCV_BUF_SIZE];
                command_queue[queue_head] = let;
                queue_head = (queue_head + 1) % COM_BUF_SIZE;   
            }
            rcv[rcv_tail] = '\0'; /* Clear value */
            rcv_tail = (rcv_tail + 1) % RCV_BUF_SIZE;
        }
        Thread::wait(10);
    }
}
    
/* PC receive interrupt routine */
void pc_recv() {
    led1 = 1;
    while(pc.readable()) {
        /* Print out to ESP8266 hardware */
        esp.putc(pc.getc());
    }
    led1 = 0;
}

/* ESP83266 receive interrupt routine */
void dev_recv() {
    led4 = 1;
    while(esp.readable()) {
        char let = esp.getc();
        /* Echo to pc to see output */
        pc.putc(let);
        /* Put in receive buffer */
        rcv[rcv_head] = let;
        rcv_head = (rcv_head + 1) % RCV_BUF_SIZE;
    }
    led4 = 0;
}
 
/* Main code */
int main() {
    /* Set pc serial baud rate */
    pc.baud(9600);
    
    /* Set up esp */
    esp.reset();
    esp.baud(9600);
    esp.setup();
    
    /* Initialize command buffer */
    command_queue = (char*) calloc(sizeof(char), COM_BUF_SIZE);
    
    /* Attach interrupts */
    pc.attach(&pc_recv, Serial::RxIrq);
    esp.attach(&dev_recv, Serial::RxIrq);
    
    /* Print IP and MAC */
    esp.getIP();
    esp.getMAC();
    
    /* Start threads */
    Thread command_exec_thr(command_exec_thread);
    Thread command_queue_thr(command_queue_thread);
    Thread ir_thr(ir_thread);
    
    while(1) {
        Thread::wait(10000);
    }
}
