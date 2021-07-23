/**
 *
 * @file gui.cpp
 * @author William Reinhardt
 *
 * Contains definitions for functions relating to GUI functionality.
 */

#include "main.h"

/***** Global Variables ****/
std::string prev;                                 //Stores which menu was displayed previously
std::string curr_motor;                           //Stores which motor is being edited in the motor settings
std::string curr_label;                           //Stores a value that is received from the terminal

std::vector<std::string> path_vec;                //Stores the instructions for the custom path

std::stringstream location;                       //Stores where the robot is and where it should go

bool is_skills;                                   //Determines which field is shown

static lv_obj_t * temperature;                    //Displays the temperature of the selected motor
static lv_obj_t * bumper_pressed;                 //Displays whether a switch is pressed
static lv_obj_t * optical_hue_label;              //Displays hue detected by optical sensor
static lv_obj_t * optical_saturation_label;       //Displays saturation detected by optical sensor
static lv_obj_t * optical_brightness_label;       //Displays brightness detected by optical sensor
static lv_obj_t * optical_rgb_label;              //Displays RGB values detected by optical sensor

bool get_btn_bool = false;                        //Enables get_task to sense for a button
bool get_limit_switch_bool = false;               //Enables get_limit_switch to get the switch state
bool get_optical_data_bool = false;               //Enables getoptical_data to get data from the optical sensor

static lv_obj_t * tiles_obj[36];                  //Array of mats that make up the field

static lv_obj_t * towers_obj[9];                  //Array of towers on the field

static lv_obj_t * blue_balls_obj[5];              //Array of blue balls on the field
static lv_obj_t * red_balls_obj[5];               //Array of red balls on the field
static lv_obj_t * red_balls_obj_skills[14];       //Array of red balls on the field during skills

tile_t tiles[36];                                 //Stores data for the tiles
tower_t towers[9];                                //Stores data for the towers
ball_t blue_balls[5];                             //Stores data for the blue balls
ball_t red_balls[5];                              //Stores data for the red balls
ball_t red_balls_skills[14];                      //Stores data for the red balls during skills

static lv_obj_t * robot;                          //Representation of the robot on the field

robot_t robot_data;                               //Stores data for the robot

static lv_obj_t * confirm_btn;                    //The confirm button used when making a custom path
static lv_obj_t * confirm_btn_label;              //Label for the aforementioned confirm button

static lv_obj_t * ta;                             //The text area displayed when using the keyboard

lv_task_t * get_temp_task;                        //Task that repeatedly gets the temperature of the selected motor
lv_task_t * get_limit_switch_task;                //Task that repeatedly gets the state of a limit/bumper switch
lv_task_t * get_btn_task;                         //Task that repeatedly gets the button being pressed on the controller
lv_task_t * get_optical_data_task;                //Task that repeatedly gets data from the optical sensor

int motor_set_id;                                 //Determines which motor is being controlled in the robot menu (path selector)
int motor_id;                                     //Determines which motor is being edited by the motor settings menu
int sensor_id;                                    //Determines which sensor is being edited by the sensor settings menu
int button_id;                                    //Determines which button action is being edited by the button mapper
int auton_preset_id;                              //Determines which autonomous preset is being edited by the path selector
int tower_id;                                     //Determines which tower is being manipulated in the path selector
int control_menu_id;                              //Determines which control configuration is being edited
int terminal_id;                                  //Determines what action is performed after get_terminal gets a line

std::string saveload;                             //Determines whether auton_list saves over or loads an autonomous preset

lv_obj_t * motion_profiling_sw;                   //The switch that enables motion profiling in the path selector

bool battery_override = false;                    //Determines if motion profiling will be enabled when the battery is less than 50%

lv_obj_t * battery_override_mbox;                 //Message box that enables battery_override if chosen
/***************************/

/*** Image Declarations ****/
LV_IMG_DECLARE(gtile);
LV_IMG_DECLARE(gtile_pr);
LV_IMG_DECLARE(gtile2);
LV_IMG_DECLARE(gtile2_pr);

LV_IMG_DECLARE(tower);
LV_IMG_DECLARE(tower_pr);
LV_IMG_DECLARE(btower);
LV_IMG_DECLARE(btower_pr);
LV_IMG_DECLARE(rtower);
LV_IMG_DECLARE(rtower_pr);
LV_IMG_DECLARE(bball);
LV_IMG_DECLARE(bball_pr);
LV_IMG_DECLARE(rball);
LV_IMG_DECLARE(rball_pr);

LV_IMG_DECLARE(big_bball);
LV_IMG_DECLARE(big_rball);

LV_IMG_DECLARE(robot_img);
LV_IMG_DECLARE(robot_img_pr);
/***************************/

/*******Style Definitions*******/
 static lv_style_t scr;
 static lv_style_t btn_bg;
 static lv_style_t std_btn;
 static lv_style_t std_btn_pr;
 static lv_style_t red_btn;
 static lv_style_t red_btn_pr;
 static lv_style_t switch_bg;
 static lv_style_t switch_indic;
 static lv_style_t switch_off;
 static lv_style_t switch_on;
 static lv_style_t cal_bg;
 static lv_style_t cal_header;
 static lv_style_t week_box;
 static lv_style_t today_box;
 static lv_style_t highlighted_day;
 static lv_style_t label_style;
 static lv_style_t kb_bg_style;
 static lv_style_t kb_style;
 static lv_style_t kb_style_pr;
 static lv_style_t imgbtn_disable;
 /*******************************/



 /**
  *
  * nav_btn
  *
  * Changes the shown screen when a button is pressed. The screen shown is
  * dependant upon the free number of the button.
  *
  * @param btn is the button to be pressed.
  */
 static lv_res_t nav_btn(lv_obj_t * btn){
   get_btn_bool = false;                          //Turns off get_btn_task
   get_limit_switch_task = lv_task_create(null, 999999, LV_TASK_PRIO_OFF, NULL);//Sets get_limit_switch_task to do nothing
   get_optical_data_task = lv_task_create(null, 999999, LV_TASK_PRIO_OFF, NULL);//Sets get_optical_data_task to do nothing
   lv_task_del(get_limit_switch_task);                                          //Deletes get_limit_switch_task
   lv_task_del(get_optical_data_task);                                          //Deletes get_optical_data_task
   pros::delay(100);                              //Waits 100 milliseconds
   int id = lv_obj_get_free_num(btn);             //Gets free number from the pressed button, stores it in "id"
   switch(id){                                    //Tests id
     case 0:                                      //If id = 0
       lv_obj_clean(lv_scr_act());                //Clears the screen
       if(prev == "Main Menu")                    //If prev (global) = "Main Menu"
         main_menu(NULL);                         //Loads the main menu
       else if(prev == "Motors"){                 //If prev (global) = "Motors"
         get_temp_task = lv_task_create(null, 999999, LV_TASK_PRIO_LOWEST, NULL);   //Sets get_temp_task to do nothing
         lv_task_del(get_temp_task);              //Deletes get_temp_task
         motor_select();                          //Loads motor selection menu
       }
       else if(prev == "Motor Settings")          //If prev (global) = "Motor Settings"
         motor_settings(motor_id);                //Loads motor settings menu with parameter motor_id (global)
       else if(prev == "Sensor Menu"){            //If prev (global) = "Sensor Menu"
         sensor_menu();                           //Opens sensor selection menu
       }
       else if(prev == "Controls")                //If prev (global) = "Controls"
         control_menu();                          //Loads control menu
       else if(prev == "Button Mapping"){         //If prev (global) = "Button Mapping"
         get_btn_task = lv_task_create(null, 999999, LV_TASK_PRIO_OFF, NULL);   //Sets get_btn_task to do nothing
         button_mapping_menu();                   //Loads button mapping menu
       }
       break;                                     //Breaks out of id switch statement
     case 1:                                      //Tests if id = 1
       lv_obj_clean(lv_scr_act());                //Clears the screen
       port_menu();                               //Opens port selection menu
       break;                                     //Breaks out of id switch statement
    case 7:                                       //Tests if id = 7
      lv_obj_clean(lv_scr_act());                 //Clears screen
      sensor_port_menu();                         //Opens sensor port selection menu
      break;                                      //Breaks out of id switch statement
   }
   return LV_RES_INV;                             //Returns LV_RES_INV (button is deleted)
 }

 /**
  *
  * nav_btnm
  *
  * Same as nav_btn but for button matrices.
  *
  * @param btn is the button matrix to be pressed.
  * @param txt is the text displayed on the pressed button (usually a number
  * or "Exit").
  */
 static lv_res_t nav_btnm(lv_obj_t * btn, const char *txt){
   lv_obj_clean(lv_scr_act());                    //Clears the screen
   pros::delay(100);                              //Waits 100 milliseconds
   if(!motor_labels.empty())                      //If motor_labels is not empty
     for(int i = 0; i < motor_labels.size(); i++) //For every item in motor_labels
       if(motor_labels[i].compare(txt) == 0 && (prev != "Controls" && prev != "Button Mapping"))    //If motor_labels[i] is equal to the text on the pressed button and not in controls
         motor_settings(i);                       //Open motor settings menu with i as a parameter
   if(!sensor_labels.empty())                     //If sensor_labels is not empty
     for(int i = 0; i < sensor_labels.size(); i++)//For every item in sensor_labels
       if(sensor_labels[i].compare(txt) == 0)     //If sensor_labels[i] is equal to the text on the pressed button
         sensor_settings(i);                      //Open sensor settings menu with i as a parameter
   if(!control_labels.empty())                    //If control_labels is not empty
     for(int i = 0; i < control_labels.size(); i++)                   //For every item in control_labels
        if(control_labels[i].compare(txt) == 0 && (prev == "Controls" || prev == "Button Mapping")){    //If control_labels[i] is equal to the text on the pressed button and in controls
          button_id = i;                          //Set button_id to i
          button_mapper();                        //Open button mapper
        }
   if(strcmp(txt, "Controls") == 0){              //Checks if button pressed says "Controls"
     control_menu();                              //Loads control menu
   }
   else if(strcmp(txt, "Chassis\nControls") == 0){//Checks if button pressed says "Chassis\nControls"
     chassis_controls();                          //Loads chassis control menu
   }
   else if(strcmp(txt, "Drive\nModel") == 0){     //Checks if button pressed says "Drive\nModel"
     model_select();                              //Loads model selection menu
   }
   else if(strcmp(txt, "Skid-Steer") == 0){       //Checks if button pressed says "Skid-Steer"
     drive_model = "skidsteer";                   //Changes drive_model (globals.cpp) to "skidsteer"
     config_update();                             //Updates configuration file
     exit_screen();                               //Prompts the user to restart the program
   }
   else if(strcmp(txt, "X-Drive") == 0){          //Checks if button pressed says "X-Drive"
     drive_model = "xdrive";                      //Changes drive_model (globals.cpp) to "xdrive"
     config_update();                             //Updates configuration file
     exit_screen();                               //Prompts the user to restart the program
   }
   else if(strcmp(txt, "Button\nMapping") == 0){  //Checks if button pressed says "Button\nMapping"
     button_mapping_menu();                       //Loads button mapping menu
   }
   else if(strcmp(txt, "Tank") == 0){             //Checks if button pressed says "Tank"
     drivesettings = "tank";                      //Changes drivesettings (globals.cpp) to "tank"
     config_update();                             //Updates config file
     main_menu(NULL);                             //Loads main menu
   }
   else if(strcmp(txt, "Arcade (Left)") == 0){    //Checks if button pressed says "Arcade (Left)"
     drivesettings = "arcadeleft";                //Changes drivesettings (globals.cpp) to "arcadeleft"
     config_update();                             //Updates config file
     main_menu(NULL);                             //Loads main menu
   }
   else if(strcmp(txt, "Arcade (Right)") == 0){   //Checks if button pressed says "Arcade (Right)"
     drivesettings = "arcaderight";               //Changes drivesettings (globals.cpp) to "arcaderight"
     config_update();                             //Updates config file
     main_menu(NULL);                             //Loads main menu
   }
   else if(strcmp(txt, "Split Arcade") == 0){     //Checks if button pressed says "Split Arcade"
     drivesettings = "splitarcade";               //Changes drivesettings (globals.cpp) to "splitarcade"
     config_update();                             //Updates config file
     main_menu(NULL);                             //Loads main menu
   }
   else if(strcmp(txt, "Auton") == 0){            //Checks if button pressed says "Auton"
     select_field();                              //Opens menu to select normal or skills field
   }
   else if(strcmp(txt, "Normal\nField") == 0){    //Checks if button pressed says "Normal\nField"
     is_skills = false;                           //Sets is_skills to false (skills field not selected)
     path_vec.clear();                            //Empties path_vec (global)
     robot_data.balls.clear();                    //Clears balls held by robot
     location.str("");                            //Clears location (global)
     auton_menu();                                //Opens autonomous menu
   }
   else if(strcmp(txt, "Skills\nField") == 0){    //Checks if button pressed says "Skills\nField"
     is_skills = true;                            //Sets is_skills to true (skills field is selected)
     path_vec.clear();                            //Empties path_vec
     robot_data.balls.clear();                    //Clears balls held by robot
     location.str("");                            //Clears locaation (global)
     auton_menu();                                //Opens autonomous menu
   }
   else if(strcmp(txt, "Standard\nPath") == 0){   //Checks if button pressed says "Standard\nPath"
     main_menu(NULL);                             //Loads main menu
   }
   else if(strcmp(txt, "Custom\nPath") == 0){     //Checks if button pressed says "Custom\nPath"
     custom_path_menu();                          //Opens path selector
   }
   else if(strcmp(txt, "Motors") == 0){           //Checks if button pressed says "Motors"
     motor_select();                              //Opens motor selection menu
   }
   else if(strcmp(txt, "Sensors") == 0){          //Checks if button pressed says "Sensors"
     sensor_menu();                               //Opens sensor selection menu
   }
   else if(strcmp(txt, "Skills") == 0){           //Checks if button pressed says "Skills"
     skills_select();                             //Selects skills if battery > 50% (see skills_select)
   }
   else if (strcmp(txt, "Exit") == 0){            //Checks if button pressed says "Exit"
     if(prev == "Main Menu")                      //If the previous screen was the main menu
       main_menu(NULL);                           //Load the main menu
     else if(prev == "Motor Select")              //If the previous screen was the motor selection menu
       motor_select();                            //Opens the motor selection menu
     else if(prev == "Motor Settings")            //If the previous screen was the motor settings menu
       motor_settings(motor_id);                  //Opens the motor settings menu with parameter motor_id (global)
     else if(prev == "Sensor Menu")               //If the previous screen was the sensor selection menu
       sensor_menu();                             //Opens sensor selection menu
     else if(prev == "Auton")                     //If the previous screen was the autonomous menu
       auton_menu();                              //Opens the autonomous menu
     else if(prev == "Controls")                  //If the previous screen was the controls menu
       control_menu();                            //Opens the control menu
     else if(prev == "Button Mapping")            //If the previous screen was the button mapping menu
       button_mapping_menu();                     //Opens button mapping menu
   }
   return LV_RES_INV;                             //Returns LV_RES_INV (button matrix is deleted)
 }

 /**
  *
  * load_main_menu
  *
  * Loads the main menu or an error screen after the microSD Card error is
  * displayed
  *
  * @param mbox is the message box displayed on the screen.
  * @param txt is the text displayed on the buttons inside the message box.
  */
 static lv_res_t load_main_menu(lv_obj_t * mbox, const char *txt){
   lv_obj_clean(lv_scr_act());                      //Clears the screen
   if(strcmp(txt, "Yes") == 0)                      //If the pressed button says "Yes"
    main_menu(NULL);           //Opens main menu in new task
  else{                                             //If the pressed button says "No"
    exit_screen();
  }
  return LV_RES_INV;                                //Returns LV_RES_INV (mbox is deleted)
 }

 /**
  *
  * motor_rev
  *
  * Reverses a different motor depending on the id of the switch.
  *
  * @param sw is the switch that is pressed to reverse the motor
  */
 static lv_res_t motor_rev(lv_obj_t * sw){
 uint8_t id = lv_obj_get_free_num(sw);              //Gets free number of switch, stores it in "id"

 for(int i = 0; i < motor_reversals.size(); i++)    //For every motor reversal in motor_reversals (globals.cpp)
   if(id == i)                                      //If id = i
    motor_reversals[i] = !motor_reversals[i];       //Reverse motor reversal of the selected motor

 config_update();                                 //Updates config file
 return LV_RES_OK;                                //Returns LV_RES_OK (switch is not deleted)
}

/**
 *
 * change_motor_port
 *
 * Changes motor port based on the selected button
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t change_motor_port(lv_obj_t * btn, const char *txt){
  if(strcmp(txt, "Exit") != 0){                   //Checks if the button pressed does not say "Exit"
    uint8_t port = std::atoi(txt);                //Stores text on pressed button in "port"
    for(int i = 0; i < motor_ports.size(); i++)   //For every motor port in motor_ports (globals.cpp)
      if(curr_motor == motor_labels[i])           //If curr_motor (global) is equal to the selected motor
        motor_ports[i] = port;                    //The port of the selected motor is changed to port
  }
  lv_obj_clean(lv_scr_act());                     //Clears the screen
  motor_settings(motor_set_id);                   //Loads motor selection menu
  config_update();                                //Updates config file
  return LV_RES_INV;                              //Returns LV_RES_INV (button matrix is deleted)
}


/**
 *
 * change_motor_type
 *
 * Changes motor type based on the ddlist selection
 *
 * @param ddlist is the list of motor types
 */
static lv_res_t change_motor_type(lv_obj_t * ddlist){
  char selected[50];
  lv_ddlist_get_selected_str(ddlist, selected);
  std::string selected_str(selected);
  motor_types[motor_set_id] = selected_str;
  if(selected_str == "Left Drive")        //If selected motor type is "Left Drive"
    left_drive_ports.push_back(motor_ports[motor_set_id]);            //Adds port of selected motor to left_drive_ports
  else if(selected_str == "Right Drive")  //If selected motor type is "Right Drive"
    right_drive_ports.push_back(motor_ports[motor_set_id]);           //Adds port of selected motor to right_drive_ports
  else{                                             //If selected motor type is not "Left Drive" or "Right Drive"
    if(std::find(left_drive_ports.begin(), left_drive_ports.end(), motor_ports[motor_set_id]) != left_drive_ports.end()){   //If selected motor port is in left_drive_ports
      left_drive_ports.erase(std::find(left_drive_ports.begin(), left_drive_ports.end(), motor_ports[motor_set_id]));   //Delete selected motor port from left_drive_ports
    }
    if(std::find(right_drive_ports.begin(), right_drive_ports.end(), motor_ports[motor_set_id]) != right_drive_ports.end()){    //If selected motor port is in right_drive_ports
      right_drive_ports.erase(std::find(right_drive_ports.begin(), right_drive_ports.end(), motor_ports[motor_set_id]));    //Delete selected motor port from right_drive_ports
    }
  }
  config_update();                                  //Updates config.ini
  return LV_RES_OK;                                 //Returns LV_RES_OK (ddlist is not deleted)
}

/**
 *
 * change_sensor_type
 *
 * Changes sensor type based on the ddlist selection
 *
 * @param ddlist is the list of sensor types
 */
static lv_res_t change_sensor_type(lv_obj_t * ddlist){
  int index = lv_ddlist_get_selected(ddlist);       //Gets index of selected option
  sensor_types[sensor_id] = sensor_type_list[index];//Sets sensor type of selected sensor to index of selected sensor type
  config_update();                                  //Updates config.ini
  return LV_RES_OK;                                 //Returns LV_RES_OK (ddlist is not deleted)
}

/**
 *
 * change_sensor_type
 *
 * Changes sensor location based on the ddlist selection
 *
 * @param ddlist is the list of sensor locations
 */
static lv_res_t change_sensor_location(lv_obj_t * ddlist){
  char selected[50];                              //Creates empty char array
  lv_ddlist_get_selected_str(ddlist, selected);   //Stores ddlist selection in char array
  std::string selected_str(selected);             //Creates a string out of the char array
  sensor_locations[sensor_id] = selected_str;     //Updates the selected sensors location
  config_update();                                //Updates the configuration file
  return LV_RES_OK;                               //Returns LV_RES_OK (ddlist is not deleted)
}

/**
 *
 * change_sensor_port
 *
 * Changes sensor port based on the selected button
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t change_sensor_port(lv_obj_t * btnm, const char * txt){
  int port = std::atoi(txt);                      //Stores text on pressed button in "port"
  sensor_ports[sensor_id] = port;                 //Sets port of selected sensor to "port"
  lv_obj_clean(lv_scr_act());                     //Clears the screen
  sensor_settings(sensor_id);                     //Loads sensor settings menu for selected sensor
  config_update();                                //Updates config file
  return LV_RES_INV;                              //Returns LV_RES_INV (button matrix is deleted)
}

/**
 *
 * edit_motor
 *
 * Handles the motor editing menu
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t edit_motor(lv_obj_t * btnm, const char *txt){
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  if(strcmp(txt, SYMBOL_PLUS) == 0)                 //If the pressed button displays SYMBOL_PLUS
    add_motor();                                    //Open menu to add a motor
  else                                              //If the pressed button displays SYMBOL_MINUS
    remove_motor();                                 //Open menu to remove motor
  return LV_RES_INV;                                //Returns LV_RES_INV (button matrix is deleted)
}

/**
 *
 * apply_motor
 *
 * Adds motor data to all motor-related vectors.
 *
 * @param kb is the keyboard being used
 */
static lv_res_t apply_motor(lv_obj_t * kb){
  std::string name(lv_ta_get_text(ta));             //Creates string that holds content of text area
  motor_labels.push_back(name);                     //Pushes string into the back of motor_labels
  motor_ports.push_back(1);                         //Pushes 1 into the back of motor_ports
  motor_reversals.push_back(false);                 //Pushes false into the back of motor_reversals
  motor_types.push_back("None");                    //Pushes "None" into the back of motor_types
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  config_update();                                  //Updates config.ini
  motor_select();                                   //Returns to motor selection menu
  return LV_RES_INV;                                //Returns LV_RES_INV (kb is deleted)
}

/**
 *
 * apply_remove_motor
 *
 * Opens menu that confirms that the user wants to delete the selected motor.
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t apply_remove_motor(lv_obj_t * btnm, const char *txt){
  if(!motor_labels.empty())                         //If motor_labels is not empty
    for(int i = 0; i < motor_labels.size(); i++)    //For every item in motor_labels
      if(motor_labels[i].compare(txt) == 0){        //If txt is equal to motor_labels[i]
        motor_set_id = i;                           //Set motor_set_id to i
        std::string remove_confirm_txt = "Delete " + motor_labels[i] + "?";     //Creates mbox string

        static const char *btns[] = {"Yes", "No", ""};                //Creates mbox buttons

        lv_obj_t * remove_confirm_mbox = lv_mbox_create(lv_scr_act(), NULL);
        lv_mbox_set_text(remove_confirm_mbox, remove_confirm_txt.c_str());  //Sets mbox text
        lv_mbox_add_btns(remove_confirm_mbox, btns, delete_motor);        //Adds buttons to mbox
        lv_obj_set_size(remove_confirm_mbox, 200, 200);                 //Sets size of mbox
        lv_obj_align(remove_confirm_mbox, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);   //Aligns mbox to the center of the screen
      }
  return LV_RES_OK;                                 //Returns LV_RES_OK (btnm is not deleted)
}

/**
 *
 * delete_motor
 *
 * Deletes data for selected motor from all motor-related vectors.
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t delete_motor(lv_obj_t * mbox, const char *txt){
  if(strcmp(txt, "Yes") == 0){                      //If the pressed says "Yes"
    motor_labels.erase(motor_labels.begin() + motor_set_id);          //Erases selected motor from motor_labels
    motor_ports.erase(motor_ports.begin() + motor_set_id);            //Erases selected motor from motor_ports
    motor_reversals.erase(motor_reversals.begin() + motor_set_id);    //Erases selected motor from motor_reversals
    motor_types.erase(motor_types.begin() + motor_set_id);            //Erases selected motor from motor_types

    if(motor_types[motor_set_id] == "Left Drive"){     //If the selected motor is of type "Left Drive"
      for(int i = 0; i < left_drive_ports.size(); i++)                //For every item in left_drive_ports
        if(motor_ports[sensor_id] == left_drive_ports[i])             //If the selected motor port is in left_drive_ports
          left_drive_ports.erase(left_drive_ports.begin() + i);       //Erases selected motor port from left_drive_ports
    }
    else if(motor_types[motor_set_id] == "Right Drive"){                 //If the selected motor is of type "Right Drive"
      for(int i = 0; i < right_drive_ports.size(); i++)               //For every item in right_drive_ports
        if(motor_ports[sensor_id] == right_drive_ports[i])            //IF the selected motor port is in right_drive_ports
          right_drive_ports.erase(right_drive_ports.begin() + i);     //Erases selected motor port from right_drive_ports
    }
  }
  config_update();                                  //Updates config.ini
  lv_obj_clean(lv_scr_act());                       //Clears screen
  motor_select();                                   //Loads motor selection screen
  return LV_RES_INV;                                //Returns LV_RES_INV (mbox is deleted)
}

/**
 *
 * edit_sensor
 *
 * Handles the sensor editing menu
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t edit_sensor(lv_obj_t * btnm, const char *txt){
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  if(strcmp(txt, SYMBOL_PLUS) == 0)                 //If the pressed button displays SYMBOL_PLUS
    add_sensor();                                   //Open menu to add a sensor
  else                                              //If the pressed button display SYMBOL_MINUS
    remove_sensor();                                //Open menu to remove a sensor
  return LV_RES_INV;                                //Returns LV_RES_INV (btnm is deleted)
}

/**
 *
 * apply_sensor
 *
 * Adds sensor data to all sensor-related vectors.
 *
 * @param kb is the keyboard being used
 */
static lv_res_t apply_sensor(lv_obj_t * kb){
  sensor_labels.push_back(lv_ta_get_text(ta));      //Pushes contents of text area into the back of sensor labels
  sensor_ports.push_back(1);                        //Pushes 1 into sensor_ports
  sensor_types.push_back("None");                   //Pushes "None" into sensor_types
  sensor_locations.push_back("None");               //Pushes "None" into sensor_locations
  config_update();                                  //Updates config.ini
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  sensor_menu();                                    //Opens sensor selection menu
  return LV_RES_INV;                                //Returns LV_RES_INV (kb is deleted)
}

/**
 *
 * apply_remove_sensor
 *
 * Opens menu that confirms that the user wants to delete the selected sensor.
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t apply_remove_sensor(lv_obj_t * btnm, const char *txt){
  if(strcmp(txt, "Exit") != 0){                     //If the pressed button does not say "Exit"
    for(int i = 0; i < sensor_labels.size(); i++)   //For every item in sensor_labels
      if(strcmp(txt, sensor_labels[i].c_str()) == 0){                 //If the text of the pressed utton is equal to sensor_labels[i]
        sensor_id = i;                              //Sets sensor_id to i
        std::string remove_confirm_txt = "Delete " + sensor_labels[i] + "?";    //Creates mbox string

        static const char *btns[] = {"Yes", "No", ""};                //Creates mbox buttons

        lv_obj_t * remove_confirm_mbox = lv_mbox_create(lv_scr_act(), NULL);
        lv_mbox_set_text(remove_confirm_mbox, remove_confirm_txt.c_str());  //Sets mbox text
        lv_mbox_add_btns(remove_confirm_mbox, btns, delete_sensor);        //Adds buttons to mbox
        lv_obj_set_size(remove_confirm_mbox, 200, 200);                 //Sets size of mbox
        lv_obj_align(remove_confirm_mbox, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);   //Aligns mbox to the center of the screen
      }
    return LV_RES_OK;                               //Returns LV_RES_OK (btnm is not deleted)
  }
  else{                                             //If the pressed button says "Exit"
    lv_obj_clean(lv_scr_act());                     //Clears the screen
    sensor_menu();                                  //Loads sensor selection menu
    return LV_RES_INV;                              //Returns LV_RES_INV (btnm is deleted)
  }
}

/**
 *
 * delete_sensor
 *
 * Deletes data for selected sensor from all sensor-related vectors.
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t delete_sensor(lv_obj_t * mbox, const char *txt){
  if(strcmp(txt, "Yes") == 0){                      //If the pressed button says "Yes"
    sensor_labels.erase(sensor_labels.begin() + sensor_id);           //Erase selected sensor from sensor_labels
    sensor_ports.erase(sensor_ports.begin() + sensor_id);             //Erase selected sensor from sensor_ports
    sensor_types.erase(sensor_types.begin() + sensor_id);             //Erase selected sensor from sensor_types
    sensor_locations.erase(sensor_locations.begin() + sensor_id);     //Erase selected sensor from sensor_locations
    config_update();                                //Updates config.ini
  }
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  sensor_menu();                                    //Opens the sensor menu
  return LV_RES_INV;                                //Returns LV_RES_INV (mbox is deleted)
}

/**
 *
 * edit_controls
 *
 * Handles the button editing menu
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t edit_controls(lv_obj_t * btnm, const char *txt){
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  if(strcmp(txt, SYMBOL_PLUS) == 0)                 //If the button pressed displays SYMBOL_PLUS
    add_controls();                                 //Opens menu to add controls
  else                                              //If the button pressed does not display SYMBOL_PLUS
    remove_controls();                              //Opens menu to remove controls
  return LV_RES_INV;                                //Returns LV_RES_INV (btnm is deleted)
}

/**
 *
 * apply_controls
 *
 * Adds button data to all button-related vectors.
 *
 * @param kb is the keyboard being used
 */
static lv_res_t apply_controls(lv_obj_t * kb){
  std::string name(lv_ta_get_text(ta));             //Creates string from the contents of the text area
  control_labels.push_back(name);                   //Pushes contents of the text area into control_labels
  control_btns.push_back((pros::controller_digital_e_t) -1);          //Pushes a dummy button into control_btns (does nothing)
  control_motor_type.push_back("None");             //Pushes "None" into control_motor_type (does not control a motor)
  control_toggle.push_back(false);                  //Pushes false into control_toggle (is not togglable)
  control_rev.push_back(0);
  control_speed.push_back(0);                       //Pushes 0 into control_speed
  control_p_speed.push_back(0);                     //Pushes 0 into control_p_speed
  control_p_degrees.push_back(0);                   //Pushes 0 into control_p_degrees
  config_update();                                  //Updates the configuration file
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  button_mapping_menu();                            //Returns to button mapping menu
  return LV_RES_INV;                                //Returns LV_RES_INV (kb is deleted)
}

/**
 *
 * apply_remove_controls
 *
 * Opens menu that confirms that the user wants to delete the selected button.
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t apply_remove_controls(lv_obj_t * btnm, const char *txt){
  if(!control_labels.empty())                       //If control_labels is not empty
    for(int i = 0; i < control_labels.size(); i++)              //For every item in control_labels
      if(control_labels[i].compare(txt) == 0){      //If the current button label is equal to the text on the selected button
        button_id = i;                              //button_id is set to i
        std::string remove_confirm_txt = "Delete " + control_labels[i] + "?";   //Creates text for mbox

        static const char *btns[] = {"Yes", "No", ""};                //Creates button for mbox

        lv_obj_t * remove_confirm_mbox = lv_mbox_create(lv_scr_act(), NULL);    //Creates mbox
        lv_mbox_set_text(remove_confirm_mbox, remove_confirm_txt.c_str());      //Sets mbox text
        lv_mbox_add_btns(remove_confirm_mbox, btns, delete_controls); //Adds buttons to mbox
        lv_obj_set_size(remove_confirm_mbox, 200, 200);               //Sets size of mbox
        lv_obj_align(remove_confirm_mbox, lv_scr_act(), LV_ALIGN_CENTER, 0, 0); //Aligns mbox to the center of the screen
    }
  return LV_RES_OK;                                 //Returns LV_RES_OK (btnm is not deleted)
}

/**
 *
 * delete_controls
 *
 * Deletes data for selected button from all control-related vectors.
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t delete_controls(lv_obj_t * mbox, const char *txt){
  if(strcmp(txt, "Yes") == 0){                      //If the selected button says "Yes"
    control_labels.erase(control_labels.begin() + button_id);         //Erase item at button_id from control_labels
    control_btns.erase(control_btns.begin() + button_id);             //Erase item at button_id from control_btns
    control_motor_type.erase(control_motor_type.begin() + button_id); //Erase item at button_id from control_motor_type
    control_toggle.erase(control_toggle.begin() + button_id);         //Erase item at button_id from control_toggle
    control_speed.erase(control_speed.begin() + button_id);           //Erase item at button_id from control_speed
    control_rev.erase(control_rev.begin() + button_id);           //Erase item at button_id from control_speed
    control_p_speed.erase(control_p_speed.begin() + button_id);       //Erase item at button_id from control_p_speed
    control_p_degrees.erase(control_p_degrees.begin() + button_id);   //Erase item at button_id from control_p_degrees
  }
  config_update();                                  //Updates configuration file
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  button_mapping_menu();                            //Returns to button mapping menu
  return LV_RES_INV;                                //Returns LV_RES_INV (mbox is deleted)
}

/**
 *
 * control_nav
 *
 * Handles navigation within the control editing menu
 *
 * @param btn is the button being pressed
 */
static lv_res_t control_nav(lv_obj_t * btn){

  lv_obj_clean(lv_scr_act());                       //Clears the screen

  int nav = lv_obj_get_free_num(btn);               //Stores free number of btn into a variable "nav"
  if(nav == 0)                                      //If nav is 0
    control_menu_id--;                              //Decrement control_menu_id (global)
  else                                              //If nav is not 0
    control_menu_id++;                              //Increment control_menu_id (global)

  switch(control_menu_id){                          //Tests control_menu_id (global)
    case 0:                                         //If control_menu_id (global) is 0
      button_mapper();                              //Open button mapper
      break;                                        //Break out of switch statement
    case 1:                                         //If control_menu_id (global) is 1
      control_type_menu();                          //Open control type editor
      break;                                        //Break out of switch statement
    case 2:                                         //If control_menu_id (global) is 2
      control_params_menu_one();                    //Open first param editor
      break;                                        //Break out of switch statement
  }
  return LV_RES_INV;                                //Returns LV_RES_INV (btn is deleted)
}

/**
 *
 * disable_button
 *
 * Disables the selected button
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t disable_button(lv_obj_t * mbox, const char * txt){
  control_btns[button_id] = (pros::controller_digital_e_t) -1;        //Sets control_btns at button_id to a dummy value (does nothing)
  config_update();                                  //Updates configuration file
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  button_mapper();                                  //Returns to button mapper
  return LV_RES_INV;                                //Returns LV_RES_INV (mbox is deleted)
}

/**
 *
 * change_button_type
 *
 * Changes what type of motor the button controls based on the ddlist selection
 *
 * @param ddlist is the list of sensor types
 */
static lv_res_t change_btn_type(lv_obj_t * ddlist){
  char selection[50];                               //Creates empty char array
  lv_ddlist_get_selected_str(ddlist, selection);    //Stores ddlist selection in char array
  std::string selection_str(selection);             //Creates string out of char aray
  control_motor_type[button_id] = selection_str;    //Changes control_motor_type at button_id to string
  config_update();                                  //Updates configuration file
  return LV_RES_OK;                                 //Returns LV_RES_OK (ddlist is not deleted)
}

/**
 *
 * button_mapper
 *
 * Opens a menu and waits for a digital input on the controller, then assigns
 * that input to a function on the robot based on the selected on-screen button.
 *
 */
void button_mapper(){

  prev = "Button Mapping";                        //Sets previous screen to button mapping menu

  lv_obj_clean(lv_scr_act());                     //Clears the screen

  std::stringstream text;                         //Text displayed on the control label
  std::string control_label;                      //The selected action (displayed on a label)
  std::string current_btn;                        //The name of the button currently assigned to the selected action
  int btn_assignment;                             //The id of the button currently assigned to the selected action

  control_label = control_labels[button_id];                          //control_label is set to "Drive Reverse"
  btn_assignment = (int) control_btns[button_id];                     //btn_assignment is set to drive_rev (globals.cpp)

  switch(btn_assignment){                         //Tests btn_assignment
    case 6:                                       //If btn_assignment is equal to 6 (L1)
      current_btn = "L1";                         //current_btn is set to "L1"
      break;                                      //Breaks out of btn_assignment switch statement
    case 7:                                       //If btn_assignment is equal to 7 (L2)
      current_btn = "L2";                         //current_btn is set to "L2"
      break;                                      //Breaks out of btn_assignment switch statement
    case 8:                                       //If btn_assignment is equal to 8 (R1)
      current_btn = "R1";                         //current_btn is set to "R1"
      break;                                      //Breaks out of btn_assignment switch statement
    case 9:                                       //If btn_assignment is equal to 9 (R2)
      current_btn = "R2";                         //current_btn is set to "R2"
      break;                                      //Breaks out of btn_assignment switch statement
    case 10:                                      //If btn_assignment is equal to 10 (Up)
      current_btn = "Up";                         //current_btn is set to "Up"
      break;                                      //Breaks out of btn_assignment switch statement
    case 11:                                      //If btn_assignment is equal to 11 (Down)
      current_btn = "Down";                       //current_btn is set to "Down"
      break;                                      //Breaks out of btn_assignment switch statement
    case 12:                                      //If btn_assignment is equal to 12 (Left)
      current_btn = "Left";                       //current_btn is set to "Left"
      break;                                      //Breaks out of btn_assignment switch statement
    case 13:                                      //If btn_assignment is equal to 13 (Right)
      current_btn = "Right";                      //current_btn is set to "Right"
      break;                                      //Breaks out of btn_assignment switch statement
    case 14:                                      //If btn_assignment is equal to 14 (X)
      current_btn = "X";                          //current_btn is set to "X"
      break;                                      //Breaks out of btn_assignment switch statement
    case 15:                                      //If btn_assignment is equal to 15 (B)
      current_btn = "B";                          //current_btn is set to "B"
      break;                                      //Breaks out of btn_assignment switch statement
    case 16:                                      //If btn_assignment is equal to 16 (Y)
      current_btn = "Y";                          //current_btn is set to "Y"
      break;                                      //Breaks out of btn_assignment switch statement
    case 17:                                      //If btn_assignment is equal to 17 (A)
      current_btn = "A";                          //current_btn is set to "A"
      break;                                      //Breaks out of btn_assignment switch statement
    default:                                      //If btn_assignment is not equal to any of the above
      current_btn = "Disabled";                   //current_btn is set to "Disabled"
      break;                                      //Breaks out of btn_assignment switch statement
  }
  text << control_label << "\n\nCurrent button:\n" << current_btn
                        << "\nPress a button to assign this function to.";  //Sets label text to include control_label and current_btn

  static const char *btns[] = {"Disable", ""};          //mbox button

  lv_obj_t * disable_mbox = lv_mbox_create(lv_scr_act(), NULL);       //Creates mbox
  lv_mbox_set_text(disable_mbox, text.str().c_str());                 //Sets box text
  lv_mbox_add_btns(disable_mbox, btns, disable_button);               //Adds buttons to mbox
  lv_obj_set_size(disable_mbox, 300, 180);                 //Sets size of mbox
  lv_obj_align(disable_mbox, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);   //Aligns mbox to the center of the screen

  lv_obj_t * next_btn = lv_btn_create(lv_scr_act(), NULL);            //Creates button
  lv_obj_set_size(next_btn, 60, 60);                //Sets button size
  lv_btn_set_style(next_btn, LV_BTN_STYLE_REL, &std_btn);             //Sets style of button when not pressed
  lv_btn_set_style(next_btn, LV_BTN_STYLE_PR, &std_btn_pr);           //Sets style of button when pressed
  lv_obj_align(next_btn, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, 0, 0);  //Aligns button to the middle-right
  lv_obj_set_free_num(next_btn, 1);                 //Sets free number of button to 1
  lv_btn_set_action(next_btn, LV_BTN_ACTION_CLICK, control_nav);      //Sets button action to control_nav

  lv_obj_t * next_btn_label = lv_label_create(next_btn, NULL);        //Creates button label
  lv_label_set_style(next_btn_label, &label_style);                   //Sets style of label
  lv_label_set_text(next_btn_label, SYMBOL_RIGHT);                    //Sets text of label


  lv_obj_t * exit_btn = lv_btn_create(lv_scr_act(), NULL);            //Creates exit button
  lv_obj_set_size(exit_btn, 60, 60);                                  //Sets size of button
  lv_btn_set_style(exit_btn, LV_BTN_STYLE_REL, &std_btn);             //Sets style of button when not pressed
  lv_btn_set_style(exit_btn, LV_BTN_STYLE_PR, &std_btn_pr);           //Sets style of button when pressed
  lv_obj_align(exit_btn, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);//Aligns button to bottom left
  lv_obj_set_free_num(exit_btn, 0);                 //Sets free number of button to 0
  lv_btn_set_action(exit_btn, LV_BTN_ACTION_CLICK, nav_btn);          //Sets button action to nav_btn

  lv_obj_t * exit_btn_label = lv_label_create(exit_btn, NULL);        //Creates exit button label
  lv_label_set_style(exit_btn_label, &label_style);                   //Sets style of label
  lv_label_set_text(exit_btn_label, "Exit");        //Sets label text

  get_btn_bool = true;                              //Sets get_btn_bool (global) to true
  get_btn_task = lv_task_create(get_btn, 50, LV_TASK_PRIO_HIGH, NULL);//Creates get_btn_task (repeatedly searches for pressed button)
}

/**
 *
 * control_type_menu
 *
 * Opens the menu for changing which type of motor a button controls
 *
 */
void control_type_menu(){
  get_btn_bool = false;                             //Sets get_btn_bool to false, disabling get_btn_task

  std::string motor_types = "Drive\n";              //Begins motor_types with "Drive\n"
  motor_types += join_string(motor_type_list, '\n');//Adds motor_type_list to motor_types
  motor_types.resize(motor_types.size() - 1);       //Removes trailing '\n' from motor_types

  std::string none_str = "None\n";                  //Creates a new string that holds "None\n"
  std::string::size_type none = motor_types.find(none_str);           //Finds position of none_str in motor_types
  if(motor_types.find(none_str) != std::string::npos)                 //If none_str is in motor_types
    motor_types.erase(none, none_str.length());     //Erases none_str from motor_types

  std::string left_drive_str = "Left Drive\n";      //Creates a new string that holds "Left Drive\n"
  std::string::size_type left_drive = motor_types.find(left_drive_str);         //Finds position of left_drive_str in motor_types
  if(motor_types.find(left_drive_str) != std::string::npos)           //If left_drive_str is in motor_types
    motor_types.erase(left_drive, left_drive_str.length());           //Erase left_drive_str from motor_types

  std::string right_drive_str = "Right Drive\n";    //Creates a new string that holds "Right Drive\n"
  std::string::size_type right_drive = motor_types.find(right_drive_str);       //Finds position of right_drive_str in motor_types
  if(motor_types.find(right_drive_str) != std::string::npos)          //If right_drive_str is in motor_types
    motor_types.erase(right_drive, right_drive_str.length());         //Erase right_drive_str from motor_types

  motor_types += "\nAuto-Sort";                     //Add "\nAuto-Sort" to motor_types

  std::vector<std::string> control_types_str = split_string(motor_types, '\n'); //Creates vector of strings from motor_types
  std::vector<const char*> control_types;           //Creates empty vector of c-strings

  for(int i = 0; i < control_types_str.size(); i++) //For every string in control_types_str
    control_types.push_back(control_types_str[i].c_str());            //Push current string into control_types as a c-string

  lv_obj_t * control_type_ddlist = lv_ddlist_create(lv_scr_act(), NULL);        //Creates control type ddlist
  lv_ddlist_set_options(control_type_ddlist, motor_types.c_str());    //Sets ddlist options
  lv_ddlist_set_selected(control_type_ddlist, std::distance(control_types_str.begin(), std::find(control_types_str.begin(), control_types_str.end(), control_motor_type[button_id])));    //Sets selected option to the current control type
  lv_ddlist_set_action(control_type_ddlist, change_btn_type);         //Sets ddlist action to change_btn_type
  lv_obj_align(control_type_ddlist, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 60);  //Aligns ddlist to the top-middle of the screen

  lv_obj_t * prev_btn = lv_btn_create(lv_scr_act(), NULL);            //Creates button that returns to previous menu
  lv_obj_set_size(prev_btn, 60, 60);                //Sets size of button
  lv_btn_set_style(prev_btn, LV_BTN_STYLE_REL, &std_btn);             //Sets style of button when not pressed
  lv_btn_set_style(prev_btn, LV_BTN_STYLE_PR, &std_btn_pr);           //Sets style of button when pressed
  lv_obj_align(prev_btn, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 0, 0);   //Aligns button to the middle-left of the screen
  lv_obj_set_free_num(prev_btn, 0);                 //Sets free number of the button to 0
  lv_btn_set_action(prev_btn, LV_BTN_ACTION_CLICK, control_nav);      //Sets action of button to control_nav

  lv_obj_t * prev_btn_label = lv_label_create(prev_btn, NULL);        //Creates label for prev_btn
  lv_label_set_style(prev_btn_label, &label_style); //Sets style of label
  lv_label_set_text(prev_btn_label, SYMBOL_LEFT);   //Sets label text

  lv_obj_t * next_btn = lv_btn_create(lv_scr_act(), NULL);            //Creates a button that proceeds to the next menu
  lv_obj_set_size(next_btn, 60, 60);                //Sets size of button
  lv_btn_set_style(next_btn, LV_BTN_STYLE_REL, &std_btn);             //Sets style of button when not pressed
  lv_btn_set_style(next_btn, LV_BTN_STYLE_PR, &std_btn_pr);           //Sets style of button when pressed
  lv_obj_align(next_btn, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, 0, 0);  //Aligns button to the middle-right
  lv_obj_set_free_num(next_btn, 1);                 //Sets free number of button to 1
  lv_btn_set_action(next_btn, LV_BTN_ACTION_CLICK, control_nav);      //Sets action of button to control_nav

  lv_obj_t * next_btn_label = lv_label_create(next_btn, NULL);        //Creates label for next_btn
  lv_label_set_style(next_btn_label, &label_style); //Sets style of label
  lv_label_set_text(next_btn_label, SYMBOL_RIGHT);  //Sets label text
}

/**
 *
 * change_p_loop_status
 *
 * Enables p-loop controls for the selected motor
 *
 * @param cb is the checkbox to be pressed
 */
static lv_res_t change_p_loop_status(lv_obj_t * cb){
  if(control_p_degrees[button_id] == 0){            //If p-loop degrees for selected motor is 0
    control_p_degrees[button_id] = 360;             //Changes p-loop degrees for selected motor to 360
  }
  return LV_RES_OK;                                 //Returns LV_RES_OK (cb is not deleted)
}

/**
 *
 * apply_control_voltage
 *
 * Changes what voltage the selected button runs the motor at
 *
 * @param kb is the keyboard being used
 */
static lv_res_t apply_control_voltage(lv_obj_t * kb){
  std::string temp_speed(lv_ta_get_text(ta));       //Creates string that holds the text in the text area
  control_speed[button_id] = std::stoi(temp_speed); //Converts temp_string to an integer and stores it in the selected button configuration
  if(control_speed[button_id] < 0){                 //If voltage is less than 0
    control_speed[button_id] *= -1;                 //Make voltage positive
    control_rev[button_id] = true;                  //Make the button run in reverse
  }
  config_update();                                  //Updates configuration file
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  control_params_menu_one();                        //Returns to button configuration menu
  return LV_RES_INV;                                //Returns LV_RES_INV (kb is deleted)
}

/**
 *
 * set_control_voltage
 *
 * Opens the menu for changing what voltage the selected button runs the motor
 * at
 *
 * @param ta is the text area being typed on
 */
static lv_res_t set_control_voltage(lv_obj_t * ta){
  terminal_id = 5;                                  //Sets terminal_id (global) to 5

  lv_obj_clean(lv_scr_act());                       //Clears the screen

  static const char *keys[] = {"1", "2", "3", "\n",
                               "4", "5", "6", "\n",
                               "7", "8", "9", "\n",
                               "-", "0", "Bksp", "\n",
                               SYMBOL_LEFT, SYMBOL_OK, SYMBOL_RIGHT, ""};       //Creates keys that are displayed by kb

  ta = lv_ta_create(lv_scr_act(), NULL);            //Creates text area
  lv_obj_t * kb = lv_kb_create(lv_scr_act(), NULL); //Creates keyboard

  lv_obj_set_size(ta, LV_HOR_RES / 5, LV_VER_RES / 6);                //Sets size of text area
  lv_obj_set_size(kb, LV_HOR_RES / 2, LV_VER_RES / 1.3);              //Sets size of keyboard

  lv_obj_align(ta, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 10);         //Aligns text area to the top-middle
  lv_obj_align(kb, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, 0);       //Aligns keyboard to the bottom-middle

  lv_kb_set_map(kb, keys);                          //Sets keys on the keyboard

  lv_kb_set_style(kb, LV_KB_STYLE_BG, &kb_bg_style);                  //Sets background style of keyboard
  lv_kb_set_style(kb, LV_KB_STYLE_BTN_REL, &kb_style);                //Sets style of keyboard when not pressed
  lv_kb_set_style(kb, LV_KB_STYLE_BTN_PR, &kb_style_pr);              //Sets style of keyboard when pressed

  lv_ta_set_one_line(ta, true);                 //Sets the text area to only accept one line of text

  lv_ta_set_text(ta, "");                       //Empties text area

  lv_kb_set_ta(kb, ta);                         //Sets keyboard input to go to the text area

  lv_kb_set_ok_action(kb, apply_control_voltage);                     //Sets keyboard action to apply_control_voltage

  std::cout << "You can enter a speed (in mV, 0 to 12000) here:" << std::endl;    //Prints a message to the terrminal indicating that you can enter a value
  pros::Task get_terminal_task(get_terminal);       //Reads from the terminal

  return LV_RES_INV;                                //Returns LV_RES_INV (ta is deleted)
}

/**
 *
 * rev_control
 *
 * Reverses the voltage that the button runs the motor at
 *
 * @param cb is the checkbox to be pressed
 */
static lv_res_t rev_control(lv_obj_t * cb){
  control_rev[button_id] = !control_rev[button_id]; //Reverses the selected value of control_rev
  config_update();                                  //Updates the configuration file
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  control_params_menu_one();                        //Returns to the control configuration menu
  return LV_RES_INV;                                //Return LV_RES_INV (cb is deleted)
}

/**
 *
 * control_params_one
 *
 * Opens the first menu that edits button parameters
 *
 */
void control_params_menu_one(){
  if(control_p_speed[button_id] == 0){              //If the selected p-loop speed is 0 (p-loop is disabled)
    lv_obj_t * speed_ta = lv_ta_create(lv_scr_act(), NULL);           //Creates text area for speed
    lv_obj_set_size(speed_ta, LV_HOR_RES / 5, LV_VER_RES / 6);        //Sets size of text area
    lv_obj_align(speed_ta, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);      //Aligns text area to the center of the screen
    lv_ta_set_one_line(speed_ta, true);             //Sets the text area to only accept one line of text
    lv_ta_set_text(speed_ta, std::to_string(control_speed[button_id]).c_str()); //Sets text area text to the current voltage of the selected motor
    lv_ta_set_action(speed_ta, set_control_voltage);                  //Sets text area action to set_control_voltage

    lv_obj_t * speed_label = lv_label_create(lv_scr_act(), NULL);     //Creates label for speed text area
    lv_label_set_style(speed_label, &label_style);  //Sets label style
    lv_label_set_text(speed_label, "Voltage:");     //Sets label text
    lv_obj_align(speed_label, speed_ta, LV_ALIGN_CENTER, 0, -40);     //Aligns label with the center of the text area

    lv_obj_t * rev_cb = lv_cb_create(lv_scr_act(), NULL);             //Creates checkbox that reverses voltage
    lv_cb_set_text(rev_cb, "Reversed?");            //Sets text of checkbox label
    lv_cb_set_checked(rev_cb, control_rev[button_id]);                //Checks checkbox if reversed, sets to not checked otherwise
    lv_obj_align(rev_cb, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, -30, -30);     //Aligns checkbox with the bottom-right of the screen
    lv_cb_set_action(rev_cb, rev_control);          //Sets checkbox action to rev_control

    lv_obj_t * prev_btn = lv_btn_create(lv_scr_act(), NULL);          //Creates button that returns to previous menu
    lv_obj_set_size(prev_btn, 60, 60);              //Sets size of button
    lv_btn_set_style(prev_btn, LV_BTN_STYLE_REL, &std_btn);           //Sets style of button when not pressed
    lv_btn_set_style(prev_btn, LV_BTN_STYLE_PR, &std_btn_pr);         //Sets style of button when pressed
    lv_obj_align(prev_btn, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 0, 0); //Aligns the button with the middle-left of the screen
    lv_obj_set_free_num(prev_btn, 0);               //Sets free number of button to 0
    lv_btn_set_action(prev_btn, LV_BTN_ACTION_CLICK, control_nav);    //Sets button action to control_nav

    lv_obj_t * prev_btn_label = lv_label_create(prev_btn, NULL);      //Creates label for prev_btn
    lv_label_set_style(prev_btn_label, &label_style);                 //Sets style of the label
    lv_label_set_text(prev_btn_label, SYMBOL_LEFT);                   //Sets label text
  }
}

/**
 *
 * auton_select_menu
 *
 * Allows the user to select whether the selected autonomous routine follows
 * the standard path or if the user wants to create a custom autonomous path
 *
 * @param btn is the button that is being pressed
 */
static lv_res_t auton_select_menu(lv_obj_t * btn){

  prev = "Auton";                                   //The previous screen was the autonomous menu

  auton_id = lv_obj_get_free_num(btn);              //Gets free number of button, stores it in auton_id (globals.cpp)
  config_update();                                  //Updates config file
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  static const char * menu_btns[] = {"Standard\nPath", "Custom\nPath", "\n",
                                     "Exit", ""};   //Creates button matrix map

  lv_obj_t * menu = lv_btnm_create(lv_scr_act(), NULL);                 //Creates menu

  lv_btnm_set_map(menu, menu_btns);                 //Adds the button matrix buttons to the menu
  lv_obj_set_size(menu, LV_HOR_RES - 40, LV_VER_RES - 40);              //Resizes the menu
  lv_obj_align(menu, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);              //Aligns the menu to the center of the screen
  lv_btnm_set_style(menu, LV_BTNM_STYLE_BG, &btn_bg);                   //Sets the background style of the button matrix
  lv_btnm_set_style(menu, LV_BTNM_STYLE_BTN_REL, &std_btn);             //Sets the style of the button matrix when the button is not pressed
  lv_btnm_set_style(menu, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);           //Sets the style of the button matrix when the button is pressed
  lv_btnm_set_action(menu, nav_btnm);               //Sets the button matrix action to nav_btnm
  return LV_RES_INV;                                //Returns LV_RES_INV (the button is deleted)
}

/**
 *
 * motion_profiling_switch
 *
 * Callback function for the motion profiling switch in the path selector, turns
 * motion profiling on and off
 *
 * @param sw is the switch to be pressed.
 */
static lv_res_t motion_profiling_switch(lv_obj_t * sw){
  if(pros::battery::get_capacity() < 50 && battery_override == false && motion_profiling == false){  //If the battery level is over 50% and battery_override (global) is false and motion_profiling is false

    std::string msg = SYMBOL_WARNING "WARNING:" SYMBOL_WARNING"\n"
                      "Battery level is under 50%.\n"
                      "Motion profiling will not be accurate.\n"
                      "Would you like to use motion profiling anyway?"; //mbox text

    static const char * btns[] = {"\221No","\221Yes", ""};              //mbox buttons

    battery_override_mbox = lv_mbox_create(lv_scr_act(), NULL);         //Creates battery_override_mbox
    lv_mbox_set_text(battery_override_mbox, msg.c_str());               //Sets mbox text
    lv_mbox_add_btns(battery_override_mbox, btns, battery_override_switch);     //Adds buttons to mbox
  }
  else{
    motion_profiling = !motion_profiling;         //Inverts the value of motion_profiling (globals.cpp)
    if(motion_profiling == true){                 //If motion_profiling is true
      path_vec.push_back("mp");                   //Adds motion profiling flag to path_vec
      lv_sw_on(sw);                               //Turns switch on
    }
    else{                                         //If motion_profiling is false
      path_vec.push_back("nmp");                  //Adds flag to turn off motion profiling to path_vec
      lv_sw_off(sw);                              //Turns switch off
    }
  }
  return LV_RES_OK;                               //Returns LV_RES_OK (the switch is not deleted)
}


/**
 *
 * battery_override_switch
 *
 * Callback function for the mbox in motion_profiling_switch, allows the user to
 * use motion profiling during autonomous when the battery is under 50%
 *
 * @param mbox is the message box displayed on the screen
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t battery_override_switch(lv_obj_t * mbox, const char * txt){
  if(strcmp(txt, "Yes") == 0){                    //If the pressed button says "Yes"
    battery_override = true;                      //battery_override (global) is equal to true
    motion_profiling_switch(motion_profiling_sw); //Calls motion_profiling_switch, turning on motion profiling
  }
  lv_obj_del(battery_override_mbox);              //Deletes battery_override_mbox
}

/**
 *
 * name_auton
 *
 * Allows the user to name the selected autonomous preset.
 *
 * @param btn is the button that is being pressed
 */
static lv_res_t name_auton(lv_obj_t * mbox, const char * txt){
  terminal_id = 3;                                  //Sets terminal_id (global) to 3

  lv_obj_clean(lv_scr_act());
  if(strcmp(txt, "Yes") == 0){                      //Checks if the pressed button says "Yes"
    lv_obj_t * kb = lv_kb_create(lv_scr_act(), NULL);                   //Creates keyboard
    ta = lv_ta_create(lv_scr_act(), NULL);          //Creates text area

    lv_obj_set_size(kb, LV_HOR_RES - 10, LV_VER_RES - 60);              //Sets size of keyboard
    lv_obj_align(kb, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, 0);       //Aligns keyboard to bottom middle of the screen
    lv_kb_set_style(kb, LV_KB_STYLE_BG, &kb_bg_style);                  //Sets the background style of the keyboard
    lv_kb_set_style(kb, LV_KB_STYLE_BTN_REL, &kb_style);                //Sets the style of a key when pressed
    lv_kb_set_style(kb, LV_KB_STYLE_BTN_PR, &kb_style_pr);              //Sets the style of a key when not pressed
    lv_kb_set_ta(kb, ta);                           //Assigns the keyboard to the text area
    lv_kb_set_hide_action(kb, clear_ta);            //Sets the text area to be cleared when the hide button on the keyboard is pressed
    lv_kb_set_ok_action(kb, save_confirm);          //Sets the autonomous to be saved when the OK button on the keyboard is pressed

    lv_obj_set_size(ta, LV_HOR_RES - 10, 30);       //Sets the size of the text area
    lv_obj_align(ta, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 5);        //Aligns the text area to the top middle
    lv_ta_set_one_line(ta, true);                   //Sets the text area to only accept one line of text
    lv_ta_set_text(ta, "");          //Sets the default text of the text area

    std::cout << "You can enter an auton name here:" << std::endl;    //Prints message to terminal, indicating that you can enter a name for the autonomous
    pros::Task get_terminal_task(get_terminal);     //Reads from the terminal
  }
  else                                              //If the pressed button does not say "Yes"
    auton_list();                                   //Loads the autonomous preset list
  return LV_RES_INV;                                //Returns LV_RES_INV (the mbox is deleted)
}

/**
 *
 * clear_ta
 *
 * Clears the text area associated with the keyboard
 *
 * @param kb is the keyboard displayed on the screen.
 */
static lv_res_t clear_ta(lv_obj_t * kb){
  lv_ta_set_text(ta, "");                         //Empties the text area
  return LV_RES_OK;                               //Returns LV_RES_OK (the keyboard is not deleted)
}

/**
 *
 * save_init
 *
 * Sets saveload (global) to "save" and loads the auton preset list, allowing the
 * user to save over a preset.
 *
 * @param btn is the button being pressed.
 */
static lv_res_t save_init(lv_obj_t * btn){
  saveload = "save";                                //Sets saveload (global) to "save"
  auton_list();                                     //Loads the autonomous preset list in save mode
  return LV_RES_INV;                                //Returns LV_RES_INV (the button is deleted)
}

/**
 *
 * save_confirm
 *
 * Displays a menu asking the user if they want to set the saved autonomous
 * routine as the current autonomous routine.
 *
 * @param kb is the keyboard displayed on the screen.
 */
static lv_res_t save_confirm(lv_obj_t * kb){
  static const char * btns[] = {"\221No", "\221Yes", ""};             //Buttons for mbox
  std::stringstream mbox_label;                                       //Initialization of text for mbox
  mbox_label << "Would you like to set\n"
             << lv_ta_get_text(ta) << " as the current autonomous?";  //Text for mbox

  lv_obj_t * save_confirm_mbox = lv_mbox_create(lv_scr_act(), NULL);  //Creates mbox
  lv_mbox_set_text(save_confirm_mbox, mbox_label.str().c_str());      //Sets mbox text
  lv_mbox_add_btns(save_confirm_mbox, btns, save_auton);              //Adds buttons to mbox
  lv_obj_set_size(save_confirm_mbox, 200, 200);     //Sets size of mbox
  lv_obj_align(save_confirm_mbox, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);//Aligns mbox to the center of the screen
  return LV_RES_OK;                                 //Returns LV_RES_OK (the keyboard is not deleted)
}

/**
 *
 * save_menu
 *
 * Displays an mbox that confirms whether or not the user wants to overwrite the
 * selected autonomous preset.
 *
 * @param btn is the button to be pressed.
 */
static lv_res_t save_menu(lv_obj_t * btn){

  auton_preset_id = lv_obj_get_free_num(btn);       //Stores the free number of the button in auton_preset_id (global)

  static const char * btns[] = {"\221Yes", "\221No", ""};             //Buttons for mbox

  std::stringstream mbox_text;                      //Initialization of text for mbox
  mbox_text << "Overwrite\n"
            << auton_labels[auton_preset_id] << "?";//Text for mbox

  lv_obj_t * mbox = lv_mbox_create(lv_scr_act(), NULL);               //Creates mbox
  lv_mbox_set_text(mbox, mbox_text.str().c_str());  //Sets mbox text
  lv_mbox_add_btns(mbox, btns, name_auton);         //Adds buttons to mbox
  lv_obj_set_size(mbox, 200, 200);                  //Sets size of mbox
  lv_obj_align(mbox, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);            //Aligns mbox to the center of the screen
  return LV_RES_INV;                                //Returns LV_RES_INV (the button is deleted)
}

/**
 *
 * save_auton
 *
 * Saves the created autonomous preset to the selected slot, then returns to the
 * main menu.
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t save_auton(lv_obj_t * mbox, const char * txt){
  path_vec.push_back("end");                        //Adds "end" to path_vec, denoting the end of the path
  custom_paths[auton_preset_id] = join_string(path_vec, ':');              //Sets the selected custom path to the joined string of path_vec
  auton_labels[auton_preset_id] = lv_ta_get_text(ta);                 //Sets the selected autonomous label to the text in the text area
  if(auton_id == 1 || auton_id == 2)                //If the auton starts on the red side
    alliance_color = "red";                         //Sets alliance_color to "red"
  else                                              //If the auton starts on the blue side
    alliance_color = "blue";                        //Sets alliance_color to "blue"
  if(strcmp(txt, "Yes") == 0)                       //Checks if the pressed button says "Yes"
    auton_id = auton_preset_id + 6;                 //Updates auton_id (globals.cpp) to be auton_preset_id (global) + 6 (first 6 indices are reserved)
  config_update();                                  //Updates the config file
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  main_menu(NULL);                                  //Loads the main menu
  return LV_RES_INV;                                //Returns LV_RES_INV (the mbox is deleted)
}

/**
 *
 * load_init
 *
 * Sets saveload (global) to "load" and loads the auton preset list, allowing the
 * user to load a preset.
 *
 * @param btn is the button being pressed.
 */
static lv_res_t load_init(lv_obj_t * btn){
  saveload = "load";                                //Sets saveload (global) to "load"
  auton_list();                                     //Loads the autonomous preset list in load mode
  return LV_RES_INV;                                //Returns LV_RES_INV (the button is deleted)
}

/**
 *
 * load_menu
 *
 * Displays an mbox that confirms whether or not the user wants to load the
 * selected autonomous preset.
 *
 * @param btn is the button to be pressed.
 */
static lv_res_t load_menu(lv_obj_t * btn){
 lv_obj_clean(lv_scr_act());                        //Clears the screen

 auton_preset_id = lv_obj_get_free_num(btn);        //Stores the free number of the button in auton_preset_id (global)

 static const char * buttons[] = {"\221Yes", "\221No", ""};           //mbox buttons

 lv_obj_t * mbox = lv_mbox_create(lv_scr_act(), NULL);                //Creates mbox
 lv_obj_set_size(mbox, 200, 200);                   //Sets size of mbox
 lv_obj_align(mbox, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);             //Aligns mbox to the center of the screen
 lv_mbox_set_text(mbox, "Load this autonomous preset?");              //Sets the text of the mbox
 lv_mbox_add_btns(mbox, buttons, load_auton);       //Adds buttons to the mbox
 return LV_RES_INV;                                 //Returns LV_RES_INV (the button is deleted)
}

/**
 *
 * load_auton
 *
 * Loads the autonomous preset stored in the selected slot, then returns to the
 * main menu.
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t load_auton(lv_obj_t* mbox, const char * txt){
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  if(strcmp(txt, "Yes") == 0){                      //Checks if the pressed battery says "Yes"
    auton_id = auton_preset_id + 6;                 //Updates auton_id (globals.cpp) to be auton_preset_id (global) + 6 (first 6 indices are reserved)
    config_update();                                //Updates the config file
    config_read();                                  //Reads the config file
    main_menu(NULL);                                //Loads the main menu
  }
  else                                              //If the pressed button does not say "Yes"
    auton_list();                                   //Opens the autonomous preset list
  return LV_RES_INV;                                //Returns LV_RES_INV (the mbox is deleted)
}

/**
 *
 * delete_mbox
 *
 * Reloads the screen and opens the path selector menu.
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t delete_mbox(lv_obj_t * mbox, const char * txt){
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  custom_path_menu();                               //Opens the path selector menu
  return LV_RES_INV;                                //Returns LV_RES_INV (the mbox is deleted)
}

/**
 *
 * disabled_element
 *
 * A callback function for disabled elements, alerting the user that the
 * selected element is disabled (on the opposite side of the field).
 *
 * @param btn is the button to be pressed.
 */
static lv_res_t disabled_element(lv_obj_t * btn){

  static const char * btns[] = {"OK", ""};          //mbox button

  lv_obj_t * mbox = lv_mbox_create(lv_scr_act(), NULL);               //Creates the mbox
  lv_obj_set_size(mbox, 200, 200);                                    //Sets the size of the mbox
  lv_mbox_set_text(mbox, "You cannot reach this field element\n"
                         "because it is on the other side of the\n"
                         "field.");                 //Sets the text of the mbox
  lv_mbox_add_btns(mbox, btns, delete_mbox);        //Adds buttons to the mbox
  lv_obj_align(mbox, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);            //Aligns the mbox to the center of the screen

  return LV_RES_OK;                                 //Returns LV_RES_OK (the button is not deleted)
}

/**
 *
 * tile_relocate
 *
 * Push back a tile to go to into path_vec, and aligns the robot with the
 * pressed tile in the path selector menu.
 *
 * @param btn is the tile to be pressed
 */
static lv_res_t tile_relocate(lv_obj_t * btn){
  int id = lv_obj_get_free_num(btn);                //Stores the free number of the button into "id"
  switch(id){                                       //Tests id
    case 1:                                         //If id = 1 (upper left tile)
      location << "to_upper_left";                  //Adds "to_upper_left" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "upper_left_";                    //Adds "upper_left_" to location (global)
      break;                                        //Breaks out of id switch statement
    case 2:                                         //If id = 2 (lower left tile)
      location << "to_lower_left";                  //Adds "to_lower_left" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "lower_left_";                    //Adds "lower_left_" to location (global)
      break;                                        //Breaks out of id switch statement
    case 3:                                         //If id = 3 (upper right tile)
      location << "to_upper_right";                 //Adds "to_upper_right" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "upper_right_";                   //Adds "upper_right_" to location (global)
      break;                                        //Breaks out of id switch statement
    case 4:                                         //If id = 4
      location << "to_lower_right";                 //Adds "to_lower_right" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "lower_right_";                   //Adds "lower_right_" to location (global)
      break;                                        //Breaks out of id switch statement
    default:                                        //If id is none of the above
      return LV_RES_OK;                             //Returns LV_RES_OK (does nothing)
  }
  lv_obj_align(robot, btn, LV_ALIGN_CENTER, 0, 0);  //Aligns robot to the tile
  return LV_RES_OK;                                 //Returns LV_RES_OK (the button is not deleted)
}

/**
 *
 * ball_relocate
 *
 * Push back a ball to go to into path_vec, and aligns the robot with the
 * pressed ball in the path selector menu.
 *
 * @param btn is the ball to be pressed
 */
static lv_res_t ball_relocate(lv_obj_t * btn){
  int id = lv_obj_get_free_num(btn);                //Stores the free number of the button into "id"
  switch(id){                                       //Tests id
    case 1:                                         //If id = 1
      robot_data.balls.insert(robot_data.balls.begin(), 'r');       //Inserts red ball into the beginning of robot_data (global, see tile.hpp)
      location << "to_bottom_ball";                 //Adds "to_bottom_ball" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "bottom_ball_";                   //Adds "bottom_ball_" to location (global)
      break;                                        //Breaks out of id switch statement
    case 2:                                         //If id = 2
      robot_data.balls.insert(robot_data.balls.begin(), 'r');       //Inserts red ball into the beginning of robot_data (global, see tile.hpp)
      location << "to_mid_ball";                    //Adds "to_mid_ball" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "mid_ball_";                      //Adds "mid_ball_" to location
      break;                                        //Breaks out of id switch statement
    case 3:                                         //If id = 3
      robot_data.balls.insert(robot_data.balls.begin(), 'r');       //Inserts red ball into the beginning of robot_data (global, see tile.hpp)
      location << "to_top_ball";                    //Add "to_top_ball" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "top_ball_";                      //Adds "top_ball_" to location (global)
      break;                                        //Breaks out of id switch statement
    case 4:                                         //If id = 4
      robot_data.balls.insert(robot_data.balls.begin(), 'b');       //Inserts blue ball into the beginning of robot_data (global, see tile.hpp)
      location << "to_bottom_ball";                 //Adds "to_bottom_ball" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "bottom_ball_";                   //Adds "bottom_ball_" to location (global)
      break;                                        //Breaks out of id switch statement
    case 5:                                         //If id = 5
      robot_data.balls.insert(robot_data.balls.begin(), 'b');       //Inserts blue ball into the beginning of robot_data (global, see tile.hpp)
      location << "to_middle_ball";                 //Adds "to_middle_ball" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "middle_ball_";                   //Adds "middle_ball_" to location (global)
      break;                                        //Breaks out of id switch statement
    case 6:                                         //If id = 6
      robot_data.balls.insert(robot_data.balls.begin(), 'b');       //Inserts blue ball into the beginning of robot_data (global, see tile.hpp)
      location << "to_top_ball";                    //Adds "to_top_ball" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "top_ball_";                      //Adds "top_ball_" to location (global)
      break;                                        //Breaks out of id switch statement
    default:                                        //If id is none of the above
      return LV_RES_OK;                             //Return LV_RES_OK (does nothing)
  }
  lv_obj_align(robot, btn, LV_ALIGN_CENTER, 0, 0);  //Aligns the robot with the ball
  lv_obj_set_hidden(btn, true);                     //Hides the ball (since it was collected)
  return LV_RES_OK;                                 //Returns LV_RES_OK (the button was not deleted)
}

/**
 *
 * tower_menu_nav
 *
 * Callback function for the button matrix created by tower_menu, handles adding
 * tower instructions to path_vec
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t tower_menu_nav(lv_obj_t * btnm, const char * txt){
  int red_ball_count = 0;                           //Counts number of red balls in robot and tower
  int blue_ball_count = 0;                          //Counts number of blue balls in robot and tower
  if(strcmp(txt, "Cancel") == 0){                   //If the pressed button says "Cancel"
    lv_obj_del(btnm);                               //Delete the tower menu
    return LV_RES_INV;                              //Return LV_RES_INV (the button matrix is deleted)
  }
  switch(tower_id){
    case 0:                                         //If tower_id = 0
      lv_obj_align(robot, towers_obj[4], LV_ALIGN_CENTER, 0, 0);      //Aligns the robot with the center tower
      location << "to_middle";                      //Adds "to_middle" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "middle_";                        //Adds "middle_" to location (global)
      break;                                        //Breaks out of tower_id switch statement
    case 1:                                         //If tower_id = 1
    lv_obj_align(robot, towers_obj[0], LV_ALIGN_CENTER, 0, 0);        //Aligns the robot with the fifth tower
      location << "to_bottom_left_tower";                //Adds "to_bottom_left_tower" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "bottom_left_tower_";                  //Adds "bottom_left_tower_" to location (global)
      break;                                        //Breaks out of tower_id switch statement
    case 2:                                         //If tower_id = 2
      lv_obj_align(robot, towers_obj[3], LV_ALIGN_CENTER, 0, 0);      //Aligns the robot with the forth tower
      location << "to_middle_left_tower";                //Adds "to_middle_left_tower" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "middle_left_tower_";                  //Adds "middle_left_tower_" to location (global)
      break;                                        //Breaks out of tower_id switch statement
    case 3:                                         //If tower_id = 3
      lv_obj_align(robot, towers_obj[6], LV_ALIGN_CENTER, 0, 0);      //Aligns the robot with the seventh tower
      location << "to_top_left_tower";                   //Adds "to_top_left_tower" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "top_left_tower_";                     //Adds "top_left_tower_" to location (global)
      break;                                        //Breaks out of tower_id switch statement
    case 4:                                         //If tower_id = 4
      lv_obj_align(robot, towers_obj[2], LV_ALIGN_CENTER, 0, 0);      //Aligns the robot with the third tower
      location << "to_bottom_right_tower";                //Adds "to_bottom_right_tower" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "bottom_right_tower_";                  //Adds "bottom_right_tower_" to location (global)
      break;                                        //Breaks out of tower_id switch statement
    case 5:                                         //If tower_id = 5
      lv_obj_align(robot, towers_obj[5], LV_ALIGN_CENTER, 0, 0);      //Aligns the robot with the sixth tower
      location << "to_middle_right_tower";                //Adds "to_middle_right_tower" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "middle_right_tower_";                  //Adds "middle_right_tower" to location (global)
      break;                                        //Breaks out of tower_id switch statement
    case 6:                                         //If tower_id = 6
      lv_obj_align(robot, towers_obj[8], LV_ALIGN_CENTER, 0, 0);      //Aligns the robot with the ninth tower
      location << "to_top_right_tower";                   //Adds "to_top_right_tower" to location (global)
      path_vec.push_back(location.str());           //Pushes location (global) into path_vec
      location.str("");                             //Clears location (global)
      location << "top_right_tower_";                     //Adds "top_right_tower" to location (global)
      break;                                        //Breaks out of tower_id switch statement
    default:                                        //If tower_id is none of the above
      return LV_RES_OK;                             //Returns LV_RES_OK (does nothing)
  }
  if(strcmp(txt, "Deposit\nBall") == 0){
    path_vec.push_back("db");                       //Pushes control suffix "db" (deposit ball) into path_vec
    switch(tower_id){                               //Tests tower_id
      case 0:                                       //If tower_id = 0
        towers[4].balls.push_back(robot_data.balls.back());           //Push ball in the back of the robot into the back of the tower
        robot_data.balls.pop_back();                //Delete last ball in robot
        break;
      case 1:                                       //If tower_id = 1
        towers[0].balls.push_back(robot_data.balls.back());           //Push ball in the back of the robot into the back of the tower
        robot_data.balls.pop_back();                //Delete last ball in robot
        break;
      case 2:
        towers[3].balls.push_back(robot_data.balls.back());           //Push ball in the back of the robot into the back of the tower
        robot_data.balls.pop_back();                //Delete last ball in robot
        break;
      case 3:
        towers[6].balls.push_back(robot_data.balls.back());           //Push ball in the back of the robot into the back of the tower
        robot_data.balls.pop_back();                //Delete last ball in robot
        break;
      case 4:
        towers[2].balls.push_back(robot_data.balls.back());           //Push ball in the back of the robot into the back of the tower
        robot_data.balls.pop_back();                //Delete last ball in robot
        break;
      case 5:
        towers[5].balls.push_back(robot_data.balls.back());           //Push ball in the back of the robot into the back of the tower
        robot_data.balls.pop_back();                //Delete last ball in robot
        break;
      case 6:
        towers[8].balls.push_back(robot_data.balls.back());           //Push ball in the back of the robot into the back of the tower
        robot_data.balls.pop_back();                //Delete last ball in robot
        break;
      case 7:
        towers[1].balls.push_back(robot_data.balls.back());           //Push ball in the back of the robot into the back of the tower
        robot_data.balls.pop_back();                //Delete last ball in robot
        break;
      case 8:
        towers[7].balls.push_back(robot_data.balls.back());           //Push ball in the back of the robot into the back of the tower
        robot_data.balls.pop_back();                //Delete last ball in robot
        break;
    }
  }
  else if(strcmp(txt, "Cycle") == 0){
    path_vec.push_back("ch");
  }
  refresh_towers();                                 //Updates tower graphics
  lv_obj_del(btnm);                                 //Deletes tower menu
  return LV_RES_INV;                                //Returns LV_RES_INV (the button matrix is deleted)
}

/**
 *
 * tower_menu
 *
 * Opens a menu for tower operations
 *
 * @param btn is the button to be pressed
 */
 static lv_res_t tower_menu(lv_obj_t * btn){
  tower_id = lv_obj_get_free_num(btn);              //Stores the free number of the button into tower_id (global)

  static const char * btnm_map[] = {"Deposit\nBall", "Cycle", "\n",
                                   "Cancel", ""};   //The buttons on the tower menu

  lv_obj_t * tower_menu_m = lv_btnm_create(lv_scr_act(), NULL);//Creates the tower menu
  lv_btnm_set_map(tower_menu_m, btnm_map);          //Adds the buttons to the tower menu
  lv_obj_set_size(tower_menu_m, LV_HOR_RES - 100, LV_VER_RES - 20);   //Sets the size of the tower menu
  lv_obj_align(tower_menu_m, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);    //Aligns the tower menu to the center of the screen
  lv_btnm_set_style(tower_menu_m, LV_BTNM_STYLE_BG, &btn_bg);         //Sets the background style of the button matrix
  lv_btnm_set_style(tower_menu_m, LV_BTNM_STYLE_BTN_REL, &std_btn);   //Sets the style of the button matrix when a button is not pressed
  lv_btnm_set_style(tower_menu_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr); //Sets the style of the button matrix when a button is pressed
  lv_btnm_set_action(tower_menu_m, tower_menu_nav); //Sets the tower menu action to tower_menu_nav
  return LV_RES_OK;                                 //Returns LV_RES_OK (the button is not deleted)
}


/**
 *
 * confirm_skills
 *
 * A callback function for the mbox in skills_error, which either sets auton_id
 * to 5 (skills) or does nothing.
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t confirm_skills(lv_obj_t * mbox, const char * txt){
  if(strcmp(txt, "Yes") == 0){                       //Checks if the pressed button says "Yes"
    auton_id = 5;                                    //Sets auton_id to 5 (skills)
    config_update();                                 //Updates the config file
  }
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  main_menu(NULL);                                  //Loads the main menu
  return LV_RES_INV;                                //Returns LV_RES_INV (the mbox is deleted)
}

/**
 *
 * null
 *
 * Does nothing
 *
 */
void null(void * ign){}

/**
 *
 * style_init
 *
 * Defines the styles for all LVGL objects
 */
void style_init(){
    /**************Screen Style**************/
    lv_style_copy(&scr, &lv_style_scr);
    scr.body.main_color = LV_COLOR_MAKE(0x0f, 0x0f, 0x0f);
    scr.body.grad_color = LV_COLOR_MAKE(0x00, 0x00, 0x00);
    lv_obj_set_style(lv_scr_act(), &scr);
    /****************************************/

    /**************Button Style**************/
    lv_style_copy(&btn_bg, &lv_style_transp);
    btn_bg.body.padding.hor = 5;
    btn_bg.body.padding.ver = 5;

    lv_style_copy(&std_btn, &lv_style_btn_rel);
    std_btn.body.main_color = LV_COLOR_MAKE(0x07, 0x00, 0x93);
    std_btn.body.grad_color = LV_COLOR_MAKE(0x03, 0x22, 0x44);
    std_btn.body.border.color = LV_COLOR_GRAY;

    lv_style_copy(&std_btn_pr, &lv_style_btn_pr);
    std_btn_pr.body.main_color = LV_COLOR_MAKE(0x03, 0x22, 0x44);
    std_btn_pr.body.grad_color = LV_COLOR_MAKE(0x07, 0x00, 0x93);
    std_btn_pr.body.border.color = LV_COLOR_MAKE(0x40, 0x40, 0x40);
    std_btn_pr.text.color = LV_COLOR_GRAY;
    /****************************************/

    /**************Switch Style**************/
    lv_style_copy(&switch_bg, &lv_style_pretty);
    switch_bg.body.padding.ver = 0;

    lv_style_copy(&switch_indic, &lv_style_pretty);
    switch_indic.body.main_color = LV_COLOR_MAKE(0x03, 0x06, 0xb0);
    switch_indic.body.grad_color = LV_COLOR_MAKE(0x03, 0x06, 0xb0);
    switch_indic.body.padding.ver = 0;
    switch_indic.body.padding.hor = 0;

    lv_style_copy(&switch_off, &lv_style_pretty);
    switch_off.body.radius = LV_RADIUS_CIRCLE;

    lv_style_copy(&switch_on, &lv_style_pretty);
    switch_on.body.main_color = LV_COLOR_CYAN;
    switch_on.body.grad_color = LV_COLOR_BLUE;
    switch_on.body.radius = LV_RADIUS_CIRCLE;
    /****************************************/

    /**************Label Style***************/
    lv_style_copy(&label_style, &lv_style_pretty);
    label_style.text.color = LV_COLOR_HEX3(0xfff);
    /****************************************/

    /*************Keyboard Style*************/
    lv_style_copy(&kb_bg_style, &lv_style_transp);
    kb_bg_style.body.padding.hor = 0;
    kb_bg_style.body.padding.ver = 0;

    lv_style_copy(&kb_style, &lv_style_btn_rel);
    kb_style.body.main_color = LV_COLOR_MAKE(0x07, 0x00, 0x93);
    kb_style.body.grad_color = LV_COLOR_MAKE(0x03, 0x22, 0x44);
    kb_style.body.border.color = LV_COLOR_GRAY;

    lv_style_copy(&kb_style_pr, &lv_style_btn_pr);
    kb_style_pr.body.main_color = LV_COLOR_MAKE(0x03, 0x22, 0x44);
    kb_style_pr.body.grad_color = LV_COLOR_MAKE(0x07, 0x00, 0x93);
    kb_style_pr.body.border.color = LV_COLOR_MAKE(0x40, 0x40, 0x40);
    kb_style_pr.text.color = LV_COLOR_GRAY;
    /****************************************/

    /************Disabled Objects************/
    lv_style_copy(&imgbtn_disable, &lv_style_plain);
    imgbtn_disable.image.color = LV_COLOR_BLACK;
    imgbtn_disable.image.intense = LV_OPA_50;
    imgbtn_disable.text.color = LV_COLOR_HEX3(0xaaa);
    /****************************************/
}

/**
 *
 * get_terminal
 *
 * Reads a line from the terminal and stores it in curr_label
 *
 * @param ign ignore
 */
void get_terminal(void * ign){
  std::getline(std::cin, curr_label);               //Reads a line from the terminal and stores it in curr_label
  exit_terminal();                                  //Does something with curr_label
}

/**
 *
 * save_auton
 *
 * Saves the created autonomous preset to the selected slot, then returns to the
 * main menu, via the terminal.
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t save_auton_terminal(lv_obj_t * mbox, const char *txt){
  path_vec.push_back("end");                        //Adds "end" to path_vec, denoting the end of the path
  custom_paths[auton_preset_id] = join_string(path_vec, ':');              //Sets the selected custom path to the joined string of path_vec
  auton_labels[auton_preset_id] = curr_label;                 //Sets the selected autonomous label to the text in curr_label
  if(strcmp(txt, "Yes") == 0)                       //Checks if the pressed button says "Yes"
    auton_id = auton_preset_id + 6;                 //Updates auton_id (globals.cpp) to be auton_preset_id (global) + 6 (first 6 indices are reserved)
  config_update();                                  //Updates the config file
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  main_menu(NULL);                                  //Loads the main menu
  return LV_RES_INV;                                //Returns LV_RES_INV (the mbox is deleted)
}

/**
 *
 * save_confirm_terminal
 *
 * Displays a menu asking the user if they want to set the saved autonomous
 * routine as the current autonomous routine.
 *
 */
void save_confirm_terminal(){
  static const char * btns[] = {"\221No", "\221Yes", ""};             //Buttons for mbox
  std::stringstream mbox_label;                                       //Initialization of text for mbox
  mbox_label << "Would you like to set\n"
             << curr_label << " as the current autonomous?";  //Text for mbox

  lv_obj_t * save_confirm_mbox = lv_mbox_create(lv_scr_act(), NULL);  //Creates mbox
  lv_mbox_set_text(save_confirm_mbox, mbox_label.str().c_str());      //Sets mbox text
  lv_mbox_add_btns(save_confirm_mbox, btns, save_auton_terminal);              //Adds buttons to mbox
  lv_obj_set_size(save_confirm_mbox, 200, 200);     //Sets size of mbox
  lv_obj_align(save_confirm_mbox, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);//Aligns mbox to the center of the screen
}

/**
 *
 * exit_terminal
 *
 * Exits the terminal and processes curr_label based on the value of terminal_id
 *
 */
void exit_terminal(){
  switch(terminal_id){                              //Tests terminal_id
    case 1:                                         //If terminal_id = 1 (adding a motor)
      motor_labels.push_back(curr_label);           //Push curr_label into motor_labels
      motor_types.push_back("None");                //Push "None" into motor_types
      motor_ports.push_back(0);                     //Push 0 into motor_ports
      motor_reversals.push_back(false);             //Push false into motor_reversals
      config_update();                              //Updates the configuration file
      lv_obj_clean(lv_scr_act());                   //Clears the screen
      motor_select();                               //Returns to motor selection menu
      break;
    case 2:                                         //If terminal_id = 2 (adding a sensor)
      sensor_labels.push_back(curr_label);          //Push curr_label into sensor_labels
      sensor_types.push_back("None");               //Push "None" into sensor_types
      sensor_ports.push_back(0);                    //Push 0 into sensor_ports
      sensor_locations.push_back("None");           //Push "None" into sensor_locations
      config_update();                              //Updates the configuration file
      lv_obj_clean(lv_scr_act());                   //Clears the screen
      sensor_menu();                                //Returns to sensor selection menu
      break;
    case 3:                                         //If terminal_id = 3 (adding an autonomous routine)
      auton_labels[auton_preset_id] = curr_label;   //Sets auton_labels at the preset id to curr_label
      save_confirm_terminal();                      //Confirms the saving of the routine
      break;
    case 4:                                         //If terminal_id = 4 (adding a button)
      control_labels.push_back(curr_label);         //Push curr_label into ontrol_labels
      control_btns.push_back((pros::controller_digital_e_t) -1);      //Push -1 into control_btns
      control_motor_type.push_back("None");         //Push "None" into control_motor_type
      control_toggle.push_back(false);              //Push false into control_toggle
      control_rev.push_back(false);                 //Push false into control_rev
      control_speed.push_back(0);                   //Push 0 into control_speed
      control_p_speed.push_back(0);                 //Push 0 into control_p_speed
      control_p_degrees.push_back(0);               //Push 0 into control_p_degrees
      lv_obj_clean(lv_scr_act());                   //Clears the screen
      config_update();                              //Updates the configuration file
      button_mapping_menu();                        //Returns to button mapping menu
      break;
    case 5:                                         //If terminal_id = 5 (control speed setting)
      control_speed[button_id] = std::stoi(curr_label);               //curr_label is converted to an integer and stored in control_speed at button_id
      if(control_speed[button_id] < 0){             //If control_speed at button_id  < 0
        control_speed[button_id] *= -1;             //Make control_speed at button_id positive
        control_rev[button_id] = true;              //Set control_rev at button_id to true
      }
      config_update();                              //Update the configuration file
      lv_obj_clean(lv_scr_act());                   //Clear the screen
      control_params_menu_one();                    //Return to control parameter screen
      break;
  }
}

/**
 *
 * exit_screen
 *
 * Displays a screen prompting the user to exit the program
 *.
 */
void exit_screen(){
  lv_obj_clean(lv_scr_act());
  lv_obj_t * turn_off_label = lv_label_create(lv_scr_act(), NULL);  //Creates turn off label
  lv_label_set_style(turn_off_label, &label_style);                 //Sets style of turn off label
  lv_label_set_text(turn_off_label, "Please exit the program.");    //Sets text telling user to turn off the program
  lv_obj_align(turn_off_label, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);//Aligns turn off label
}


/**
 *
 * no_sdcard_menu
 *
 * Displays a menu that alerts the user that there is no microSD Card
 * inserted
 *.
 */
void no_sdcard_menu(void * ign){
  std::string error_txt = SYMBOL_WARNING SYMBOL_WARNING "WARNING!!!" SYMBOL_WARNING SYMBOL_WARNING "\n\n"
                          "No MicroSD Card has been inserted. Any configurations will have to be set manually and will not be saved.\n"
                          "Would you still like to continue?";        //Creates text for the mbox

  static const char * btns[] = {"\221Yes", "\221No", ""};             //Creates the buttons for the mbox

  lv_obj_t * error_mbox = lv_mbox_create(lv_scr_act(), NULL);         //Creates the error mbox
  lv_mbox_set_text(error_mbox, error_txt.c_str());                    //Sets the mbox text
  lv_mbox_add_btns(error_mbox, btns, load_main_menu);                 //Adds buttons to the mbox
  lv_obj_set_size(error_mbox, LV_HOR_RES / 1.5, LV_VER_RES);          //Sets the size of the mbox
  lv_obj_align(error_mbox, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);      //Aligns the mbox with the center of the screen
}

/**
 *
 * main_menu
 *
 * Displays the main menu, which allows the user to navigate to all other menus.
 */
void main_menu(void * ign){
  static const char * main_menu_c[] = { "Controls", "Motors", "Sensors", "\n",
                                        "Skills", "Auton", "" };    //Main menu buttons

  //C-strings that display which autonomous routine is being used
  static std::vector<const char*> auton_text = {"Auton: None", "Auton: Upper Red Side", "Auton: Lower Red Side", "Auton: Upper Blue Side", "Auton: Lower Blue Side", "Auton: Skills"};
  for(int i = 0; i < 15; i++){                      //For each of the 15 autonomous presets
    auton_text.push_back(auton_labels[i].c_str());  //Push auton_labels[i] into auton_text
  }
  auton_text.push_back("Custom Path");              //Push "Custom Path" into auton_text

  lv_obj_t * main_menu_m = lv_btnm_create(lv_scr_act(), NULL);        //Creates main menu button matrix
  lv_obj_t * auton_label = lv_label_create(lv_scr_act(), NULL);       //Creates label that displays the selected autonomous
  lv_btnm_set_map(main_menu_m, main_menu_c);                          //Adds buttons to main menu
  lv_label_set_text(auton_label, auton_text[auton_id]);               //Sets the text of auton_label to be auton_text with an index of auton_id
  lv_obj_align(main_menu_m, NULL, LV_ALIGN_IN_TOP_LEFT, 20, 40);      //Aligns the main menu to the top-left of the screen
  lv_obj_align(auton_label, NULL, LV_ALIGN_IN_BOTTOM_MID, 0, 0);      //Aligns the autonomous label to the bottom-middle
  lv_obj_set_size(main_menu_m, LV_HOR_RES - 40, LV_VER_RES / 1.5);    //Sets the size of the main menu
  lv_btnm_set_style(main_menu_m, LV_BTNM_STYLE_BG, &btn_bg);          //Sets the background style of the button matrix
  lv_btnm_set_style(main_menu_m, LV_BTNM_STYLE_BTN_REL, &std_btn);    //Sets the style of the button matrix when a button is not pressed
  lv_btnm_set_style(main_menu_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);  //Sets the style of the button matrix when a button is pressed
  lv_label_set_style(auton_label, &label_style);                      //Sets the style of the autonomous label
  lv_btnm_set_action(main_menu_m, nav_btnm);                          //Assigns nav_btnm to the main menu
}

/**
 *
 * control_menu
 *
 * Displays the control menu, which leads to menus that allow the user to change
 * the control scheme or button mapping.
 */
void control_menu(){
  prev = "Main Menu";                               //Sets the previous menu as the main menu

  static const char * options_c[] = {"Chassis\nControls", "Drive\nModel", "Button\nMapping", "\n",
                                     "Exit", ""};

  lv_obj_t * options_m = lv_btnm_create(lv_scr_act(), NULL);          //Creates the button menu
  lv_btnm_set_map(options_m, options_c);            //Adds the control menu buttons to the control menu
  lv_obj_set_size(options_m, LV_HOR_RES - 20, LV_VER_RES - 20);       //Sets the size of the control menu
  lv_obj_align(options_m, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);       //Aligns the control menu to the center of the screen
  lv_btnm_set_style(options_m, LV_BTNM_STYLE_BG, &btn_bg);            //Sets the background style of the button matrix
  lv_btnm_set_style(options_m, LV_BTNM_STYLE_BTN_REL, &std_btn);      //Sets the style of the button matrix when a button is not pressed
  lv_btnm_set_style(options_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);    //Sets the style of the button matrix when a button is pressed
  lv_btnm_set_action(options_m, nav_btnm);          //Assigns nav_btnm to the control menu
}

/**
 *
 * chassis_controls
 *
 * Allows the user to change the control scheme to tank, left arcade, right
 * arcade, or split arcade controls.
 */
void chassis_controls(){

  static const char * options_c[] = {"Tank", "Split Arcade", "\n",
                                     "Arcade (Left)", "Arcade (Right)", ""};      //Control scheme options

  lv_obj_t * options_m = lv_btnm_create(lv_scr_act(), NULL);          //Creates control scheme options menu
  lv_btnm_set_map(options_m, options_c);            //Adds control scheme options to the control scheme options menu
  lv_obj_set_size(options_m, LV_HOR_RES - 20, LV_VER_RES - 20);       //Sets the size of the control scheme menu
  lv_obj_align(options_m, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);       //Aligns the control scheme menu to the center of the screen
  lv_btnm_set_style(options_m, LV_BTNM_STYLE_BG, &btn_bg);            //Sets the background style of the button matrix
  lv_btnm_set_style(options_m, LV_BTNM_STYLE_BTN_REL, &std_btn);      //Sets the style of the button matrix when a button is not pressed
  lv_btnm_set_style(options_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);    //Sets the style of the button matrix when a button is pressed
  lv_btnm_set_action(options_m, nav_btnm);          //Assigns nav_btnm to the control scheme options menu
}

/**
 *
 * model_select
 *
 * Allows the user to select what control scheme they would like to use when
 * driving
 *
 */
void model_select(){

  static const char * models_c[] = {"Skid-Steer", "X-Drive", "\n",
                                    "Exit", ""};                      //Creates buttons for the button matrix

  lv_obj_t * models_m = lv_btnm_create(lv_scr_act(), NULL);           //Creates the button matrix
  lv_btnm_set_map(models_m, models_c);                                //Adds buttons to the button matrix
  lv_obj_set_size(models_m, LV_HOR_RES - 20, LV_VER_RES - 20);        //Sets size of the button matrix
  lv_obj_align(models_m, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);        //Aligns the button matrix with the center of the screen
  lv_btnm_set_style(models_m, LV_BTNM_STYLE_BG, &btn_bg);             //Sets the background style of the button matrix
  lv_btnm_set_style(models_m, LV_BTNM_STYLE_BTN_REL, &std_btn);       //Sets the style of the button matrix when not pressed
  lv_btnm_set_style(models_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);     //Sets the style of the button matrix when pressed
  lv_btnm_set_action(models_m, nav_btnm);                             //Sets the action of the button matrix to nav_btnm
}

/**
 *
 * button_mapping_menu
 *
 * Opens a menu that allows the user to change what button performs an action
 * selected from the button matrix.
 */
void button_mapping_menu(){
  std::cout << "test" << std::endl;
  prev = "Controls";                                //Sets the previous menu to the controls menu

  std::vector<const char*> controls_vec;            //Creates a vector of c-strings
  if(!control_labels.empty())                       //If control_labels is not empty
    for(int i = 0; i < (int) std::floor(control_labels.size() * 1.5); i++){     //For every ite in control_labels * 1.5
      if(((i - 2) % 3 == 0) && i > 0)               //If i - 2 is a multiple of 3 and not 0
        controls_vec.push_back("\n");               //Push '\n' into controls_vec
      else                                          //If i - 2 is not a multiple of 3 or i is 0
        controls_vec.push_back(control_labels[i - (int) i/3].c_str());//Push the current item in control_labels into controls_vec
                 //Push '\n' into controls_vec
    }
  if(controls_vec.size() % 3 != 0)                  //If the final size of controls_vec is not a multiple of 3
    controls_vec.push_back("\n");
  controls_vec.push_back("Exit");                   //Push "Exit" into controls_vec
  controls_vec.push_back("");                       //End of vector

  static const char * controls_c[50];               //Create an array of c-strings
  std::copy(controls_vec.begin(), controls_vec.end(), controls_c);    //Copy controls_vec into controls_c

  lv_obj_t * controls_m = lv_btnm_create(lv_scr_act(), NULL);         //Creates control mapping menu
  lv_btnm_set_map(controls_m, controls_c);          //Adds control mapping menu buttons to the control mapping menu
  lv_obj_set_size(controls_m, LV_HOR_RES - 40, LV_VER_RES);      //Sets the size of the control mapping menu
  lv_obj_align(controls_m, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 0, 0);      //Aligns the control mapping menu to the center of the screen
  lv_btnm_set_style(controls_m, LV_BTNM_STYLE_BG, &btn_bg);           //Sets the background style of the control mapping menu
  lv_btnm_set_style(controls_m, LV_BTNM_STYLE_BTN_REL, &std_btn);     //Sets the style of the control mapping menu when a button is not pressed
  lv_btnm_set_style(controls_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);   //Sets the style of the control mapping menu when a button is pressed
  lv_btnm_set_action(controls_m, nav_btnm);                   //Assigns button_mapper to the control mapping menu

  static const char * control_edit_c[] = {SYMBOL_PLUS, "\n",
                                          SYMBOL_MINUS, ""};          //Creates buttons for control editor

  lv_obj_t * control_edit_m = lv_btnm_create(lv_scr_act(), NULL);     //Creates control editor button matrix
  lv_btnm_set_map(control_edit_m, control_edit_c);  //Add buttons to button matrix
  lv_obj_set_size(control_edit_m, 40, LV_VER_RES);  //Set size of button matrix
  lv_obj_align(control_edit_m, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, 0, 0);      //Aligns the button matrix with the top-right of the screen
  lv_btnm_set_style(control_edit_m, LV_BTNM_STYLE_BG, &btn_bg);           //Sets the background style of the control edit menu
  lv_btnm_set_style(control_edit_m, LV_BTNM_STYLE_BTN_REL, &std_btn);     //Sets the style of the control edit menu when a button is not pressed
  lv_btnm_set_style(control_edit_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);   //Sets the style of the control edit menu when a button is pressed
  lv_btnm_set_action(control_edit_m, edit_controls);//Sets button matrix action to edit_controls
}

/**
 *
 * get_btn
 *
 * Searches for a digital input from the controller and assigns it to a task
 * based on button_id (ideally called repeatedly using a task).
 */
void get_btn(void * ign){
  if(get_btn_bool == true)                            //If the get_btn task is enabled
    for(int i = 6; i < 18; i++){                      //For every button input
      if(master.get_digital_new_press((pros::controller_digital_e_t) i)){       //If the button pressed is equal to i
        control_btns[button_id] = (pros::controller_digital_e_t) i;
        config_update();                              //Updates config file
        lv_obj_clean(lv_scr_act());                   //Clears the screen
        button_mapper();                              //Return to button mapper
      }
    }
}

/**
 *
 * add_controls
 *
 * Displays a menu prompting the user to name the new control configuration
 *
 */
void add_controls(){
  terminal_id = 4;                                  //Sets terminal_id to 4

  lv_obj_t * kb = lv_kb_create(lv_scr_act(), NULL);                   //Creates keyboard
  ta = lv_ta_create(lv_scr_act(), NULL);            //Creates text area

  lv_obj_set_size(kb, LV_HOR_RES - 10, LV_VER_RES - 60);              //Sets size of keyboard
  lv_obj_align(kb, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, 0);       //Aligns keyboard to bottom middle of the screen
  lv_kb_set_style(kb, LV_KB_STYLE_BG, &kb_bg_style);                  //Sets the background style of the keyboard
  lv_kb_set_style(kb, LV_KB_STYLE_BTN_REL, &kb_style);                //Sets the style of a key when pressed
  lv_kb_set_style(kb, LV_KB_STYLE_BTN_PR, &kb_style_pr);              //Sets the style of a key when not pressed
  lv_kb_set_ta(kb, ta);                             //Assigns the keyboard to the text area
  lv_kb_set_hide_action(kb, clear_ta);              //Sets the text area to be cleared when the hide button on the keyboard is pressed
  lv_kb_set_ok_action(kb, apply_controls);          //Sets the motor to be saved when the OK button on the keyboard is pressed

  lv_obj_set_size(ta, LV_HOR_RES - 10, 30);         //Sets the size of the text area
  lv_obj_align(ta, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 5);          //Aligns the text area to the top middle
  lv_ta_set_one_line(ta, true);                     //Sets the text area to only accept one line of text
  lv_ta_set_text(ta, "");                           //Sets the default text of the text area

  std::cout << "You can enter a control name here:" << std::endl;     //Prints a message to the terrminal indicating that you can enter a name for the control configuration
  pros::Task get_terminal_task(get_terminal);       //Reads from the terminal
}

/**
 *
 * remove_controls
 *
 * Displays a menu allowing the user to remove a button configuration
 *
 */
void remove_controls(){
  std::vector<const char*> controls_select_vec;     //Creates a vector of c-strings
  for(int i = 0; i < (int) std::floor(control_labels.size() * 1.5); i++){       //For every item in control_labels * 1.5
    if(((i - 2) % 3 == 0) && i > 0)                 //If i - 2 is a multiple of 3 and not 0
      controls_select_vec.push_back("\n");          //Push '\n' into controls_select_vec
    else                                            //If i - 2 is not a multiple of 3 or i is 0
      controls_select_vec.push_back(control_labels[i - (int) i/3].c_str());     //Pushes the current item in control_labels into controls_select_vec
  }
  if(controls_select_vec.size() % 3 != 0)           //If the final size of controls_select_vec is not a multiple of 3
    controls_select_vec.push_back("\n");            //Push '\n' into controls_select_vec
  controls_select_vec.push_back("Exit");            //Push "Exit" into controls_select_vec
  controls_select_vec.push_back("");                //End of vector

  static const char * controls_select_c[50];        //Creates an array of c-strings
  std::copy(controls_select_vec.begin(), controls_select_vec.end(), controls_select_c);   //Copy controls_select_vec into controls_select_c

  lv_obj_t * controls_select_m = lv_btnm_create(lv_scr_act(), NULL);  //Creates button matrix
  lv_obj_align(controls_select_m, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 10); //Aligns button matrix to the top left corner of the screen
  lv_btnm_set_map(controls_select_m, controls_select_c);              //Adds controls selection menu buttons to controls selection menu
  lv_obj_set_size(controls_select_m, LV_HOR_RES - 40, LV_VER_RES - 20);          //Sets size of button matrix
  lv_btnm_set_style(controls_select_m, LV_BTNM_STYLE_BG, &btn_bg);    //Sets the background style of the buton matrix
  lv_btnm_set_style(controls_select_m, LV_BTNM_STYLE_BTN_REL, &std_btn);        //Sets the style of the button matrix when a button is not pressed
  lv_btnm_set_style(controls_select_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);      //Sets the style of the button matrix when a button is a pressed
  lv_btnm_set_action(controls_select_m, apply_remove_controls);       //Set action of button matrix to apply_remove_controls
}

/**
*
* motor_select
*
* Displays the motor selection menu, where you can select a motor to view its
* settings.
*/
void motor_select(){
  prev = "Main Menu";                               //Sets the previous screen to the main menu

  std::vector<const char*> motor_select_vec;        //Creates vector of c-strings
  if(!motor_labels.empty())                         //If motor_labels is not empty
    for(int i = 0; i < (int) std::floor(motor_labels.size() * 1.5); i++){       //For every item in motor_labels * 1.5
      if(((i - 2) % 3 == 0) && i > 0)               //If i - 2 is a multiple of 3 and i is not 0
        motor_select_vec.push_back("\n");           //Push '\n' into motor_select_vec
      else                                          //If i - 2 is not a multiple of 3 or i is 0
        motor_select_vec.push_back(motor_labels[i - (int) i/3].c_str());        //Push the current item in motor_labels into motor_select_vec
      }
  if(motor_select_vec.size() % 3 != 0)              //If the final size of motor_select_vec is not a multiple of 3
    motor_select_vec.push_back("\n");               //Push '\n' into motor_select_vec
  motor_select_vec.push_back("Exit");               //Push "Exit" into motor_select_vec
  motor_select_vec.push_back("");                   //End of vector

  static const char * motor_select_c[50];           //Create an array of c-strings
  std::copy(motor_select_vec.begin(), motor_select_vec.end(), motor_select_c);  //Copy motor_select_vec into motor_select_c


  lv_obj_t * motor_select_m = lv_btnm_create(lv_scr_act(), NULL);     //Creates button matrix
  lv_obj_align(motor_select_m, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 0, 0);    //Aligns button matrix to the top left corner of the screen
  lv_btnm_set_map(motor_select_m, motor_select_c);                    //Adds motor selection menu buttons to motor selection menu
  lv_obj_set_size(motor_select_m, LV_HOR_RES - 40, LV_VER_RES - 20);       //Sets size of button matrix
  lv_btnm_set_style(motor_select_m, LV_BTNM_STYLE_BG, &btn_bg);       //Sets the background style of the buton matrix
  lv_btnm_set_style(motor_select_m, LV_BTNM_STYLE_BTN_REL, &std_btn); //Sets the style of the button matrix when a button is not pressed
  lv_btnm_set_style(motor_select_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);         //Sets the style of the button matrix when a button is a pressed
  lv_btnm_set_action(motor_select_m, nav_btnm);                       //Assigns nav_btnm to the motor selection menu

  static const char* motor_edit_c[] = {SYMBOL_PLUS, "\n",
                                       SYMBOL_MINUS, ""};             //Creates buttons for motor editing button matrix

  lv_obj_t * motor_edit_m = lv_btnm_create(lv_scr_act(), NULL);       //Creates button matrix
  lv_btnm_set_map(motor_edit_m, motor_edit_c);                        //Adds motor selection menu buttons to motor selection menu
  lv_obj_set_size(motor_edit_m, 40, LV_VER_RES - 20);                 //Sets size of button matrix
  lv_btnm_set_style(motor_edit_m, LV_BTNM_STYLE_BG, &btn_bg);         //Sets the background style of the buton matrix
  lv_btnm_set_style(motor_edit_m, LV_BTNM_STYLE_BTN_REL, &std_btn);   //Sets the style of the button matrix when a button is not pressed
  lv_btnm_set_style(motor_edit_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);           //Sets the style of the button matrix when a button is a pressed
  lv_obj_align(motor_edit_m, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, 0, 0);        //Aligns button matrix to the top left corner of the screen
  lv_btnm_set_action(motor_edit_m, edit_motor);                       //Assigns nav_btnm to the motor selection menu
}

/**
 *
 * motor_settings
 *
 * Displays the motor settings menu for a different motor based on the passed
 * parameter
 *
 * @param motor is the id of the motor to be changed
 */
void motor_settings(int motor){
  prev = "Motors";                                  //Sets the previous screen to the motor selection screen

  std::string text = "Motor Port: ";                //Adds "Motor Port: " to the port label

  motor_set_id = motor;

  lv_obj_t * rev_switch = lv_sw_create(lv_scr_act(), NULL);           //Creates a switch (for reversing motors)
  lv_sw_set_style(rev_switch, LV_SW_STYLE_BG, &switch_bg);            //Sets the background style of the switch
  lv_sw_set_style(rev_switch, LV_SW_STYLE_INDIC, &switch_indic);      //Sets the indicative style of the switch
  lv_sw_set_style(rev_switch, LV_SW_STYLE_KNOB_OFF, &switch_off);     //Sets the style of the switch when turned off
  lv_sw_set_style(rev_switch, LV_SW_STYLE_KNOB_ON, &switch_on);       //Sets the style of the switch when turned on
  lv_obj_align(rev_switch, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, -15, -20);   //Aligns the switch to the bottom right corner of the screen

  lv_obj_t * port_menu_btn = lv_btn_create(lv_scr_act(), NULL);       //Creates port menu button
  lv_obj_t * port_menu_label = lv_label_create(port_menu_btn, NULL);  //Creates label for port menu button
  lv_obj_set_free_num(port_menu_btn, 1);          //Sets the free number of the port menu button to 1
  lv_btn_set_style(port_menu_btn, LV_BTN_STYLE_REL, &std_btn);        //Sets the style of the port menu button when not pressed
  lv_btn_set_style(port_menu_btn, LV_BTN_STYLE_PR, &std_btn_pr);      //Sets the style of the port menu button when pressed
  lv_label_set_text(port_menu_label, "Port Menu");                    //Sets the text of the port menu button label
  lv_obj_align(port_menu_label, port_menu_btn, LV_ALIGN_CENTER, 0, 0);//Aligns the port menu button label to the center of the port menu button

  lv_obj_t * exit_btn = lv_btn_create(lv_scr_act(), NULL);            //Creates exit button
  lv_obj_t * exit_btn_label = lv_label_create(exit_btn, NULL);        //Creates label for exit button
  lv_obj_set_free_num(exit_btn, 0);                 //Sets the free number of the exit button to 0
  lv_btn_set_style(exit_btn, LV_BTN_STYLE_REL, &std_btn);             //Sets the style of the exit button when not pressed
  lv_btn_set_style(exit_btn, LV_BTN_STYLE_PR, &std_btn_pr);           //Sets the style of the exit button when pressed
  lv_label_set_text(exit_btn_label, "Exit");        //Sets the text of the exit button label
  lv_obj_align(exit_btn, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);//Aligns the exit button to the bottom-left corner of the screen
  lv_obj_align(exit_btn_label, exit_btn, LV_ALIGN_CENTER, 0, 0);      //Aligns the exit button label to the center of the exit button

  lv_obj_t * name = lv_label_create(lv_scr_act(), NULL);              //Creates name label
  lv_obj_t * port = lv_label_create(lv_scr_act(), NULL);              //Creates port label
  lv_label_set_style(name, &label_style);           //Sets style of name label
  lv_label_set_style(port, &label_style);           //Sets style of port label

  curr_motor = motor_labels[motor];               //curr_motor (global) is set to motor_labels[motor-1] (the selected motor)
  lv_obj_set_free_num(rev_switch, motor);           //Sets free num of rev_switch to the value of parameter "motor"
  lv_label_set_text(name, motor_labels[motor].c_str());             //Sets the text of the name label to motor_labels[motor-1] (the selected motor)
  text += std::to_string(motor_ports[motor]);     //Adds motor_ports[motor-1] (as a string) to text
  lv_label_set_text(port, text.c_str());            //Sets the text of the port label to the variable "text"
  if(motor_reversals[motor] == true)              //If motor_reversals[motor-1] is true
    lv_sw_on(rev_switch);                           //Turn on rev_switch
  else                                              //If lm1_rev is not true (false)
    lv_sw_off(rev_switch);                          //Turn off rev_switch

  lv_obj_align(name, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 0);        //Aligns name label to the top-middle of the screen
  lv_obj_align(port, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, -60);            //Aligns port label to the center of the screen

  lv_sw_set_action(rev_switch, motor_rev);          //Sets the action of rev_switch to motor_rev
  lv_btn_set_action(port_menu_btn, LV_BTN_ACTION_CLICK, nav_btn);     //Sets action of port_menu_btn to nav_btn
  lv_btn_set_action(exit_btn, LV_BTN_ACTION_CLICK, nav_btn);          //Sets action of exit_btn to nav_btn

  temperature = lv_label_create(lv_scr_act(), NULL);//Defines temperature label (global)
  lv_obj_set_style(temperature, &label_style);      //Sets style of the temperature level
  lv_label_set_text(temperature, "Temperature: ");  //Sets text of the temperature level
  lv_obj_align(temperature, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, -30);    //Aligns the temperature label to the center of the screen

  get_temp_task = lv_task_create(get_temp, 1000, LV_TASK_PRIO_HIGH, NULL);       //Creates get_temp_task, runs every 1000 milliseconds

  std::string motor_types_str = join_string(motor_type_list, '\n');   //Create new string from items in motor_type_list
  motor_types_str.resize(motor_types_str.size() - 1);                 //Remove last character from motor_types_str

  if(drive_model == "xdrive"){                      //If the drive model is x-drive
    const std::string new_options = "Left Drive Front\nLeft Drive Rear\nRight Drive Front\nRight Drive Rear\n";   //
    const std::string left_motor_option = "Left Drive\n";
    const std::string right_motor_option = "Right Drive\n";
    std::string::size_type left_size = motor_types_str.find(left_motor_option);
    if(motor_types_str.find(left_motor_option) != std::string::npos)
      motor_types_str.erase(left_size, left_motor_option.length());
    std::string::size_type right_size = motor_types_str.find(right_motor_option);
    if(motor_types_str.find(right_motor_option) != std::string::npos)
      motor_types_str.erase(right_size, right_motor_option.length());
    motor_types_str.insert(5, new_options);
  }

  lv_obj_t * motor_type_ddlist = lv_ddlist_create(lv_scr_act(), NULL);
  lv_ddlist_set_options(motor_type_ddlist, motor_types_str.c_str());
  lv_ddlist_set_selected(motor_type_ddlist, std::distance(motor_type_list.begin(), std::find(motor_type_list.begin(), motor_type_list.end(), motor_types[motor])));
  lv_ddlist_set_action(motor_type_ddlist, change_motor_type);
  lv_obj_align(motor_type_ddlist, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, 0, 0);
}

/**
 *
 * get_temp
 *
 * Updates the label temperature with the temperature of the selected motor in
 * fahrenheit based on curr_motor (ideally called repeatedly in a task).
 *
 * @param ign ignore
 */
void get_temp(void * ign){
  std::string temperature_s;                        //Creates string that holds the motor temperature
  for(int i = 0; i < motor_labels.size(); i++){
      if(curr_motor == motor_labels[i])                           //If the current motor is the Left Side Front Drive Motor
      temperature_s = std::to_string(((pros::c::motor_get_temperature(motor_ports[i]) * 1.8) + 32.0));  //Reads the temperature of the motor, converts it into Fahrenheit, and stores it in temperature_s
  }
  temperature_s.resize(5);                          //Resize temperature_s to 5 characters (4 decimal places)
  lv_label_set_text(temperature, ("Temperature: " + temperature_s + "°F").c_str());   //Sets the temperature label text
  lv_obj_align(temperature, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, -30);    //Realigns the temperature label
}


/**
 *
 * add_motor
 *
 * Displays a menu that allows the user to name a new motor
 *.
 */
void add_motor(){
  prev = "Motors";

  terminal_id = 1;

  get_temp_task = lv_task_create(null, 999999, LV_TASK_PRIO_LOWEST, NULL);    //Sets dummy task
  lv_task_del(get_temp_task);

  lv_obj_t * kb = lv_kb_create(lv_scr_act(), NULL);                   //Creates keyboard
  ta = lv_ta_create(lv_scr_act(), NULL);        //Creates text area

  lv_obj_set_size(kb, LV_HOR_RES - 10, LV_VER_RES - 60);              //Sets size of keyboard
  lv_obj_align(kb, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, 0);       //Aligns keyboard to bottom middle of the screen
  lv_kb_set_style(kb, LV_KB_STYLE_BG, &kb_bg_style);                  //Sets the background style of the keyboard
  lv_kb_set_style(kb, LV_KB_STYLE_BTN_REL, &kb_style);                //Sets the style of a key when pressed
  lv_kb_set_style(kb, LV_KB_STYLE_BTN_PR, &kb_style_pr);              //Sets the style of a key when not pressed
  lv_kb_set_ta(kb, ta);                         //Assigns the keyboard to the text area
  lv_kb_set_hide_action(kb, clear_ta);          //Sets the text area to be cleared when the hide button on the keyboard is pressed
  lv_kb_set_ok_action(kb, apply_motor);        //Sets the motor to be saved when the OK button on the keyboard is pressed

  lv_obj_set_size(ta, LV_HOR_RES - 10, 30);     //Sets the size of the text area
  lv_obj_align(ta, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 5);        //Aligns the text area to the top middle
  lv_ta_set_one_line(ta, true);                 //Sets the text area to only accept one line of text
  lv_ta_set_text(ta, "");                       //Sets the default text of the text area

  std::cout << "You can enter a motor name here:" << std::endl;
  pros::Task get_terminal_task(get_terminal);
}

/**
 *
 * remove_motor
 *
 * Displays a menu that allows the user to select a motor to delete
 *.
 */
void remove_motor(){
  prev = "Motors";                                  //Sets previous screen to motor select menu

  std::vector<const char*> motor_select_vec;        //Creates vector of c-strings
  for(int i = 0; i < (int) std::floor(motor_labels.size() * 1.5); i++){         //For every item in motor_labels * 1.5
    if(((i - 2) % 3 == 0) && i > 0)                 //If i - 2 is a multiple of 3 and i is not equal to 0
      motor_select_vec.push_back("\n");             //Push '\n' into vector
    else                                            //If i - 2 is not a multiple of 3 or i is equal to 0
      motor_select_vec.push_back(motor_labels[i - (int) i/3].c_str());//Push the current item in motor_labels into vector
  }
  if(motor_select_vec.size() % 3 != 0)              //If the final size of the vector is not a multiple of 3
    motor_select_vec.push_back("\n");               //Push '\n' into vector
  motor_select_vec.push_back("Exit");               //Push "Exit" into vector
  motor_select_vec.push_back("");                   //End of vector

  static const char * motor_select_c[50];           //Creates an array of c-strings
  std::copy(motor_select_vec.begin(), motor_select_vec.end(), motor_select_c);  //Copies motor_select_vec into motor_select_c

  lv_obj_t * motor_select_m = lv_btnm_create(lv_scr_act(), NULL);     //Creates button matrix
  lv_btnm_set_map(motor_select_m, motor_select_c);                    //Adds motor selection menu buttons to motor selection menu
  lv_obj_set_size(motor_select_m, LV_HOR_RES - 40, LV_VER_RES - 20);  //Sets size of button matrix
  lv_btnm_set_style(motor_select_m, LV_BTNM_STYLE_BG, &btn_bg);       //Sets the background style of the buton matrix
  lv_btnm_set_style(motor_select_m, LV_BTNM_STYLE_BTN_REL, &std_btn); //Sets the style of the button matrix when a button is not pressed
  lv_btnm_set_style(motor_select_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);         //Sets the style of the button matrix when a button is a pressed
  lv_obj_align(motor_select_m, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 20);    //Aligns button matrix to the top left corner of the screen
  lv_btnm_set_action(motor_select_m, apply_remove_motor);

  lv_obj_t * remove_motor_label = lv_label_create(lv_scr_act(), NULL);
  lv_label_set_style(remove_motor_label, &label_style);
  lv_label_set_text(remove_motor_label, "Which motor would you like to remove?");
  lv_obj_align(remove_motor_label, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 0);
}

/**
 *
 * port_menu
 *
 * Displays a button matrix of ports to assign to a motor.
 */
void port_menu(){
  prev = "Motor Settings";                          //Sets the previous screen to the motor settiings menu
  static const char * port_menu_c[] = {"1", "2", "3", "4", "5", "\n",
                                       "6", "7", "8", "9", "10", "\n",
                                       "11", "12", "13", "14", "15", "\n",
                                       "16", "17", "18", "19", "20", "\n",
                                       "21", "\n",
                                       "Exit", ""}; //Creates port menu buttons

  lv_obj_t * port_menu_m = lv_btnm_create(lv_scr_act(), NULL);        //Creates port menu
  lv_obj_set_size(port_menu_m, LV_HOR_RES, LV_VER_RES);               //Sets size of the port menu
  lv_btnm_set_style(port_menu_m, LV_BTNM_STYLE_BG, &btn_bg);          //Sets the background style of the port menu
  lv_btnm_set_style(port_menu_m, LV_BTNM_STYLE_BTN_REL, &std_btn);    //Sets the style of the port menu when a button is not pressed
  lv_btnm_set_style(port_menu_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);  //Sets the style of the port menu when a button is pressed
  lv_btnm_set_map(port_menu_m, port_menu_c);        //Adds the port menu buttons to the port menu
  lv_btnm_set_action(port_menu_m, change_motor_port);                 //Sets the action of the port menu to change_motor_port
}

/**
*
* sensor_menu
*
* Displays the sensir selection menu, where you can select a sensor to view its
* settings.
*/
void sensor_menu(){
  prev = "Main Menu";                               //Sets the previous menu to the main menu

  std::vector<const char*> sensor_menu_vec;         //Creates a vector of c-strings
  for(int i = 0; i < (int) std::floor(sensor_labels.size() * 1.5); i++)         //For every item in sensor_labels * 1.5
    if(((i - 2) % 3 == 0) && i > 0)                 //If i - 2 is a multiple of 3 and i is not equal to 0
      sensor_menu_vec.push_back("\n");              //Push '\n' into vector
    else                                            //If i - 2 is not a multiple of 3 or i is equal to 0
      sensor_menu_vec.push_back(sensor_labels[i - (int) i/3].c_str());          //Push the current item in sensor_labels into vector
    if(sensor_menu_vec.size() % 3 != 0)             //If final vector size is not a multiple of 3
      sensor_menu_vec.push_back("\n");              //Push '\n' into vector
    sensor_menu_vec.push_back("Exit");              //Push "Exit" into vetor
    sensor_menu_vec.push_back("");                  //End of vector

  static const char * sensor_menu_c[50];            //Creates array of c-strings
  std::copy(sensor_menu_vec.begin(), sensor_menu_vec.end(), sensor_menu_c);     //Copy sensor_menu_vec into sensor_menu_c

  lv_obj_t * sensor_menu_m = lv_btnm_create(lv_scr_act(), NULL);      //Creates sensor selection menu
  lv_obj_set_size(sensor_menu_m, LV_HOR_RES - 40, LV_VER_RES - 20);   //Sets size of sensor selection menu
  lv_btnm_set_style(sensor_menu_m, LV_BTNM_STYLE_BG, &btn_bg);        //Sets the background style of the sensor selectiton menu
  lv_btnm_set_style(sensor_menu_m, LV_BTNM_STYLE_BTN_REL, &std_btn);  //Sets the style of the sensor selction menu when a button is not pressed
  lv_btnm_set_style(sensor_menu_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);//Sets the style of the esensor selection menu when a button is pressed
  lv_btnm_set_map(sensor_menu_m, sensor_menu_c);    //Adds the sensor selection menu buttons to the sensor selection menu
  lv_obj_align(sensor_menu_m, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 0, 0);   //Aligns the senso selection menu to the center of the screen
  lv_btnm_set_action(sensor_menu_m, nav_btnm);      //Sets the action of the sensor selection menu to nav_btnm

  static const char *sensor_edit_c[] = {SYMBOL_PLUS, "\n",
                                        SYMBOL_MINUS, ""};

  lv_obj_t * sensor_edit_m = lv_btnm_create(lv_scr_act(), NULL);
  lv_obj_set_size(sensor_edit_m, 40, LV_VER_RES - 20);                //Sets size of sensor selection menu
  lv_btnm_set_style(sensor_edit_m, LV_BTNM_STYLE_BG, &btn_bg);        //Sets the background style of the sensor selectiton menu
  lv_btnm_set_style(sensor_edit_m, LV_BTNM_STYLE_BTN_REL, &std_btn);  //Sets the style of the sensor selction menu when a button is not pressed
  lv_btnm_set_style(sensor_edit_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);//Sets the style of the esensor selection menu when a button is pressed
  lv_btnm_set_map(sensor_edit_m, sensor_edit_c);    //Adds the sensor selection menu buttons to the sensor selection menu
  lv_obj_align(sensor_edit_m, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, 0, 0);   //Aligns the senso selection menu to the center of the screen
  lv_btnm_set_action(sensor_edit_m, edit_sensor);      //Sets the action of the sensor selection menu to nav_btnm
}

/**
 *
 * sensor_settings
 *
 * Displays the sensor settings menu for a different sensor based on the passed
 * parameter
 *
 * @param motor is the id of the sensor to be changed
 */
void sensor_settings(int sensor){
  prev = "Sensor Menu";                             //Sets the previous menu to the sensor selection menu

  sensor_id = sensor;                               //Sets sensor_id (global) to parameter sensor

  std::string sensor_name;                          //Creates sensor name string
  std::string sensor_port = "Sensor Port: ";        //Create sensor port string

  int8_t * port_ref;                                //Pointer to type int8_t

  lv_obj_t * port_menu_btn = lv_btn_create(lv_scr_act(), NULL);       //Creates port menu button
  lv_obj_t * port_menu_label = lv_label_create(port_menu_btn, NULL);  //Creates port menu button label
  lv_obj_set_free_num(port_menu_btn, 7);                              //Sets the free num of the port menu button to 7
  lv_btn_set_style(port_menu_btn, LV_BTN_STYLE_REL, &std_btn);        //Sets the style of the port menu button when not pressed
  lv_btn_set_style(port_menu_btn, LV_BTN_STYLE_PR, &std_btn_pr);      //Sets the style of the port menu button when pressed
  lv_label_set_text(port_menu_label, "Port Menu");  //Sets the text of the port menu button label
  lv_obj_align(port_menu_label, port_menu_btn, LV_ALIGN_CENTER, 0, 0);//Aligns the port menu button label to the center of the port menu button

  lv_obj_t * exit_btn = lv_btn_create(lv_scr_act(), NULL);            //Creates exit button
  lv_obj_t * exit_btn_label = lv_label_create(exit_btn, NULL);        //Creates exit button label
  lv_obj_set_free_num(exit_btn, 0);                 //Sets the free num of the exit button to 0
  lv_btn_set_style(exit_btn, LV_BTN_STYLE_REL, &std_btn);             //Sets the style of the exit button when not pressed
  lv_btn_set_style(exit_btn, LV_BTN_STYLE_PR, &std_btn_pr);           //Sets the style of the exit button when preed
  lv_label_set_text(exit_btn_label, "Exit");        //Sets the text of the exit button label
  lv_obj_align(exit_btn, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);//Aligns the exit button to the bottom-left corner of the screen
  lv_obj_align(exit_btn_label, exit_btn, LV_ALIGN_CENTER, 0, 0);      //Aligns the exit button label to the center of the exit button

  lv_obj_t * name = lv_label_create(lv_scr_act(), NULL);              //Creates name label
  lv_obj_t * port = lv_label_create(lv_scr_act(), NULL);              //Creates port label
  lv_label_set_style(name, &label_style);           //Sets the style of the name label
  lv_label_set_style(port, &label_style);           //Sets the style of the port label

  std::string sensor_type_str = join_string(sensor_type_list, '\n');
  sensor_type_str.resize(sensor_type_str.size() - 1);

  lv_obj_t * sensor_type_ddlist = lv_ddlist_create(lv_scr_act(), NULL);
  lv_ddlist_set_options(sensor_type_ddlist, sensor_type_str.c_str());
  lv_ddlist_set_selected(sensor_type_ddlist, std::distance(sensor_type_list.begin(), std::find(sensor_type_list.begin(), sensor_type_list.end(), sensor_types[sensor_id])));
  lv_ddlist_set_action(sensor_type_ddlist, change_sensor_type);
  lv_obj_align(sensor_type_ddlist, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, 0, 0);

  sensor_name = sensor_labels[sensor_id];                     //Sensor name is set to "Bumper A"
  port_ref = &sensor_ports[sensor_id];                    //port_ref is set to the address of bumper_a_port (globals.cpp

  if(sensor_types[sensor_id] == "Back Bumper" || sensor_types[sensor_id] == "Limit" || sensor_types[sensor_id] == "Encoder"){   //If sensor is an ADI sensor
    switch(*port_ref){                              //Tests variable that port_ref points to
      case 1:                                       //If variable = 1
        sensor_port += "1/A";                       //Add "1/A" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 2:                                       //If variable = 2
        sensor_port += "2/B";                       //Add "2/B" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 3:                                       //IF variable = 3
        sensor_port += "3/C";                       //Add "3/C" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 4:                                       //If variable = 4
        sensor_port += "4/D";                       //Add "4/D" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 5:                                       //If variable = 5
        sensor_port += "5/E";                       //Add "5/E" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 6:                                       //If variable = 6
        sensor_port += "6/F";                       //Add "6/F" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 7:                                       //If variable = 7
        sensor_port += "7/G";                       //Add "7/G" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 8:                                       //If variable = 8
        sensor_port += "8/H";                       //Add "8/H" to sensor port text
        break;                                      //Breaks out of variable switch statement
    }
  }
  else{                                             //If sensor is a V5 sensor
    switch(*port_ref){                              //Tests the variable that port_ref points to
      case 1:                                       //If variable = 1
        sensor_port += "1";                         //Add "1" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 2:                                       //If variable = 2
        sensor_port += "2";                         //Add "2" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 3:                                       //If variable = 3
        sensor_port += "3";                         //Add "3" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 4:                                       //If variable = 4
        sensor_port += "4";                         //Add "4" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 5:                                       //If variable = 5
        sensor_port += "5";                         //Add "5" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 6:                                       //If variable = 6
        sensor_port += "6";                         //Add "6" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 7:                                       //If variable = 7
        sensor_port += "7";                         //Add "7" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 8:                                       //If variable = 8
        sensor_port += "8";                         //Add "8" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 9:                                       //If variable = 9
        sensor_port += "9";                         //Add "9" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 10:                                      //If variable = 10
        sensor_port += "10";                        //Add "10" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 11:                                      //If variable = 11
        sensor_port += "11";                        //Add "11" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 12:                                      //If variable = 12
        sensor_port += "12";                        //Add "12" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 13:                                      //If variable = 13
        sensor_port += "13";                        //Add "13" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 14:                                      //If variable = 14
        sensor_port += "14";                        //Add "14" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 15:                                      //If variable = 15
        sensor_port += "15";                        //Add "15" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 16:                                      //If variable = 16
        sensor_port += "16";                        //Add "16" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 17:                                      //If variable = 17
        sensor_port += "17";                        //Add "17" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 18:                                      //If variable = 18
        sensor_port += "18";                        //Add "18" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 19:                                      //If variable = 19
        sensor_port += "19";                        //Add "19" to sensor port text
        break;                                      //Breaks out of variable switch statement
      case 20:                                      //If variable = 20
        sensor_port += "20";                        //Add "20" to sensor port text
        break;                                      //Breaks out of variable switch statement
    }
  }

  lv_label_set_text(name, sensor_name.c_str());     //Sets the text of the name label
  lv_label_set_text(port, sensor_port.c_str());     //Sets the text of the port label
  lv_obj_align(name, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 0);        //Aligns the name label to the top-middle of the screen
  lv_obj_align(port, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, -60);            //Aligns the port label to the center of the screen

  lv_btn_set_action(port_menu_btn, LV_BTN_ACTION_CLICK, nav_btn);     //Sets the action of the port menu button to nav_btn
  lv_btn_set_action(exit_btn, LV_BTN_ACTION_CLICK, nav_btn);          //Sets the action of the exit button to nav_btn

  if(sensor_types[sensor_id] == "Back Bumper" || sensor_types[sensor_id] == "Limit"){ //If sensor is bumper/limit switch
    get_limit_switch_bool = true;
    get_optical_data_bool = false;
    bumper_pressed = lv_label_create(lv_scr_act(), NULL);             //Creates bumper pressed label
    lv_label_set_style(bumper_pressed, &label_style);                 //Sets the style of the bumper pressed label
    lv_obj_align(bumper_pressed, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, -30);//Aligns the bumper pressed label to the center of the screen
    get_limit_switch_task = lv_task_create(get_limit_switch, 10, LV_TASK_PRIO_HIGH, NULL);  //Creates a task that calls get_limit_switch every 10 milliseconds
  }
  else if(sensor_types[sensor_id] == "Optical"){                          //If the sensor is an optical sensor
    get_limit_switch_bool = false;
    get_optical_data_bool = true;
    lv_obj_align(port, name, LV_ALIGN_CENTER, 0, 40);                 //Aligns the port label with the center of the name label

    optical_hue_label = lv_label_create(lv_scr_act(), NULL);          //Creates label that displays the hue detectd by the optical sensor
    lv_label_set_style(optical_hue_label, &label_style);              //Sets the style of the hue label
    lv_obj_align(optical_hue_label, lv_scr_act(), LV_ALIGN_CENTER, -40, -40);   //Aligns the hue label with the center of the screen

    optical_saturation_label = lv_label_create(lv_scr_act(), NULL);   //Creates label that displays the saturation detectd by the optical sensor
    lv_label_set_style(optical_saturation_label, &label_style);       //Sets the style of the saturation label
    lv_obj_align(optical_saturation_label, lv_scr_act(), LV_ALIGN_CENTER, -40, 0);    //Aligns the saturation label with the center of the screen

    optical_brightness_label = lv_label_create(lv_scr_act(), NULL);   //Creates label that displays the brightness detectd by the optical sensor
    lv_label_set_style(optical_brightness_label, &label_style);       //Sets the style of the brightness label
    lv_obj_align(optical_brightness_label, lv_scr_act(), LV_ALIGN_CENTER, -40, 40);   //Aligns the brightness label with the center of the screen

    optical_rgb_label = lv_label_create(lv_scr_act(), NULL);          ////Creates label that displays the rgb values detectd by the optical sensor
    lv_label_set_style(optical_rgb_label, &label_style);              //Sets the style of the rgb label
    lv_obj_align(optical_rgb_label, lv_scr_act(), LV_ALIGN_CENTER, -40, 80);    //Aligns the rgb label with the center of the screen

    lv_obj_t * sensor_location_ddlist = lv_ddlist_create(lv_scr_act(), NULL);
    lv_ddlist_set_options(sensor_location_ddlist, "None\nIntakes\nSorter\nTop-Roller");
    if(sensor_locations[sensor_id] == "Intakes")
      lv_ddlist_set_selected(sensor_location_ddlist, 1);
    else if(sensor_locations[sensor_id] == "Sorter")
      lv_ddlist_set_selected(sensor_location_ddlist, 2);
    else if(sensor_locations[sensor_id] == "Top-Roller")
      lv_ddlist_set_selected(sensor_location_ddlist, 3);
    else
      lv_ddlist_set_selected(sensor_location_ddlist, 0);

    lv_ddlist_set_action(sensor_location_ddlist, change_sensor_location);
    lv_obj_align(sensor_location_ddlist, sensor_type_ddlist, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);

    get_optical_data_task = lv_task_create(get_optical_data, 1000, LV_TASK_PRIO_HIGH, NULL);    //Creates task that calls get_optical_data every second
  }
}

/**
 *
 * sensor_port_menu
 *
 * Displays a button matrix of ports to assign to a sensor.
 */
void sensor_port_menu(){
  prev = "Sensor Settings";                         //Sets the previous screen to the sensor settings menu
  static const char * adi_ports[] = {"1/A", "2/B", "3/C", "4/D", "\n",
                                     "5/E", "6/F", "7/G", "8/H", ""}; //Creates ADI port menu buttons

  static const char * v5_ports[] = {"1", "2", "3", "4", "5", "\n",
                                    "6", "7", "8", "9", "10", "\n",
                                    "11", "12", "13", "14", "15", "\n",
                                    "16", "17", "18", "19", "20", ""};//Creates V5 port menu buttons

  lv_obj_t * port_menu = lv_btnm_create(lv_scr_act(), NULL);          //Creates sensor port menu
  lv_obj_set_size(port_menu, LV_HOR_RES, LV_VER_RES);                 //Sets size of sensor port menu
  lv_btnm_set_style(port_menu, LV_BTNM_STYLE_BG, &btn_bg);            //Sets the background style of the sensor port menu
  lv_btnm_set_style(port_menu, LV_BTNM_STYLE_BTN_REL, &std_btn);      //Sets the style of the sensor port menu when not pressed
  lv_btnm_set_style(port_menu, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);    //Sets the style of the sensor port menu when pressed
  if(sensor_types[sensor_id] == "Back Bumper" || sensor_types[sensor_id] == "Limit" || sensor_types[sensor_id] == "Encoder")   //If the sensor is an ADI sensor
    lv_btnm_set_map(port_menu, adi_ports);          //Adds the ADI port menu buttons to the port menu
  else                                             //If the sensor is a V5 sensor
    lv_btnm_set_map(port_menu, v5_ports);           //Sets the V5 port menu buttons to the port menu
  lv_btnm_set_action(port_menu, change_sensor_port); //Sets the action of the port menu to change_adi_port
}

/**
 *
 * get_limit_switch
 *
 * Sets the current text of bumper_pressed (global) to reflect the current state
 * of the switch (ideally called repeatedly in a task)
 */
void get_limit_switch(void * ign){
  if((sensor_types[sensor_id] == "Back Bumper" || sensor_types[sensor_id] == "Limit") && get_limit_switch_bool == true){
    std::string pressed = "Status: ";                 //Defines pressed as beginning with "Status: "
    pressed += get_button_state(sensor_ports[sensor_id]);               //Gets the state of the sensor port at sensor_id
    lv_label_set_text(bumper_pressed, pressed.c_str());                 //Sets the text of bumper_pressed (global) to "pressed"
    lv_obj_align(bumper_pressed, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, -30); //Aligns bumper_pressed with the center of the screen
  }
  else;
}

/**
 *
 * get_button_state
 *
 * Returns a string that indicates whether or not a sensor at the specified port
 * has been pressed.
 *
 * @param port is the ADI port that is read from
 * @return a string that states whether or not the switch is pressed
 */
std::string get_button_state(int port){
  if(pros::c::adi_digital_read(port) == 0)          //If the switch is not pressed
    return "Not Pressed";                           //return "Not Pressed"
  else if(pros::c::adi_digital_read(port))          //If the switch is pressed
    return "Pressed";                               //return "Pressed"
  else                                              //Else (no sensor plugged in port)
    return "N/A";                                   //return "N/A"
}

/**
 *
 * get_optical_data
 *
 * Gets data from the optical sensor (ideally called repeatedly in a task)
 *
 * @param ign ignore
 */
void get_optical_data(void * ign){
  if(sensor_types[sensor_id] == "Optical" && get_optical_data_bool == true){
    optical_hue = pros::c::optical_get_hue(sensor_ports[sensor_id]);               //Set optical_hue (global) to the hue detected by the optical sensor
    optical_saturation = pros::c::optical_get_saturation(sensor_ports[sensor_id]); //Set optical_saturation (global) to the saturation detected by the optical sensor
    optical_brightness = pros::c::optical_get_brightness(sensor_ports[sensor_id]); //Set optical_brightness (global) to the brightness detected by the optical sensor
    pros::c::optical_rgb_s_t rgb_values = pros::c::optical_get_rgb(sensor_ports[sensor_id]); //Set rgb_values (global) to the rgb values detected by the optical sensor
    optical_rgb_red = rgb_values.red;                 //Sets optical_rgb_red (global) to the red value of rgb_values (global)
    optical_rgb_green = rgb_values.green;             //Sets optical_rgb_green (global) to the green value of rgb_values (global)
    optical_rgb_blue = rgb_values.blue;               //Sets optical_rgb_blue (global) to the b;ue value of rgb_values (global)

    lv_label_set_text(optical_hue_label, ("Hue: " + std::to_string(optical_hue)).c_str());    //Sets optical hue label display optical_hue
    lv_label_set_text(optical_saturation_label, ("Saturation: " + std::to_string(optical_saturation)).c_str());   //Sets optical saturation label to display optical_saturation
    lv_label_set_text(optical_brightness_label, ("Brightness: " + std::to_string(optical_brightness)).c_str());   //Sets optical brightness label to display optical_brightness
    lv_label_set_text(optical_rgb_label, ("RGB: (" + std::to_string(optical_rgb_red) + ", " + std::to_string(optical_rgb_green) + ", " + std::to_string(optical_rgb_blue) + ")").c_str());    //Sets optical rgb label to display rgb_values
  }
}

/**
 *
 * add_sensor
 *
 * Displays menu that allows the user to name a new sensor
 */
void add_sensor(){
  terminal_id = 2;                                  //Sets terminal_id to 2

  lv_obj_t * kb = lv_kb_create(lv_scr_act(), NULL);                   //Creates keyboard
  ta = lv_ta_create(lv_scr_act(), NULL);            //Creates text area

  lv_obj_set_size(kb, LV_HOR_RES - 10, LV_VER_RES - 60);              //Sets size of keyboard
  lv_obj_align(kb, lv_scr_act(), LV_ALIGN_IN_BOTTOM_MID, 0, 0);       //Aligns keyboard to bottom middle of the screen
  lv_kb_set_style(kb, LV_KB_STYLE_BG, &kb_bg_style);                  //Sets the background style of the keyboard
  lv_kb_set_style(kb, LV_KB_STYLE_BTN_REL, &kb_style);                //Sets the style of a key when pressed
  lv_kb_set_style(kb, LV_KB_STYLE_BTN_PR, &kb_style_pr);              //Sets the style of a key when not pressed
  lv_kb_set_ta(kb, ta);                             //Assigns the keyboard to the text area
  lv_kb_set_hide_action(kb, clear_ta);              //Sets the text area to be cleared when the hide button on the keyboard is pressed
  lv_kb_set_ok_action(kb, apply_sensor);            //Sets the motor to be saved when the OK button on the keyboard is pressed

  lv_obj_set_size(ta, LV_HOR_RES - 10, 30);         //Sets the size of the text area
  lv_obj_align(ta, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 5);          //Aligns the text area to the top middle
  lv_ta_set_one_line(ta, true);                     //Sets the text area to only accept one line of text
  lv_ta_set_text(ta, "");                           //Sets the default text of the text area

  std::cout << "You can enter a sensor name here:" << std::endl;      //Prints a message to the terrminal indicating that you can enter a name for the sensor
  pros::Task get_terminal_task(get_terminal);       //Reads from the terminal
}

/**
 *
 * remove_sensor
 *
 * Displays menu that allows the select a motor to delete
 */
void remove_sensor(){
  prev = "Sensors";                                 //Sets the previous menu to the sensor menu

  std::vector<const char*> sensor_select_vec;       //Creates vector of c-strings
  for(int i = 0; i < sensor_labels.size(); i++)     //For every item in sensor_labels
    if(((i - 2) % 3 == 0) && i > 0)                 //If i - 2 is a multiple of 3 and is not equal to 0
      sensor_select_vec.push_back("\n");            //Push '\n' into vector
    else                                            //If i - 2 is not a multiple of 3 or i is equal to 0
      sensor_select_vec.push_back(sensor_labels[i - (int) i/3].c_str());        //Push the current item in sensor_labels into vector
  if(sensor_select_vec.size() % 3 != 0)             //If the final size of the vector is not a multiple of 3
    sensor_select_vec.push_back("\n");              //Push '\n' into vector
  sensor_select_vec.push_back("Exit");              //Push "Exit" into vector
  sensor_select_vec.push_back("");                  //End of vector

  static const char * sensor_select_c[50];          //Create array of c-strings
  std::copy(sensor_select_vec.begin(), sensor_select_vec.end(), sensor_select_c);   //Copy sensor_select_vec into sensor_select_c

  lv_obj_t * sensor_remove_label = lv_label_create(lv_scr_act(), NULL);         //Create label for menu
  lv_label_set_style(sensor_remove_label, &label_style);              //Set style of label
  lv_label_set_text(sensor_remove_label, "Which sensor would you like to remove?");   //Set label text
  lv_obj_align(sensor_remove_label, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, 0);   //Align label to the top-middle of the screen

  lv_obj_t * sensor_select_m = lv_btnm_create(lv_scr_act(), NULL);    //Creates button matrix
  lv_obj_align(sensor_select_m, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 0, 40);    //Aligns button matrix to the top left corner of the screen
  lv_btnm_set_map(sensor_select_m, sensor_select_c);                  //Adds motor selection menu buttons to motor selection menu
  lv_obj_set_size(sensor_select_m, LV_HOR_RES - 40, LV_VER_RES - 40); //Sets size of button matrix
  lv_btnm_set_style(sensor_select_m, LV_BTNM_STYLE_BG, &btn_bg);      //Sets the background style of the buton matrix
  lv_btnm_set_style(sensor_select_m, LV_BTNM_STYLE_BTN_REL, &std_btn);//Sets the style of the button matrix when a button is not pressed
  lv_btnm_set_style(sensor_select_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);        //Sets the style of the button matrix when a button is a pressed
  lv_btnm_set_action(sensor_select_m, apply_remove_sensor);           //Set button matrix action to apply_remove_sensor
}

/**
 *
 * select_field
 *
 * Allows the user to select whether they want the normal field or the skills
 * field to be displayed
 */
void select_field(){
  prev = "Main Menu";                               //Sets the previous screen to the main menu
  static const char * menu_c[] = {"Normal\nField", "Skills\nField", "\n",
                                  "Exit", ""};      //Creates field selection menu buttons
  lv_obj_t * menu_m = lv_btnm_create(lv_scr_act(), NULL);             //Creates field selecton menu
  lv_obj_set_size(menu_m, LV_HOR_RES - 40, LV_VER_RES - 40);          //Sets size of field selection menu
  lv_obj_align(menu_m, lv_scr_act(), LV_ALIGN_CENTER, 0, 0);          //Aligns field selection menu to the center of the screen
  lv_btnm_set_style(menu_m, LV_BTNM_STYLE_BG, &btn_bg);               //Sets the background style of the field selection menu
  lv_btnm_set_style(menu_m, LV_BTNM_STYLE_BTN_REL, &std_btn);         //Sets the style of the field selection menu when a button is not pressed
  lv_btnm_set_style(menu_m, LV_BTNM_STYLE_BTN_PR, &std_btn_pr);       //Sets the style of the field selection menu when a button is pressed
  lv_btnm_set_map(menu_m, menu_c);                  //Adds field selection menu buttons to the field selection menu
  lv_btnm_set_action(menu_m, nav_btnm);             //Sets the action of the field selection menu to nav_btnm
}

/**
 *
 * draw_field
 *
 * Displays the field on the screen
 */
 void draw_field(){
   for(int i = 0; i < std::size(tiles); i++){        //For every tile in tiles
     tiles_obj[i] = lv_imgbtn_create(lv_scr_act(), NULL);              //Creates a tile object

     lv_obj_set_size(tiles_obj[i], 40, 40);                            //Sets size of tile object

     lv_obj_align(tiles_obj[i], lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 120 + (40 * (i % 6)), -(40 * (i / 6)));   //Aligns tile object with the bottom-left corner of the screen

     lv_obj_set_free_num(tiles_obj[i], (10 * ((i + 1) % 6)) + ((i + 1) / 6) + 1);    //Sets the free number of the tile object (formula: (x-coordinate * 10) + y-coordinate)
     if(lv_obj_get_free_num(tiles_obj[i]) < 10)      //If the free number of the tile object is less than 10
       lv_obj_set_free_num(tiles_obj[i], lv_obj_get_free_num(tiles_obj[i]) + 59);    //Add 59 to free number of tile object
   }

   int tile_count = 0;                               //Initializes tile_count

   for(int i = 0; i < std::size(tiles)/std::sqrt(std::size(tiles)); i++){    //For every tow of tiles
     for(int j = 0; j < std::size(tiles)/std::sqrt(std::size(tiles)); j++){  //For every column of tiles
       if((i + j) % 2 == 0){                         //Selects every tile, counting by twos
         lv_imgbtn_set_src(tiles_obj[tile_count], LV_BTN_STATE_REL, &gtile2);    //Sets tile object image when not pressed
         lv_imgbtn_set_src(tiles_obj[tile_count], LV_BTN_STATE_PR, &gtile2_pr);  //Sets tile object image when pressed
       }
       else{                                         //Selects the rest of the tiles
         lv_imgbtn_set_src(tiles_obj[tile_count], LV_BTN_STATE_REL, &gtile);     //Sets tile object image when not pressed
         lv_imgbtn_set_src(tiles_obj[tile_count], LV_BTN_STATE_PR, &gtile_pr);   //Sets tile object image when pressed
       }
       tile_count++;                                //Increment tile_count
     }
   }

 for(int i = 0; i < std::size(towers); i++){        //For every tower in towers
   towers_obj[i] = lv_imgbtn_create(lv_scr_act(), NULL);              //Create tower object

   lv_obj_set_size(towers_obj[i], 20, 20);          //Sets size of tower object

   lv_obj_set_free_num(towers_obj[i], i);           //Sets free number of the tower object to i
 }

 lv_obj_align(towers_obj[0], tiles_obj[0], LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);      //Aligns bottom-left tower
 lv_obj_align(towers_obj[1], tiles_obj[2], LV_ALIGN_IN_BOTTOM_RIGHT, 10, 0);    //Aligns bottom-middle tower
 lv_obj_align(towers_obj[2], tiles_obj[5], LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);     //Aligns botttom-right tower
 lv_obj_align(towers_obj[3], tiles_obj[12], LV_ALIGN_IN_TOP_LEFT, 0, -10);      //Aligns middle-left tower
 lv_obj_align(towers_obj[4], lv_scr_act(), LV_ALIGN_CENTER, 0, 0);              //Aligns center tower
 lv_obj_align(towers_obj[5], tiles_obj[17], LV_ALIGN_IN_TOP_RIGHT, 0, -10);     //Aligns middle-right tower
 lv_obj_align(towers_obj[6], tiles_obj[30], LV_ALIGN_IN_TOP_LEFT, 0, 0);        //Aligns top-left tower
 lv_obj_align(towers_obj[7], tiles_obj[32], LV_ALIGN_IN_TOP_RIGHT, 10, 0);      //Aligns top-middle tower
 lv_obj_align(towers_obj[8], tiles_obj[35], LV_ALIGN_IN_TOP_RIGHT, 0, 0);       //Aligns top-right tower

 for(int i = 0; i < std::size(blue_balls); i++){    //For every ball in blue_balls
   blue_balls_obj[i] = lv_imgbtn_create(lv_scr_act(), NULL);          //Creates blue ball object
   red_balls_obj[i] = lv_imgbtn_create(lv_scr_act(), NULL);           //Creates red ball object

   lv_obj_set_size(blue_balls_obj[i], 20, 20);      //Sets size of blue ball object
   lv_obj_set_size(red_balls_obj[i], 20, 20);       //Sets size of red ball object

   lv_obj_set_free_num(blue_balls_obj[i], i);       //Sets free number of blue ball object to i
   lv_obj_set_free_num(red_balls_obj[i], i);        //Sets free number of red ball object to i

   lv_imgbtn_set_src(blue_balls_obj[i], LV_BTN_STATE_REL, &bball);    //Sets image of blue ball object when not pressed
   lv_imgbtn_set_src(blue_balls_obj[i], LV_BTN_STATE_PR, &bball_pr);  //Sets image of blue ball object when pressed

   lv_imgbtn_set_src(red_balls_obj[i], LV_BTN_STATE_REL, &rball);     //Sets image of red ball object when not pressed
   lv_imgbtn_set_src(red_balls_obj[i], LV_BTN_STATE_PR, &rball_pr);   //Sets image of red ball object when pressed

   if(is_skills == true){                           //If the skills field is selected
     lv_obj_set_hidden(blue_balls_obj[i], true);    //Hide blue ball object
     lv_obj_set_hidden(red_balls_obj[i], true);     //Hide red ball object
   }
 }

   /*******Ball Alignments*****/
   lv_obj_align(blue_balls_obj[0], towers_obj[2], LV_ALIGN_CENTER, -15, -10);
   lv_obj_align(blue_balls_obj[1], tiles_obj[8], LV_ALIGN_CENTER, 20, 0);
   lv_obj_align(blue_balls_obj[2], towers_obj[4], LV_ALIGN_CENTER, 0, 18);
   lv_obj_align(blue_balls_obj[3], towers_obj[4], LV_ALIGN_CENTER, -18, 0);
   lv_obj_align(blue_balls_obj[4], towers_obj[8], LV_ALIGN_CENTER, -15, 10);

   lv_obj_align(red_balls_obj[0], towers_obj[0], LV_ALIGN_CENTER, 15, -10);
   lv_obj_align(red_balls_obj[1], towers_obj[4], LV_ALIGN_CENTER, 18, 0);
   lv_obj_align(red_balls_obj[2], towers_obj[4], LV_ALIGN_CENTER, 0, -18);
   lv_obj_align(red_balls_obj[3], tiles_obj[26], LV_ALIGN_CENTER, 20, 0);
   lv_obj_align(red_balls_obj[4], towers_obj[6], LV_ALIGN_CENTER, 15, 10);
   /***************************/

   for(int i = 0; i < std::size(red_balls_skills); i++){              //For every red ball in skills
     red_balls_obj_skills[i] = lv_imgbtn_create(lv_scr_act(), NULL);  //Create red skills ball object

     lv_obj_set_size(red_balls_obj_skills[i], 20, 20);                //Sets size of red skills ball object

     lv_obj_set_free_num(red_balls_obj_skills[i], i);                 //Sets free num of red skills ball to i

     lv_imgbtn_set_src(red_balls_obj_skills[i], LV_BTN_STATE_REL, &rball);      //Sets image of red skills ball when not pressed
     lv_imgbtn_set_src(red_balls_obj_skills[i], LV_BTN_STATE_PR, &rball_pr);    //Sets image of red skills ball when pressed


     if(is_skills == false){                        //If the skills field is not selected
       lv_obj_set_hidden(red_balls_obj_skills[i], true);              //Hide the red skills ball
     }
   }

   /*Red Skills Ball Alignments*/
   lv_obj_align(red_balls_obj_skills[0], tiles_obj[2], LV_ALIGN_CENTER, 20, -20);
   lv_obj_align(red_balls_obj_skills[1], towers_obj[2], LV_ALIGN_CENTER, 0, -30);
   lv_obj_align(red_balls_obj_skills[2], tiles_obj[10], LV_ALIGN_CENTER, 0, 0);
   lv_obj_align(red_balls_obj_skills[3], towers_obj[4], LV_ALIGN_CENTER, -40, 40);
   lv_obj_align(red_balls_obj_skills[4], towers_obj[4], LV_ALIGN_CENTER, 0, 40);
   lv_obj_align(red_balls_obj_skills[5], towers_obj[5], LV_ALIGN_CENTER, 0, 40);
   lv_obj_align(red_balls_obj_skills[6], towers_obj[4], LV_ALIGN_CENTER, -40, 0);
   lv_obj_align(red_balls_obj_skills[7], towers_obj[4], LV_ALIGN_CENTER, 40, 0);
   lv_obj_align(red_balls_obj_skills[8], towers_obj[4], LV_ALIGN_CENTER, -40, -40);
   lv_obj_align(red_balls_obj_skills[9], towers_obj[4], LV_ALIGN_CENTER, 0, -40);
   lv_obj_align(red_balls_obj_skills[10], towers_obj[5], LV_ALIGN_CENTER, 0, -40);
   lv_obj_align(red_balls_obj_skills[11], tiles_obj[28], LV_ALIGN_CENTER, 0, 0);
   lv_obj_align(red_balls_obj_skills[12], tiles_obj[26], LV_ALIGN_CENTER, 20, -20);
   lv_obj_align(red_balls_obj_skills[13], towers_obj[8], LV_ALIGN_CENTER, 0, 30);
   /*****************************/

   robot = lv_imgbtn_create(lv_scr_act(), NULL);    //Creates robot object
   lv_obj_set_size(robot, 40, 40);                  //Sets size of robot object
   lv_imgbtn_set_src(robot, LV_IMGBTN_STYLE_REL, &robot_img);         //Sets image of robot when not pressed
   lv_imgbtn_set_src(robot, LV_IMGBTN_STYLE_PR, &robot_img_pr);       //Sets image of robot when pressed
 }

 /**
  *
  * refresh_towers
  *
  * Redraws the towers to adjust for the ball on the top of the tower
  */
 void refresh_towers(){
   for(int i = 0; i < std::size(towers); i++){      //For every tower in towers
     if(towers[i].balls.back() == 'r'){             //If the top of the tower has a red ball
       lv_imgbtn_set_src(towers_obj[i], LV_BTN_STATE_REL, &rtower);   //Set the tower image to red tower when not pressed
       lv_imgbtn_set_src(towers_obj[i], LV_BTN_STATE_PR, &rtower_pr); //Set the tower image to red tower when pressed
     }
     else if(towers[i].balls.back() == 'b'){        //If the top of the tower has a blue ball
       lv_imgbtn_set_src(towers_obj[i], LV_BTN_STATE_REL, &btower);   //Set the tower image to blue tower when not pressed
       lv_imgbtn_set_src(towers_obj[i], LV_BTN_STATE_PR, &btower_pr); //Set the tower image to blue tower when pressed
     }
     else if(towers[i].balls.empty()){              //If the tower has no balls
       lv_imgbtn_set_src(towers_obj[i], LV_BTN_STATE_REL, &tower);    //Set the tower image to empty tower when not pressed
       lv_imgbtn_set_src(towers_obj[i], LV_BTN_STATE_PR, &tower_pr);  //Set the tower image to empty tower when pressed
     }
     else{                                          //If none of the conditions above
       lv_imgbtn_set_src(towers_obj[i], LV_BTN_STATE_REL, &tower);    //Set the tower image to empty tower when not pressed
       lv_imgbtn_set_src(towers_obj[i], LV_BTN_STATE_PR, &tower_pr);  //Set the tower image to empty tower when pressed
     }
   }
 }

 /**
  *
  * refresh_balls
  *
  * Resets the balls to account for any balls that have been collected
  */
 void refresh_balls(){
   if(is_skills == false){                          //If the skills field is not selected
     for(int i = 0; i < std::size(blue_balls); i++){//For every blue ball in blue_balls
       lv_obj_set_hidden(blue_balls_obj[i], blue_balls[i].collected); //Hide collected blue balls
       lv_obj_set_hidden(red_balls_obj[i], red_balls[i].collected);   //Hide collected red balls
     }
   }
   else                                             //If the skills field is not selected
     for(int i = 0; i < std::size(red_balls_skills); i++)             //For every red skills ball in red_balls_skills
       lv_obj_set_hidden(red_balls_obj_skills[i], red_balls_skills[i].collected);   //Hide collected red skills balls
 }

 /**
  *
  * assign_universal_vals
  *
  * Assigns universal values to the data of the field elements
  */
 void assign_universal_vals(){
   for(int i = 0; i < std::size(tiles); i++){        //For tile in tiles
     tiles[i].x_coor = (double) ((lv_obj_get_free_num(tiles_obj[i]) / 10) - 0.5);   //Set tile x coordinate to ((free_num / 10) - 0.5)
     tiles[i].y_coor = (double) ((lv_obj_get_free_num(tiles_obj[i]) % 10) - 0.5);   //Set tile y coordinate to ((free_num % 10) - 0.5)
   }


   /*****Tower X Coordinates*****/
   towers[0].x_coor = 0.00;
   towers[1].x_coor = 3.00;
   towers[2].x_coor = 6.00;
   towers[3].x_coor = 0.00;
   towers[4].x_coor = 3.00;
   towers[5].x_coor = 6.00;
   towers[6].x_coor = 0.00;
   towers[7].x_coor = 3.00;
   towers[8].x_coor = 6.00;
   /*****************************/

   /*****Tower Y Coordinates*****/
   towers[0].y_coor = 0.00;
   towers[1].y_coor = 0.00;
   towers[2].y_coor = 0.00;
   towers[3].y_coor = 3.00;
   towers[4].y_coor = 3.00;
   towers[5].y_coor = 3.00;
   towers[6].y_coor = 6.00;
   towers[7].y_coor = 6.00;
   towers[8].y_coor = 6.00;
   /*****************************/

   if(is_skills == false){                           //If the skills field is not selected
     /******Ball X Coordinates*****/
     blue_balls[0].x_coor = 5.50;
     blue_balls[1].x_coor = 3.00;
     blue_balls[2].x_coor = 3.00;
     blue_balls[3].x_coor = 2.75;
     blue_balls[4].x_coor = 5.50;

     red_balls[0].x_coor = 0.50;
     red_balls[1].x_coor = 3.25;
     red_balls[2].x_coor = 3.00;
     red_balls[3].x_coor = 3.00;
     red_balls[4].x_coor = 0.50;
     /*****************************/

     /******Ball Y Coordinates*****/
     blue_balls[0].y_coor = 0.50;
     blue_balls[1].y_coor = 1.50;
     blue_balls[2].y_coor = 2.75;
     blue_balls[3].y_coor = 3.00;
     blue_balls[4].y_coor = 5.50;

     red_balls[0].y_coor = 0.50;
     red_balls[1].y_coor = 3.00;
     red_balls[2].y_coor = 3.25;
     red_balls[3].y_coor = 4.50;
     red_balls[4].y_coor = 5.50;
     /*****************************/

     for(int i = 0; i < std::size(blue_balls); i++){ //For blue ball in blue_balls
       blue_balls[i].color = 'b';                   //Set blue ball color to blue
       red_balls[i].color = 'r';                    //Set red ball color to red
     }
     /**********Tower Balls********/
     towers[0].balls = {'r', 'b'};
     towers[1].balls = {'b', 'r', 'b'};
     towers[2].balls = {'b', 'r'};
     towers[3].balls = {'r', 'b'};
     towers[4].balls = {};
     towers[5].balls = {'b', 'r'};
     towers[6].balls = {'r', 'b'};
     towers[7].balls = {'r', 'b', 'r'};
     towers[8].balls = {'b', 'r'};
     /*****************************/

     for(int i = 0; i < std::size(blue_balls); i++){ //For every blue ball in blue_balls
       blue_balls[i].collected = false;             //Set blue ball to not collected
       red_balls[i].collected = false;              //Set red ball to not collected
     }
   }
     else{                                           //If the skills field is selected
       for(int i = 0; i < std::size(red_balls_skills); i++){           //For every red skills ball in red_balls_skills
         red_balls_skills[i].color = 'r';           //Set red skills ball color to red
       }
       /**********Tower Balls********/
       towers[0].balls = {'b', 'b'};
       towers[1].balls = {'b'};
       towers[2].balls = {'b', 'b'};
       towers[3].balls = {'b'};
       towers[4].balls = {'b', 'b', 'b'};
       towers[5].balls = {'b'};
       towers[6].balls = {'b', 'b'};
       towers[7].balls = {'b'};
       towers[8].balls = {'b', 'b'};
       /*****************************/
       for(int i = 0; i < std::size(red_balls_skills); i++)            //For every red skills ball in red_balls_skills
         red_balls_skills[i].collected = false;     //Set red skills ball to not collected
     }
 }

 /**
  *
  * auton_menu
  *
  * Displays the autonomous selection menu as well as a tiled map of the field.
  */

 void auton_menu(){

   prev = "Main Menu";                               //Set previous menu to the main menu

   lv_obj_t * top_red_btn = lv_btn_create(lv_scr_act(), NULL);         //Creates top-red autonomous button
   lv_obj_t * bottom_red_btn = lv_btn_create(lv_scr_act(), NULL);      //Creates bottom-red autonomous button
   lv_obj_t * top_blue_btn = lv_btn_create(lv_scr_act(), NULL);        //Creates top-blue autonomous button
   lv_obj_t * bottom_blue_btn = lv_btn_create(lv_scr_act(), NULL);     //Creates bottom-blue autonomous button
   lv_obj_t * exit_btn = lv_btn_create(lv_scr_act(), NULL);            //Creates exit button
   lv_obj_t * load_btn = lv_btn_create(lv_scr_act(), NULL);            //Creates load button

   lv_obj_t * top_red_btn_label = lv_label_create(top_red_btn, NULL);  //Creates label for the top-red autonomous button
   lv_obj_t * bottom_red_btn_label = lv_label_create(bottom_red_btn, NULL);      //Creates label for the bottom-red autonomous button
   lv_obj_t * top_blue_btn_label = lv_label_create(top_blue_btn, NULL);//Creates label for the top-blue autonomous button
   lv_obj_t * bottom_blue_btn_label = lv_label_create(bottom_blue_btn, NULL);    //Creates label for the bottom-blue autonomous button
   lv_obj_t * exit_btn_label = lv_label_create(exit_btn, NULL);        //Creates label for the exit button
   lv_obj_t * load_btn_label = lv_label_create(load_btn, NULL);        //Creates label for the load button

   lv_label_set_text(top_red_btn_label, "Upper\nRed Side");            //Sets text for top-red autonomous button label
   lv_label_set_text(bottom_red_btn_label, "Lower\nRed Side");         //Sets text for bottom-red autonomous button label
   lv_label_set_text(top_blue_btn_label, "Upper\nBlue Side");          //Sets text for top-blue autonomous button label
   lv_label_set_text(bottom_blue_btn_label, "Lower\nBlue Side");       //Sets text for bottom-blue autonomous button label
   lv_label_set_text(exit_btn_label, "Exit");        //Sets text for exit button label
   lv_label_set_text(load_btn_label, "Load");        //Sets text for load button label

   lv_btn_set_style(top_red_btn, LV_BTN_STYLE_REL, &std_btn);          //Sets the style of the top-red autonomous button when not pressed
   lv_btn_set_style(top_red_btn, LV_BTN_STYLE_PR, &std_btn_pr);        //Sets the style of the top-red autonomous button when pressed
   lv_btn_set_style(bottom_red_btn, LV_BTN_STYLE_REL, &std_btn);       //Sets the style of the bottom-red autonomous button when not pressed
   lv_btn_set_style(bottom_red_btn, LV_BTN_STYLE_PR, &std_btn_pr);     //Sets the style of the bottom-red autonomous button when pressed
   lv_btn_set_style(top_blue_btn, LV_BTN_STYLE_REL, &std_btn);         //Sets the style of the top-blue autonomous button when not pressed
   lv_btn_set_style(top_blue_btn, LV_BTN_STYLE_PR, &std_btn_pr);       //Sets the style of the top-blue autonomous button when pressed
   lv_btn_set_style(bottom_blue_btn, LV_BTN_STYLE_REL, &std_btn);      //Sets the style of the bottom-blue autonomous button when not pressed
   lv_btn_set_style(bottom_blue_btn, LV_BTN_STYLE_PR, &std_btn_pr);    //Sets the style of the bottom-blue autonomous button when  pressed
   lv_btn_set_style(exit_btn, LV_BTN_STYLE_REL, &std_btn);             //Sets the style of the exit button when not pressed
   lv_btn_set_style(exit_btn, LV_BTN_STYLE_PR, &std_btn_pr);           //Sets the style of the exit button when pressed
   lv_btn_set_style(load_btn, LV_BTN_STYLE_REL, &std_btn);             //Sets the style of the load button when not pressed
   lv_btn_set_style(load_btn, LV_BTN_STYLE_PR, &std_btn_pr);           //Sets the style of the load button when pressed

   lv_label_set_style(top_red_btn_label, &label_style);                //Sets the style of the top-red autonomous button label
   lv_label_set_style(bottom_red_btn_label, &label_style);             //Sets the style of the bottom-red autonomous button label
   lv_label_set_style(top_blue_btn_label, &label_style);               //Sets the style of the top-blue autonomous button label
   lv_label_set_style(bottom_blue_btn_label, &label_style);            //Sets the style of the bottom-blue autonomous button label
   lv_label_set_style(exit_btn_label, &label_style);                   //Sets the style of the exit button label
   lv_label_set_style(load_btn_label, &label_style);                   //Sets the style of the load button label

   lv_obj_set_size(top_red_btn, 100, 60);            //Sets size of top-red autonomous button
   lv_obj_set_size(bottom_red_btn, 100, 60);         //Sets size of bottom-red autonomous button
   lv_obj_set_size(top_blue_btn, 100, 60);           //Sets size of top-blue autonomous button
   lv_obj_set_size(bottom_blue_btn, 100, 60);        //Sets size of bottom-blue autonomous button
   lv_obj_set_size(exit_btn, 100, 60);               //Sets size of exit button
   lv_obj_set_size(load_btn, 100, 60);               //Sets size of load button

   lv_obj_align(top_red_btn, lv_scr_act(), LV_ALIGN_IN_TOP_LEFT, 0, 0);          //Aligns top-red autonomous button to the top-left of the screen
   lv_obj_align(bottom_red_btn, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);    //Aligns bottom-red autonomous button to the bottom-left of the screen
   lv_obj_align(top_blue_btn, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, 0, 0);        //Aligns top-blue autonomous button to the top-right of the screen
   lv_obj_align(bottom_blue_btn, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);  //Aligns bottom-blue autonomous button to the bottom-right of the screen
   lv_obj_align(exit_btn, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 0, 0);   //Aligns exit button to the middle-left of the screen
   lv_obj_align(load_btn, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, 0, 0);  //Aligns load button to the middle-right of the screen

   lv_obj_set_free_num(top_red_btn, 1);              //Sets the free num of the top-red autonomous button to 1
   lv_obj_set_free_num(bottom_red_btn, 2);           //Sets the free num of the bottom-red autonomous button to 2
   lv_obj_set_free_num(top_blue_btn, 3);             //Sets the free num of the top-blue autonomous button to 3
   lv_obj_set_free_num(bottom_blue_btn, 4);          //Sets the free num of the bottom-blue autonomous button to 4
   lv_obj_set_free_num(exit_btn, 0);                 //Sets the free num of the exit button to 0

   lv_btn_set_action(top_red_btn, LV_BTN_ACTION_CLICK, auton_select_menu);       //Sets top-red autonomous button action to auton_select_menu
   lv_btn_set_action(bottom_red_btn, LV_BTN_ACTION_CLICK, auton_select_menu);    //Sets bottom-red autonomous button action to auton_select_menu
   lv_btn_set_action(top_blue_btn, LV_BTN_ACTION_CLICK, auton_select_menu);      //Sets top-blue autonomous button action to auton_select_menu
   lv_btn_set_action(bottom_blue_btn, LV_BTN_ACTION_CLICK, auton_select_menu);   //Sets bottom-blue autonomous button action to auton_select_menu
   lv_btn_set_action(exit_btn, LV_BTN_ACTION_CLICK, nav_btn);                    //Sets exit button action to nav_btn
   lv_btn_set_action(load_btn, LV_BTN_ACTION_CLICK, load_init);                  //Sets load button action to load_init

   draw_field();                                                                 //Draws the field
   lv_obj_set_hidden(robot, true);                                               //Hides robot

   assign_universal_vals();                                                      //Assigns universal values to objects

   refresh_towers();                                                             //Refreshes towers
 }

/**
 *
 * custom_path_menu
 *
 * Displays the menu for creating a custom path.
 */
void custom_path_menu(){
  draw_field();                                     //Draws the field

  refresh_towers();                                 //Refreshes towers
  refresh_balls();                                  //Refreshes collected balls

  switch(auton_id){                                 //Tests auton_id
    case 1:                                         //If auton_id = 1
      lv_obj_align(robot, tiles_obj[24], LV_ALIGN_CENTER, 0, 0);      //Align the robot with the tile at index 24
      location << "upper_left_";                    //Add "upper_left_" to location
      robot_data.balls.push_back('r');             //Adds red preload to robot
      break;                                        //Breaks out of auton_id switch statement
    case 2:                                         //If auton_id = 2
      lv_obj_align(robot, tiles_obj[6], LV_ALIGN_CENTER, 0, 0);       //Align the robot with the tile at index 6
      location << "lower_left_";                    //Add "lower_left_" to location
      robot_data.balls.push_back('r');             //Add red preload to robot
      break;                                        //Breaks out of auton_id switch statement
    case 3:                                         //If auton_id = 3
      lv_obj_align(robot, tiles_obj[29], LV_ALIGN_CENTER, 0, 0);      //Align the robot with the tile at index 29
      location << "upper_right_";                   //Add "upper_right_" to location
      robot_data.balls.push_back('b');             //Add blue preload to robot
      break;                                        //Breaks out of auton_id switch statement
    case 4:                                         //If auton_id = 4
      lv_obj_align(robot, tiles_obj[11], LV_ALIGN_CENTER, 0, 0);      //Align the robot with the tile at index 11
      location << "lower_right_";                   //Add "lower_right_" to location
      robot_data.balls.push_back('b');             //Add blue preload to robot
      break;                                        //Breaks out of auton_id switch statement
  }

  /********Tile Free Nums*******/
  lv_obj_set_free_num(tiles_obj[24], 1);
  lv_obj_set_free_num(tiles_obj[6], 2);
  lv_obj_set_free_num(tiles_obj[29], 3);
  lv_obj_set_free_num(tiles_obj[11], 4);
  /*****************************/

  /*******Tower Free Nums*******/
  lv_obj_set_free_num(towers_obj[4], 0);
  lv_obj_set_free_num(towers_obj[0], 1);
  lv_obj_set_free_num(towers_obj[3], 2);
  lv_obj_set_free_num(towers_obj[6], 3);
  lv_obj_set_free_num(towers_obj[2], 4);
  lv_obj_set_free_num(towers_obj[5], 5);
  lv_obj_set_free_num(towers_obj[8], 6);
  /*****************************/

  /********Ball Free Nums*******/
  lv_obj_set_free_num(red_balls_obj[0], 1);
  lv_obj_set_free_num(red_balls_obj[1], 2);
  lv_obj_set_free_num(red_balls_obj[4], 3);
  lv_obj_set_free_num(blue_balls_obj[0], 4);
  lv_obj_set_free_num(blue_balls_obj[3], 5);
  lv_obj_set_free_num(blue_balls_obj[4], 6);
  /*****************************/


    if(auton_id == 3 || auton_id == 4){             //If the robot starts on the blue side
    /******Blue Side Enabled******/
    tiles[3].disabled = false;
    tiles[4].disabled = false;
    tiles[5].disabled = false;
    tiles[9].disabled = false;
    tiles[10].disabled = false;
    tiles[11].disabled = false;
    tiles[15].disabled = false;
    tiles[16].disabled = false;
    tiles[17].disabled = false;
    tiles[21].disabled = false;
    tiles[22].disabled = false;
    tiles[23].disabled = false;
    tiles[27].disabled = false;
    tiles[28].disabled = false;
    tiles[29].disabled = false;
    tiles[33].disabled = false;
    tiles[34].disabled = false;
    tiles[35].disabled = false;

    towers[2].disabled = false;
    towers[4].disabled = false;
    towers[5].disabled = false;
    towers[8].disabled = false;

    blue_balls[0].disabled = false;
    blue_balls[4].disabled = false;

    red_balls[1].disabled = false;
    /*****************************/

    /******Red Side Disabled******/
    tiles[0].disabled = true;
    tiles[2].disabled = true;
    tiles[6].disabled = true;
    tiles[7].disabled = true;
    tiles[8].disabled = true;
    tiles[12].disabled = true;
    tiles[13].disabled = true;
    tiles[14].disabled = true;
    tiles[18].disabled = true;
    tiles[19].disabled = true;
    tiles[20].disabled = true;
    tiles[24].disabled = true;
    tiles[25].disabled = true;
    tiles[26].disabled = true;
    tiles[30].disabled = true;
    tiles[31].disabled = true;
    tiles[32].disabled = true;

    towers[0].disabled = true;
    towers[1].disabled = true;
    towers[3].disabled = true;
    towers[6].disabled = true;
    towers[7].disabled = true;

    blue_balls[1].disabled = true;
    blue_balls[2].disabled = true;
    blue_balls[3].disabled = true;

    red_balls[0].disabled = true;
    red_balls[2].disabled = true;
    red_balls[3].disabled = true;
    red_balls[4].disabled = true;
    /*****************************/
  }
  else{
    /******Red Side Enabled*******/
    tiles[0].disabled = false;
    tiles[2].disabled = false;
    tiles[6].disabled = false;
    tiles[7].disabled = false;
    tiles[8].disabled = false;
    tiles[12].disabled = false;
    tiles[13].disabled = false;
    tiles[14].disabled = false;
    tiles[18].disabled = false;
    tiles[19].disabled = false;
    tiles[20].disabled = false;
    tiles[24].disabled = false;
    tiles[25].disabled = false;
    tiles[26].disabled = false;
    tiles[30].disabled = false;
    tiles[31].disabled = false;
    tiles[32].disabled = false;

    towers[0].disabled = false;
    towers[3].disabled = false;
    towers[4].disabled = false;
    towers[6].disabled = false;

    blue_balls[3].disabled = false;

    red_balls[0].disabled = false;
    red_balls[4].disabled = false;
    /*****************************/

    /******Blue Side Disabled*****/
    tiles[3].disabled = true;
    tiles[4].disabled = true;
    tiles[5].disabled = true;
    tiles[9].disabled = true;
    tiles[10].disabled = true;
    tiles[11].disabled = true;
    tiles[15].disabled = true;
    tiles[16].disabled = true;
    tiles[17].disabled = true;
    tiles[21].disabled = true;
    tiles[22].disabled = true;
    tiles[23].disabled = true;
    tiles[27].disabled = true;
    tiles[28].disabled = true;
    tiles[29].disabled = true;
    tiles[33].disabled = true;
    tiles[34].disabled = true;
    tiles[35].disabled = true;

    towers[1].disabled = true;
    towers[2].disabled = true;
    towers[5].disabled = true;
    towers[7].disabled = true;
    towers[8].disabled = true;

    blue_balls[0].disabled = true;
    blue_balls[1].disabled = true;
    blue_balls[2].disabled = true;
    blue_balls[4].disabled = true;

    red_balls[1].disabled = true;
    red_balls[2].disabled = true;
    red_balls[3].disabled = true;
    /*****************************/
  }


    for(int i = 0; i < std::size(tiles); i++){      //For every tile in tiles
      if(tiles[i].disabled == true){               //If tile is disabled
        lv_imgbtn_set_action(tiles_obj[i], LV_BTN_ACTION_CLICK, disabled_element);  //Display error message if clicked

        lv_imgbtn_set_style(tiles_obj[i], LV_BTN_STATE_REL, &imgbtn_disable);   //Sets style of disabled tile when not pressed
        lv_imgbtn_set_style(tiles_obj[i], LV_BTN_STATE_PR, &imgbtn_disable);    //Sets style of disabled tile when pressed
      }
    }

    for(int i = 0; i < std::size(towers); i++){     //For every tower in towers
      if(towers[i].disabled == true){              //If tower is disabled
        lv_imgbtn_set_action(towers_obj[i], LV_BTN_ACTION_CLICK, disabled_element);   //Display error message if clicked

        lv_imgbtn_set_style(towers_obj[i], LV_BTN_STATE_REL, &imgbtn_disable);  //Sets style of disabled tower when not pressed
        lv_imgbtn_set_style(towers_obj[i], LV_BTN_STATE_PR, &imgbtn_disable);   //Sets style of disabled tower when pressed
      }
      else{                                         //If tower is not disabled
        lv_imgbtn_set_action(towers_obj[i], LV_BTN_ACTION_CLICK, tower_menu);   //Sets action of tower to tower_menu
      }
    }

  for(int i = 0; i < std::size(red_balls); i++){    //For red ball in red_balls
    if(red_balls[i].disabled == true){             //If red ball is disabled
      lv_imgbtn_set_action(red_balls_obj[i], LV_BTN_ACTION_CLICK, disabled_element);    //Display error message if clicked

      lv_imgbtn_set_style(red_balls_obj[i], LV_BTN_STATE_REL, &imgbtn_disable); //Sets style of disabled red ball when not pressed
      lv_imgbtn_set_style(red_balls_obj[i], LV_BTN_STATE_PR, &imgbtn_disable);  //Sets style of disabled red ball when pressed
    }
    else{                                           //If red ball is not disabled
      lv_imgbtn_set_action(red_balls_obj[i], LV_BTN_ACTION_CLICK, ball_relocate);   //Sets action of red ball to ball_relocate
    }
  }

  for(int i = 0; i < std::size(blue_balls); i++){   //For every blue ball in blue_balls
    if(blue_balls[i].disabled == true){            //If blue ball is disabled
      lv_imgbtn_set_action(blue_balls_obj[i], LV_BTN_ACTION_CLICK, disabled_element);   //Display error message if clicked

      lv_imgbtn_set_style(blue_balls_obj[i], LV_BTN_STATE_REL, &imgbtn_disable);//Sets style of disabled blue ball when not pressed
      lv_imgbtn_set_style(blue_balls_obj[i], LV_BTN_STATE_PR, &imgbtn_disable); //Sets style of disabled blue ball when pressed
    }
    else{                                           //If blue ball is not disabled
      lv_imgbtn_set_action(blue_balls_obj[i], LV_BTN_ACTION_CLICK, ball_relocate);    //Sets action of blue ball to ball_relocate
    }
  }

  lv_obj_t * confirm_btn = lv_btn_create(lv_scr_act(), NULL);         //Creates confirmation button
  lv_btn_set_style(confirm_btn, LV_BTN_STYLE_REL, &std_btn);          //Sets style of confirmation button when not pressed
  lv_btn_set_style(confirm_btn, LV_BTN_STYLE_PR, &std_btn_pr);        //Sets style of confirmation button when pressed
  lv_obj_set_size(confirm_btn, 100, 60);            //Sets size of the confirmation button
  lv_obj_align(confirm_btn, lv_scr_act(), LV_ALIGN_IN_BOTTOM_RIGHT, 0, 0);      //Aligns the confirmation button to the bottom-right of the screen
  lv_btn_set_action(confirm_btn, LV_BTN_ACTION_CLICK, save_init);     //Sets action of confirmation button to save_init

  lv_obj_t * confirm_btn_label = lv_label_create(confirm_btn, NULL);  //Creates label for confirmation button
  lv_label_set_text(confirm_btn_label, "Confirm");  //Sets text of the confirmation button label

  motion_profiling_sw = lv_sw_create(lv_scr_act(), NULL);             //Creates switch that enables/disables motion profiling in autonomous
  if(motion_profiling == false)                     //If motion_profiling (globals.cpp) is disabled
    lv_sw_off(motion_profiling_sw);                 //Turns off motion_profiling_sw
  else                                              //If motion_profiling (globals.cpp) is enabled
    lv_sw_on(motion_profiling_sw);                  //Turns on motion_profiling_sw
  lv_sw_set_style(motion_profiling_sw, LV_SW_STYLE_BG, &switch_bg);   //Sets background style of the motion profiling switch
  lv_sw_set_style(motion_profiling_sw, LV_SW_STYLE_INDIC, &switch_indic);       //Sets indicative style of the motion profiling switch
  lv_sw_set_style(motion_profiling_sw, LV_SW_STYLE_KNOB_OFF, &switch_off);      //Sets style of the motion profiling switch when off
  lv_sw_set_style(motion_profiling_sw, LV_SW_STYLE_KNOB_ON, &switch_on);        //Sets style of the motion profiling switch when on
  lv_obj_align(motion_profiling_sw, lv_scr_act(), LV_ALIGN_IN_BOTTOM_LEFT, 15, -60);    //Aligns the motion profiling switch with the bottomleft of the screen
  lv_sw_set_action(motion_profiling_sw, motion_profiling_switch);     //Sets action of the motion profiling switch
}

/**
 *
 * reload_editor
 *
 * Clears the screen and redraws the field with consideration for any value
 * changes.
 */
void reload_editor(){
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  draw_field();                                     //Redraws the field on the screen
  refresh_towers();                                 //Updates the towers
  refresh_balls();                                  //Updates the balls
}

 /**
  *
  * auton_list
  *
  * Displays the names of the currently saved autonomous presets, and allows you
  * to read from or write over them.
  *
  */
 void auton_list(){
   lv_obj_t * tabview = lv_tabview_create(lv_scr_act(), NULL);        //Creates tab view
   lv_obj_t * new_tab = lv_tabview_add_tab(tabview, "Autonomous Presets");      //Createes tab
   for(int i = 0; i < 15; i++){                   //Increment i and run loop 15 times
     lv_obj_t * auton_btn = lv_btn_create(new_tab, NULL);             //Creates new button under new_tab
     lv_obj_t * auton_btn_label = lv_label_create(auton_btn, NULL);   //Creates auton button label

     lv_obj_set_size(auton_btn, LV_HOR_RES / 1.5, 40);                //Sets size of auton button

     lv_obj_set_free_num(auton_btn, i);                               //Sets free number of the button to i

     lv_obj_align(auton_btn, new_tab, LV_ALIGN_IN_TOP_MID, 0, 20 + (60 * i));   //Aligns auton_btn (each button is 60px apart vertically)
     lv_btn_set_style(auton_btn, LV_BTN_STYLE_REL, &std_btn);         //Sets the style of auton_btn when not pressed
     lv_btn_set_style(auton_btn, LV_BTN_STYLE_PR, &std_btn_pr);       //Sets the style of auton_btn when pressed

     if(saveload == "save")
      lv_btn_set_action(auton_btn, LV_BTN_ACTION_CLICK, save_menu);   //Sets action of auton button
     else
      lv_btn_set_action(auton_btn, LV_BTN_ACTION_CLICK, load_menu);   //Sets action of auton_btn
     lv_label_set_text(auton_btn_label, auton_labels[i].c_str());     //Sets text of auton button label to be auton_labels at index i
   }
 }

 /**
  *
  * skills_select
  *
  * Selects the skills autonomous
  *
  */
void skills_select(){
  auton_id = 5;                                     //Sets auton_id (globals.cpp) to 5 (skills)
  config_update();                                  //Updates the configuration file
  lv_obj_clean(lv_scr_act());                       //Clears the screen
  main_menu(NULL);                                  //Returns to the main menu
}

/**
 *
 * split_string
 *
 * Splits a string into a vector of strings, where each item in the vector is
 * a section of the string separated by a delimiter character
 *
 * @param s is the string to be split into a vector
 * @param delim is the character at which to split the string
 * @return a vector of strings derived from the splitting of s
 */
std::vector<std::string> split_string(const std::string &s, char delim){
  std::vector<std::string> result;                  //Defines result vector
  std::stringstream ss (s);                         //Defines stringstream from parameter s
  std::string elem;                                 //Defines string that holds input from stringstream
  while (getline (ss, elem, delim))                 //While stringstream contains items separated by delim
    result.push_back(elem);                         //Push item into result
  return result;                                    //Returns result vector of strings
}

/**
 *
 * join_string
 *
 * Joins a vector of strings into a single string, with each vector item being
 * separated with a delimiter character
 *
 * @param svector is the vector of strings that are joined into one string
 * @param delim is the delimiter character to add to the string
 * @return a single string derived from the contents of svector, with a
 *         delimiter character separating each item
 */
std::string join_string(const std::vector<std::string> &svector, const char delim){
  std::string result;                               //Defines result string
  for(int i = 0; i < svector.size(); i++)           //For every string in svector
    result += svector[i] + delim;                   //Add the current string and delim to svector
  return result;                                    //Returns result string
}

/**
 *
 * get_string_suffix
 *
 * Gets the control suffix from a string (the last two characters)
 *
 * @param s is the string that the suffix is derived from
 * @return the last two characters of s
 */
std::string get_string_suffix(const std::string s){
  std::string suffix;                               //Defines suffix string
  suffix.push_back(s[s.size() - 2]);                //Pushes second to last character in s to suffix
  suffix.push_back(s[s.size() - 1]);                //Pushes last character in s to suffix
  return suffix;                                    //Returns suffix string
}

/**
 *
 * get_string_value
 *
 * Gets the numeric value from a string (Everything except the last two
 * characters)
 *
 * @param s is the string that the value is derived from
 * @return the string s without the last two characters
 */
std::string get_string_value(std::string s){
  s.erase(s.end() - 2, s.end());                    //Removes last two characters from s
  return s;                                         //Returns s
}
