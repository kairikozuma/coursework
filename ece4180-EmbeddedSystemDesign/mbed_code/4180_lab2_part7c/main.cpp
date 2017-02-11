#include "mbed.h"
#include "StepperMotor_X27168.h"

StepperMotor_X27168 smotor(p25, p26, p23, p22);

int main() {


    smotor.step_position(90);
    wait(0.5);
    
    smotor.step_position(180);
    wait(0.5);
    
    smotor.angle_position(270);
    wait(0.5);
    
    smotor.step_position(0);
    wait(0.5);
}
