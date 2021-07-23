/**
 *
 * @file globals.cpp
 * @author William Reinhardt
 *
 * Contains definitions for all global variables used in the program
 */

 #include "main.h"

/********* Config **********/
Ini_file config("/usd/config.ini");

int auton_id = std::stod(config.get("settings", "auton_id", "0"));
std::string alliance_color = config.get("settings", "alliance_color", "none");
std::string drive_model = config.get("settings", "drive_model", "skidsteer");
std::string drivesettings = config.get("controls", "drivesettings", "splitarcade");

std::vector<std::string> motor_labels;
const std::vector<std::string> motor_type_list = {"None", "Left Drive", "Right Drive", "Sorter", "Top-Roller", "Intake"};
std::vector<std::string> motor_types;

std::vector<std::string> sensor_labels;
const std::vector<std::string> sensor_type_list = {"None", "Back Bumper", "Limit", "Encoder", "Optical", "Distance"};
std::vector<std::string> sensor_types;
/***************************/

/******* Motor Ports *******/
std::vector<int8_t> motor_ports;

std::vector<int8_t> left_drive_ports;
std::vector<int8_t> right_drive_ports;

int8_t top_left_motor;
int8_t bottom_left_motor;
int8_t top_right_motor;
int8_t bottom_right_motor;

std::vector<int8_t> sensor_ports;
/***************************/


/***** Motor Reversals *****/
std::vector<bool> motor_reversals;
/*******************************/

std::vector<std::string> sensor_locations;


/*********** Controls ***********/
pros::Controller master(CONTROLLER_MASTER);

std::vector<std::string> controls;

std::vector<std::string> control_labels;
std::vector<pros::controller_digital_e_t> control_btns;
std::vector<std::string> control_motor_type;
std::vector<bool> control_toggle;
std::vector<bool> control_rev;
std::vector<int> control_speed;
std::vector<int> control_p_speed;
std::vector<int> control_p_degrees;
/********************************/


/******* Auton Presets **********/
std::string auton_labels[15];
std::string custom_paths[15];
/********************************/

std::shared_ptr<ChassisController> chassis;

/****** Profile Controller ******/
std::shared_ptr<AsyncMotionProfileController> profileController;
  /********************************/

  int intake_count = 0;
  int sorter_count = 0;
  int top_roller_count = 0;
  int ball_count = 0;
  int red_count = 0;
  int blue_count = 0;
  int exit_count = 0;
  std::array<char, 3> balls_carried;

  bool auto_sort_bool = false;
  bool start_sort_bool = false;
  bool sort_hold = false;
  bool dispel = false;
  bool motion_profiling = false;

/******** Optical Values ********/
  int optical_hue;
  int optical_saturation;
  int optical_brightness;
  int optical_rgb_red;
  int optical_rgb_green;
  int optical_rgb_blue;
  /********************************/
