/**
 *
 * @file gui.cpp
 * @author William Reinhardt
 *
 * Contains definitions for functions relating to GUI functionality.
 */
 #ifndef GUI_H
 #define GUI_H

 #include "main.h"

 /**
  *
  * nav_btn
  *
  * Changes the shown screen when a button is pressed. The screen shown is
  * dependant upon the free number of the button.
  *
  * @param btn is the button to be pressed.
  */
 static lv_res_t nav_btn(lv_obj_t * btn);

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
 static lv_res_t nav_btnm(lv_obj_t * btn, const char *txt);

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
 static lv_res_t load_main_menu(lv_obj_t * mbox, const char *txt);

 /**
  *
  * motor_rev
  *
  * Reverses a different motor depending on the id of the switch.
  *
  * @param sw is the switch that is pressed to reverse the motor
  */
 static lv_res_t motor_rev(lv_obj_t * sw);

/**
 *
 * change_motor_port
 *
 * Changes motor port based on the selected button
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t change_motor_port(lv_obj_t * btn, const char *txt);


/**
 *
 * change_motor_type
 *
 * Changes motor type based on the ddlist selection
 *
 * @param ddlist is the list of motor types
 */
static lv_res_t change_motor_type(lv_obj_t * ddlist);

/**
 *
 * change_sensor_type
 *
 * Changes sensor type based on the ddlist selection
 *
 * @param ddlist is the list of sensor types
 */
static lv_res_t change_sensor_type(lv_obj_t * ddlist);

/**
 *
 * change_sensor_type
 *
 * Changes sensor location based on the ddlist selection
 *
 * @param ddlist is the list of sensor locations
 */
static lv_res_t change_sensor_location(lv_obj_t * ddlist);

/**
 *
 * change_sensor_port
 *
 * Changes sensor port based on the selected button
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t change_sensor_port(lv_obj_t * btnm, const char * txt);

/**
 *
 * edit_motor
 *
 * Handles the motor editing menu
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t edit_motor(lv_obj_t * btnm, const char *txt);

/**
 *
 * apply_motor
 *
 * Adds motor data to all motor-related vectors.
 *
 * @param kb is the keyboard being used
 */
static lv_res_t apply_motor(lv_obj_t * kb);

/**
 *
 * apply_remove_motor
 *
 * Opens menu that confirms that the user wants to delete the selected motor.
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t apply_remove_motor(lv_obj_t * btnm, const char *txt);

/**
 *
 * delete_motor
 *
 * Deletes data for selected motor from all motor-related vectors.
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t delete_motor(lv_obj_t * mbox, const char *txt);

/**
 *
 * edit_sensor
 *
 * Handles the sensor editing menu
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t edit_sensor(lv_obj_t * btnm, const char *txt);

/**
 *
 * apply_sensor
 *
 * Adds sensor data to all sensor-related vectors.
 *
 * @param kb is the keyboard being used
 */
static lv_res_t apply_sensor(lv_obj_t * kb);

/**
 *
 * apply_remove_sensor
 *
 * Opens menu that confirms that the user wants to delete the selected sensor.
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t apply_remove_sensor(lv_obj_t * btnm, const char *txt);

/**
 *
 * delete_sensor
 *
 * Deletes data for selected sensor from all sensor-related vectors.
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t delete_sensor(lv_obj_t * mbox, const char *txt);

/**
 *
 * edit_controls
 *
 * Handles the button editing menu
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t edit_controls(lv_obj_t * btnm, const char *txt);

/**
 *
 * apply_controls
 *
 * Adds button data to all button-related vectors.
 *
 * @param kb is the keyboard being used
 */
static lv_res_t apply_controls(lv_obj_t * kb);

/**
 *
 * apply_remove_controls
 *
 * Opens menu that confirms that the user wants to delete the selected button.
 *
 * @param btnm is the matrix of buttons that is being pressed
 * @param txt is the text displayed on the pressed button
 */
static lv_res_t apply_remove_controls(lv_obj_t * btnm, const char *txt);
/**
 *
 * delete_controls
 *
 * Deletes data for selected button from all control-related vectors.
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t delete_controls(lv_obj_t * mbox, const char *txt);

/**
 *
 * control_nav
 *
 * Handles navigation within the control editing menu
 *
 * @param btn is the button being pressed
 */
static lv_res_t control_nav(lv_obj_t * btn);

/**
 *
 * disable_button
 *
 * Disables the selected button
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t disable_button(lv_obj_t * mbox, const char * txt);

/**
 *
 * change_button_type
 *
 * Changes what type of motor the button controls based on the ddlist selection
 *
 * @param ddlist is the list of sensor types
 */
static lv_res_t change_btn_type(lv_obj_t * ddlist);
/**
 *
 * button_mapper
 *
 * Opens a menu and waits for a digital input on the controller, then assigns
 * that input to a function on the robot based on the selected on-screen button.
 *
 */
void button_mapper();

/**
 *
 * control_type_menu
 *
 * Opens the menu for changing which type of motor a button controls
 *
 */
void control_type_menu();

/**
 *
 * change_p_loop_status
 *
 * Enables p-loop controls for the selected motor
 *
 * @param cb is the checkbox to be pressed
 */
static lv_res_t change_p_loop_status(lv_obj_t * cb);

/**
 *
 * apply_control_voltage
 *
 * Changes what voltage the selected button runs the motor at
 *
 * @param kb is the keyboard being used
 */
static lv_res_t apply_control_voltage(lv_obj_t * kb);

/**
 *
 * set_control_voltage
 *
 * Opens the menu for changing what voltage the selected button runs the motor
 * at
 *
 * @param ta is the text area being typed on
 */
static lv_res_t set_control_voltage(lv_obj_t * ta);

/**
 *
 * rev_control
 *
 * Reverses the voltage that the button runs the motor at
 *
 * @param cb is the checkbox to be pressed
 */
static lv_res_t rev_control(lv_obj_t * cb);

/**
 *
 * control_params_one
 *
 * Opens the first menu that edits button parameters
 *
 */
void control_params_menu_one();

/**
 *
 * auton_select_menu
 *
 * Allows the user to select whether the selected autonomous routine follows
 * the standard path or if the user wants to create a custom autonomous path
 *
 * @param btn is the button that is being pressed
 */
static lv_res_t auton_select_menu(lv_obj_t * btn);

/**
 *
 * motion_profiling_switch
 *
 * Callback function for the motion profiling switch in the path selector, turns
 * motion profiling on and off
 *
 * @param sw is the switch to be pressed.
 */
static lv_res_t motion_profiling_switch(lv_obj_t * sw);


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
static lv_res_t battery_override_switch(lv_obj_t * mbox, const char * txt);

/**
 *
 * name_auton
 *
 * Allows the user to name the selected autonomous preset.
 *
 * @param btn is the button that is being pressed
 */
static lv_res_t name_auton(lv_obj_t * mbox, const char * txt);

/**
 *
 * clear_ta
 *
 * Clears the text area associated with the keyboard
 *
 * @param kb is the keyboard displayed on the screen.
 */
static lv_res_t clear_ta(lv_obj_t * kb);
/**
 *
 * save_init
 *
 * Sets saveload (global) to "save" and loads the auton preset list, allowing the
 * user to save over a preset.
 *
 * @param btn is the button being pressed.
 */
static lv_res_t save_init(lv_obj_t * btn);

/**
 *
 * save_confirm
 *
 * Displays a menu asking the user if they want to set the saved autonomous
 * routine as the current autonomous routine.
 *
 * @param kb is the keyboard displayed on the screen.
 */
static lv_res_t save_confirm(lv_obj_t * kb);

/**
 *
 * save_menu
 *
 * Displays an mbox that confirms whether or not the user wants to overwrite the
 * selected autonomous preset.
 *
 * @param btn is the button to be pressed.
 */
static lv_res_t save_menu(lv_obj_t * btn);

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
static lv_res_t save_auton(lv_obj_t * mbox, const char * txt);

/**
 *
 * load_init
 *
 * Sets saveload (global) to "load" and loads the auton preset list, allowing the
 * user to load a preset.
 *
 * @param btn is the button being pressed.
 */
static lv_res_t load_init(lv_obj_t * btn);

/**
 *
 * load_menu
 *
 * Displays an mbox that confirms whether or not the user wants to load the
 * selected autonomous preset.
 *
 * @param btn is the button to be pressed.
 */
static lv_res_t load_menu(lv_obj_t * btn);

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
static lv_res_t load_auton(lv_obj_t* mbox, const char * txt);

/**
 *
 * delete_mbox
 *
 * Reloads the screen and opens the path selector menu.
 *
 * @param mbox is the message box displayed on the screen.
 * @param txt is the text displayed on the buttons inside the message box.
 */
static lv_res_t delete_mbox(lv_obj_t * mbox, const char * txt);

/**
 *
 * disabled_element
 *
 * A callback function for disabled elements, alerting the user that the
 * selected element is disabled (on the opposite side of the field).
 *
 * @param btn is the button to be pressed.
 */
static lv_res_t disabled_element(lv_obj_t * btn);

/**
 *
 * tile_relocate
 *
 * Push back a tile to go to into path_vec, and aligns the robot with the
 * pressed tile in the path selector menu.
 *
 * @param btn is the tile to be pressed
 */
static lv_res_t tile_relocate(lv_obj_t * btn);

/**
 *
 * ball_relocate
 *
 * Push back a ball to go to into path_vec, and aligns the robot with the
 * pressed ball in the path selector menu.
 *
 * @param btn is the ball to be pressed
 */
static lv_res_t ball_relocate(lv_obj_t * btn);

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
static lv_res_t tower_menu_nav(lv_obj_t * btnm, const char * txt);

/**
 *
 * tower_menu
 *
 * Opens a menu for tower operations
 *
 * @param btn is the button to be pressed
 */
static lv_res_t tower_menu(lv_obj_t * btn);

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
static lv_res_t confirm_skills(lv_obj_t * mbox, const char * txt);

/**
 *
 * null
 *
 * Does nothing
 *
 */
void null(void * ign);

/**
 *
 * style_init
 *
 * Defines the styles for all LVGL objects
 */
void style_init();

/**
 *
 * get_terminal
 *
 * Reads a line from the terminal and stores it in curr_label
 *
 * @param ign ignore
 */
void get_terminal(void * ign);

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
static lv_res_t save_auton_terminal(lv_obj_t * mbox, const char *txt);

/**
 *
 * save_confirm_terminal
 *
 * Displays a menu asking the user if they want to set the saved autonomous
 * routine as the current autonomous routine.
 *
 */
void save_confirm_terminal();

/**
 *
 * exit_terminal
 *
 * Exits the terminal and processes curr_label based on the value of terminal_id
 *
 */
void exit_terminal();

/**
 *
 * exit_screen
 *
 * Displays a screen prompting the user to exit the program
 *.
 */
void exit_screen();

/**
 *
 * no_sdcard_menu
 *
 * Displays a menu that alerts the user that there is no microSD Card
 * inserted
 *.
 */
void no_sdcard_menu(void * ign);

/**
 *
 * main_menu
 *
 * Displays the main menu, which allows the user to navigate to all other menus.
 */
void main_menu(void * ign);

/**
 *
 * control_menu
 *
 * Displays the control menu, which leads to menus that allow the user to change
 * the control scheme or button mapping.
 */
void control_menu();

/**
 *
 * chassis controls
 *
 * Allows the user to change the control scheme to tank, left arcade, right
 * arcade, or split arcade controls.
 */
void chassis_controls();

/**
 *
 * model_select
 *
 * Allows the user to select what control scheme they would like to use when
 * driving
 *
 */
void model_select();

/**
 *
 * button_mapping_menu
 *
 * Opens a menu that allows the user to change what button performs an action
 * selected from the button matrix.
 */
void button_mapping_menu();

/**
 *
 * get_btn
 *
 * Searches for a digital input from the controller and assigns it to a task
 * based on button_id (ideally called repeatedly using a task).
 */
void get_btn(void * ign);

/**
 *
 * add_controls
 *
 * Displays a menu prompting the user to name the new control configuration
 *
 */
void add_controls();

/**
 *
 * remove_controls
 *
 * Displays a menu allowing the user to remove a button configuration
 *
 */
void remove_controls();

/**
*
* motor_select
*
* Displays the motor selection menu, where you can select a motor to view its
* settings.
*/
void motor_select();

/**
 *
 * motor_settings
 *
 * Displays the motor settings menu for a different motor based on the passed
 * parameter
 *
 * @param motor is the id of the motor to be changed
 */
void motor_settings(int motor);

/**
 *
 * get_temp
 *
 * Updates the label temperature with the temperature of the selected motor in
 * fahrenheit based on curr_motor (ideally called repeatedly in a task).
 *
 * @param ign ignore
 */
void get_temp(void * ign);


/**
 *
 * add_motor
 *
 * Displays a menu that allows the user to name a new motor
 *.
 */
void add_motor();


/**
 *
 * remove_motor
 *
 * Displays a menu that allows the user to select a motor to delete
 *.
 */
void remove_motor();

/**
 *
 * port_menu
 *
 * Displays a button matrix of ports to assign to a motor.
 */
 void port_menu();

/**
*
* sensor_menu
*
* Displays the sensir selection menu, where you can select a sensor to view its
* settings.
*/
void sensor_menu();

/**
 *
 * sensor_settings
 *
 * Displays the sensor settings menu for a different sensor based on the passed
 * parameter
 *
 * @param motor is the id of the sensor to be changed
 */
void sensor_settings(int sensor);

/**
 *
 * sensor_port_menu
 *
 * Displays a button matrix of ports to assign to a sensor.
 */
void sensor_port_menu();

/**
 *
 * get_limit_switch
 *
 * Sets the current text of bumper_pressed (global) to reflect the current state
 * of the switch (ideally called repeatedly in a task)
 */
void get_limit_switch(void * ign);

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
std::string get_button_state(int port);

/**
 *
 * get_optical_data
 *
 * Gets data from the optical sensor (ideally called repeatedly in a task)
 *
 * @param ign ignore
 */
void get_optical_data(void * ign);

/**
 *
 * add_sensor
 *
 * Displays menu that allows the user to name a new sensor
 */
void add_sensor();

/**
 *
 * remove_sensor
 *
 * Displays menu that allows the select a motor to delete
 */
void remove_sensor();

/**
 *
 * select_field
 *
 * Allows the user to select whether they want the normal field or the skills
 * field to be displayed
 */
void select_field();

/**
 *
 * draw_field
 *
 * Displays the field on the screen
 */
 void draw_field();

 /**
  *
  * refresh_towers
  *
  * Redraws the towers to adjust for the ball on the top of the tower
  */
 void refresh_towers();

 /**
  *
  * refresh_balls
  *
  * Resets the balls to account for any balls that have been collected
  */
 void refresh_balls();

 /**
  *
  * assign_universal_vals
  *
  * Assigns universal values to the data of the field elements
  */
 void assign_universal_vals();

 /**
  *
  * auton_menu
  *
  * Displays the autonomous selection menu as well as a tiled map of the field.
  */

 void auton_menu();

/**
 *
 * custom_path_menu
 *
 * Displays the menu for creating a custom path.
 */
void custom_path_menu();

/**
 *
 * reload_editor
 *
 * Clears the screen and redraws the field with consideration for any value
 * changes.
 */
void reload_editor();

 /**
  *
  * auton_list
  *
  * Displays the names of the currently saved autonomous presets, and allows you
  * to read from or write over them.
  *
  */
 void auton_list();

 /**
  *
  * skills_select
  *
  * Selects the skills autonomous
  *
  */
void skills_select();

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
std::vector<std::string> split_string(const std::string &s, char delim);

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
std::string join_string(const std::vector<std::string> &svector, const char delim);

/**
 *
 * get_string_suffix
 *
 * Gets the control suffix from a string (the last two characters)
 *
 * @param s is the string that the suffix is derived from
 * @return the last two characters of s
 */
std::string get_string_suffix(const std::string s);

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
std::string get_string_value(std::string s);

#endif
