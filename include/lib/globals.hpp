/**
 *
 * @file globals.cpp
 * @author William Reinhardt
 *
 * Contains definitions for all global variables used in the program
 */


 #ifndef GLOBALS_H
 #define GLOBALS_H

#include "main.h"

/********* Config **********/
extern Ini_file config;

extern bool ramp;
extern int auton_id;
extern std::string alliance_color;
extern std::string drive_model;
extern std::string drivesettings;

extern std::vector<std::string> motor_labels;
extern const std::vector<std::string> motor_type_list;
extern std::vector<std::string> motor_types;

extern std::vector<std::string> sensor_labels;
extern const std::vector<std::string> sensor_type_list;
extern std::vector<std::string> sensor_types;
/***************************/

/******* Motor Ports *******/
extern std::vector<int8_t> motor_ports;

extern std::vector<int8_t> left_drive_ports;
extern std::vector<int8_t> right_drive_ports;

extern int8_t top_left_motor;
extern int8_t bottom_left_motor;
extern int8_t top_right_motor;
extern int8_t bottom_right_motor;

extern std::vector<int8_t> sensor_ports;
/***************************/

/***** Motor Reversals *****/
extern std::vector<bool> motor_reversals;
/*******************************/

extern std::vector<std::string> sensor_locations;

/*********** Controls ***********/
extern pros::Controller master;

extern std::vector<std::string> controls;

extern std::vector<std::string> control_labels;
extern std::vector<pros::controller_digital_e_t> control_btns;
extern std::vector<std::string> control_motor_type;
extern std::vector<bool> control_toggle;
extern std::vector<bool> control_rev;
extern std::vector<int> control_speed;
extern std::vector<int> control_p_speed;
extern std::vector<int> control_p_degrees;
/********************************/

/******* Auton Presets **********/
extern std::string auton_labels[15];
extern std::string custom_paths[15];
/********************************/

extern std::shared_ptr<ChassisController> chassis;

/****** Profile Controller ******/
extern std::shared_ptr<AsyncMotionProfileController> profileController;
/********************************/

extern int intake_count;
extern int sorter_count;
extern int top_roller_count;
extern int ball_count;
extern int red_count;
extern int blue_count;
extern int exit_count;
extern std::array<char, 3> balls_carried;

extern bool auto_sort_bool;
extern bool start_sort_bool;
extern bool sort_hold;
extern bool dispel;
extern bool motion_profiling;

/******** Optical Values ********/
extern int optical_hue;
extern int optical_saturation;
extern int optical_brightness;
extern int optical_rgb_red;
extern int optical_rgb_green;
extern int optical_rgb_blue;
/********************************/

#endif
