/**
 *
 * @file drive.hpp
 * @author William Reinhardt
 *
 * Contains functionality for the drive.
 */

 #ifndef DRIVE_H
 #define DRIVE_H

#include "main.h"

/**
 *
 * brake
 *
 * Sets all motors of a given type to perform a hard-brake
 *
 * @param type is the type of motor to brake
 */
void brake(std::string type);

/**
 *
 * motor_voltage
 *
 * Moves a given type of motor at a given voltage (sorter motors always run in
 * the same direction)
 *
 * @param type is the type of motor to be moved
 * @param voltage is the voltage for the motor(s) to run at
 */
void move_voltage(std::string type, int voltage);

/**
 *
 * motor_degrees
 *
 * Moves a given type of motor a given number of degrees at a given speed
 *
 * @param type is the type of motor to be moved
 * @param degrees is the number of degrees the motor turns
 * @param speed is the velocity for the motor(s) to run at
 */
void move_degrees(std::string type, int degrees, int speed);

/**
 *
 * control_start
 *
 * Handles the controls/button mapping and the movement of the motors via the
 * controller
 *
 * @param ign ignore
 */
void control_start(void * ign);

/**
 *
 * drive
 *
 * Handles the usage of the joysticks and the movement of the drive
 *
 * @param ign ignore
 */
void drive(void * ign);

#endif
