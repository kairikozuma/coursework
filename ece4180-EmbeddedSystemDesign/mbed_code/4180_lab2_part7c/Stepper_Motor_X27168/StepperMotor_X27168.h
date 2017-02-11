/** Stepper Motor control library
 *  
 *  @class   StepperMotor_X27168
 *  @author  Aditya Garg, Nisarg Himani
 *  @version 1.0 (Oct-19-2015)
 *  
 *  This is the driver for Automotive Gauge Stepper Motor X27168
 *  
 *  ----------------------IMPORTANT--------------------
 *  The API assumes motor is postioned at zero as there
 *  is no way to detect or control that in software
 *  ---------------------------------------------------
 */

#ifndef MBED_STEPPER_MOTOR_X27168
#define MBED_STEPPER_MOTOR_X27168

#include "mbed.h"

#define MAX_POS         629         // Maximum Steps Possible (not degrees)
#define DEFAULT_SPEED   500         // Default Speed in Steps per second

/** Stepper Motor control class
 *
 *  Example:
 *  @code
 *  #include "mbed.h"
 *  #include "StepperMotor_X27168.h"
 *
 *  StepperMotor_X27168 smotor(p25, p26, p23, p22);
 *  
 *  int main() {
 *  
 *      smotor.step_position(180);
 *      wait(0.5);
 *      
 *      smotor.step_position(100);
 *      wait(0.5);
 *      
 *      smotor.angle_position(270);
 *      wait(0.5);
 *      
 *      smotor.step_position(0);
 *      wait(0.5);
 *  }
 *  @endcode
 */

class StepperMotor_X27168 {

public:

    /** Constants for motor rotate control */
    typedef enum  {
        COIL_A_FOR = 0,         // Forward Polarity in H-Bright Port A
        COIL_B_FOR = 1,         // Forward Polarity in H-Bright Port B
        COIL_A_REV = 2,         // Reverse Polarity in H-Bright Port A
        COIL_B_REV = 3,         // Reverse Polarity in H-Bright Port B
        STOP_MOTOR = 4,         // Turn Off Both Coils
    } Polarity;
    
    /** Create a stepper motor object connected to specified DigitalOut pins
     *
     *  @param A_f DigitalOut pin for Forward Control of H-Brigde Port A (AIN1)
     *  @param A_r DigitalOut pin for Reverse Control of H-Brigde Port A (AIN2)
     *  @param B_f DigitalOut pin for Forward Control of H-Brigde Port B (BIN1)
     *  @param B_r DigitalOut pin for Reverse Control of H-Brigde Port B (BIN2)
     */
    StepperMotor_X27168(PinName A_f, PinName A_r, PinName B_f, PinName B_r);
    
    /** Create a stepper motor object connected to specified DigitalOut pins
     *  starting at specified position
     *
     *  @param A_f DigitalOut pin for Forward Control of H-Brigde Port A (AIN1)
     *  @param A_r DigitalOut pin for Reverse Control of H-Brigde Port A (AIN2)
     *  @param B_f DigitalOut pin for Forward Control of H-Brigde Port B (BIN1)
     *  @param B_r DigitalOut pin for Reverse Control of H-Brigde Port B (BIN2)
     *  @param init_step_position Rotate of given initial step position
     */
    StepperMotor_X27168(PinName A_f, PinName A_r, PinName B_f, PinName B_r, int init_step_position);
    
    /** Set the motor speed (i.e. number of steps per second)
     *  Motor will malfuntion is speed is faster than the
     *  the maximum capability if the motor.
     *
     *  @param s steps per second : lower number makes the turn slower (default = 1000)
     */
    void set_speed(float s);
    
    /** Get the motor speed (i.e. number of steps per second)
     *
     *  @return steps per second
     */
    int get_speed();
    
    /** Set the maximum steps the motor should take (not degrees)
     *
     *  @param p maximum_steps :(ignored if parameter is greater than 629, the physical limit of the motor)
     */
    void set_max_position(int p);
    
    /** Get the motor maximum position (int steps not degress)
     *
     *  @return maximum position
     */
    int get_max_position();
    
    /** Turn the motor one step (1/2 Degree)
     *
     *  @param dir 0 CLOCKWISE/FORWARD
     *             1 ANTI-CLOCKWISE/REVERSE
     *             2 STOP
     *
     *  @return current_position of the motor
     */            
    int step(int dir);
    
    /** Turn the motor to a specific step 
     *
     *  @param pos desired position in steps (0-max_position)
     */      
    void step_position(int pos);
    
    /** Turn the motor to a specific degree angle with a resolution of 0.5 degrees
     *
     *  @param angle desired angle (0-(max_positon/2))
     */
    void angle_position(float angle);
    
private:
    BusOut motor_control;       // 4-bit Bus Controlling the H-Brigde
    int max_position;           // Software Limit to motor rotation
    int speed;                  // Speed of Rotation
    int cur_position;           // Current Position of Motor (0-max_position)
    Polarity cur_state;         // Current State of H-Brige Controls
};
#endif