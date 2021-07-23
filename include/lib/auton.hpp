/**
 *
 * @file auton.hpp
 * @author William Reinhardt
 *
 * Contains functionality for the autonomous routines.
 */

 #include "main.h"

 #ifndef AUTON_H
 #define AUTON_H

/**
 *
 * check_balls
 *
 * Actively uses the optical sensors to detect what color balls are being held
 * by the robot
 *
 * @param ign ignore
 */
void check_balls(void * ign);

/**
 *
 * deposit_ball
 *
 * Moves the top-roller enough to deposit a ball
 *
 */
void deposit_ball();

/**
 *
 * top_red_auton
 *
 * The default autonomous routine run in the upper red corner (defunct).
 */
void top_red_auton();

/**
 *
 * top_blue_auton
 *
 * The default autonomous routine run in the upper blue corner (defunct).
 */
void top_blue_auton();

/**
 *
 * bottom_red_auton
 *
 * The default autonomous routine run in the lower red corner (defunct).
 */
void bottom_red_auton();

/**
 *
 * bottom_blue_auton
 *
 * The default autonomous routine run in the lower blue corne (defunct)r.
 */
void bottom_blue_auton();

/************* Paths ************/
void upper_right_to_middle();

void upper_left_to_top_tower();

void upper_right_to_top_tower();

void lower_left_to_bottom_tower();

void lower_right_to_bottom_tower();

void top_left_tower_to_middle_tower();

void top_right_tower_to_middle_tower();
/********************************/

/**
 *
 * skills
 *
 * The autonomous routine run during skills.
 */
void skills();

/**
 *
 * custom_auton
 *
 * Parses a string of autonomous steps (each step separated by ':') into a vector and
 * processes each step one by one
 *
 * @param s is the string of autonomous steps to be parsed
 */
void custom_auton(const std::string &str);

#endif
