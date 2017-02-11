#include "StepperMotor_X27168.h"

StepperMotor_X27168::StepperMotor_X27168(PinName A_f, PinName A_r, PinName B_f, PinName B_r) 
            :motor_control(A_f, A_r, B_f, B_r)
{
        motor_control = 0x0;
        max_position = MAX_POS;
        speed = DEFAULT_SPEED;
        cur_state = STOP_MOTOR;
        cur_position = 0;
}

StepperMotor_X27168::StepperMotor_X27168(PinName A_f, PinName A_r, PinName B_f, PinName B_r, int init_step_position)
            :motor_control(A_f, A_r, B_f, B_r)
{
        StepperMotor_X27168(A_f, A_r, B_f, B_r);
        step_position(init_step_position);
}

void StepperMotor_X27168::set_speed(float s) {
    speed = s;
}

int StepperMotor_X27168::get_speed() {
    return speed;
}

void StepperMotor_X27168::set_max_position(int p) {
    if(p<MAX_POS) {
        max_position = p;
    }
}

int StepperMotor_X27168::get_max_position() {
    return max_position;
}
     
int StepperMotor_X27168::step(int dir) {
    if(dir==2)
        cur_state = STOP_MOTOR;
    else if(dir == 0) {
        cur_state = (Polarity)((cur_state+1)%4);
        
        if(cur_position <= MAX_POS) {
            cur_position++;
        }
    }
    else if (dir == 1) {
        cur_state = (Polarity)((cur_state-1)%4);
        cur_state = (Polarity)(cur_state == 255 ? cur_state + 4 : cur_state);
        
        if(cur_position>= 0) {
            cur_position--;
        }
    }
    else
        return -1;
        
    switch (cur_state) {
        case 0:
            motor_control = 0x1;
            break;
        case 1:
            motor_control = 0x4;
            break;
        case 2:
            motor_control = 0x2;
            break;
        case 3:
            motor_control = 0x8;
            break;
        case 4:
            motor_control = 0x0;
            break;
    }
    wait(1.0/speed);
    return cur_position;
}

void StepperMotor_X27168::step_position(int pos) {
    if(pos > max_position)
        pos = max_position;
    else if(pos < 0)
        pos = 0;
    
    while(cur_position < pos) {
        step(0);
    }
    while(cur_position > pos) {
        step(1);
    }
    
    step(2);
}

void StepperMotor_X27168::angle_position(float angle) {
    step_position(int(angle*2));
}