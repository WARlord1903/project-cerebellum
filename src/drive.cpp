/**
 *
 * @file drive.cpp
 * @author William Reinhardt
 *
 * Contains functions relating to drive functionality.
 */

 #include "main.h"

 /**
  *
  * brake
  *
  * Sets all motors of a given type to perform a hard-brake
  *
  * @param type is the type of motor to brake
  */
 void brake(std::string type){
   for(int i = 0; i < motor_types.size(); i++)      //For every item in motor_types
    if(motor_types[i] == type)                      //If the current item in motor_type is equal to type
      pros::c::motor_move_absolute(motor_ports[i], 0, 0);             //Stop and brake the current motor
 }

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
 void move_voltage(std::string type, int voltage){
   for(int i = 0; i < motor_types.size(); i++)      //For every item in motor_types
     if(motor_types[i] == type && motor_types[i] != "Sorter")         //If the current item in motor_types is equal to type and not equal to "Sorter"
       pros::c::motor_move_voltage(motor_ports[i], voltage);          //Run the current motor at the given voltage
     else if(motor_types[i] == type && motor_types[i] == "Sorter")    //If the current item in motor_types is equal to type and equal to "Sorter"
       pros::c::motor_move_voltage(motor_ports[i], std::abs(voltage));//Run the current motor at the absolute value of the given voltage
 }

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
 void move_degrees(std::string type, int degrees, int speed){
   for(int i = 0; i < motor_types.size(); i++)      //For every ite in motor_types
     if(motor_types[i] == type){                    //If the current item in motor_types is equal to type
      pros::c::motor_tare_position(motor_ports[i]); //Reset the position of the current motor
      pros::c::motor_move_absolute(motor_ports[i], degrees, speed);   //Move the current motor the given number of degrees at the given speed
    }
 }

/*********** WIP ***********/
struct p_loop_args{
  void * type;
  void * degrees;
  void * voltage;
};

struct p_loop_t{
  std::string type;
  int degrees;
  int voltage;
};

void p_loop(void * args){
  struct p_loop_args * params = (p_loop_args*) args;
  std::string type = *(std::string*) params->type;
  int degrees = *(int*) params->degrees;
  int voltage = *(int*) params->voltage;
  std::vector<int8_t> ports;
  /***************************/

  for(int i = 0; i < motor_types.size(); i++)
    if(motor_types[i] == type)
      ports.push_back(motor_ports[i]);

  while(pros::c::motor_get_position(ports[0]) < degrees){
    for(int i = 0; i < ports.size(); i++)
      pros::c::motor_move_voltage(ports[i], voltage - (voltage / (degrees - pros::c::motor_get_position(ports[0]))));
    pros::delay(2);
  }
}

/**
 *
 * control_start
 *
 * Handles the controls/button mapping and the movement of the motors via the
 * controller
 *
 * @param ign ignore
 */
void control_start(void * ign){

  std::vector<bool> toggled;                        //Holds which motors have been toggled
  std::vector<bool> running;                        //Holds which motors are currently running

  for(int i = 0; i < control_labels.size(); i++){   //For every item in control_labels
    toggled.push_back(false);                       //Push false into toggled
    running.push_back(false);                       //Push false into running
  }

  while(1){                                         //While the program is running
    for(int i = 0; i < control_btns.size(); i++){   //For every item in control_btns
      if(master.get_digital(control_btns[i])){      //If the controller gets the current item in control_btns
        running[i] = true;                          //Set the current item in running to true
        if(!control_rev[i])                 //If the current motor is not reversed
          move_voltage(control_motor_type[i], control_speed[i]);      //Run the motor at the selected voltage
        else if(control_rev[i])             //If the current motor is reversed
          move_voltage(control_motor_type[i], -control_speed[i]);     //Run the motor at the negative value of the selected voltage
      }
      else if(!master.get_digital(control_btns[i]) && running[i]){      //If the current item in control_btns is not pressed and the current item in running is true
        move_voltage(control_motor_type[i], 0);     //Stop the current motor
        running[i] = false;                         //Set the current item in running to false
      }
    }
  pros::delay(5);                                   //Delay 5 milliseconds (every time the loop runs through)
  }
}

/**
 *
 * drive
 *
 * Handles the usage of the joysticks and the movement of the drive
 *
 * @param ign ignore
 */
void drive(void * ign){
  std::vector<bool> left_drive_rev;                 //Create a vector that holds reversals on the left side of the drive
  std::vector<bool> right_drive_rev;                //Create a vector that holds reversals on the right side of the drive
  int left_speed;                                   //Holds voltage that the left side of the drive runs at
  int right_speed;                                  //Holds voltage that the right side of the drive runs at

  while(1){
    for(int i = 0; i < motor_ports.size(); i++)     //For every item in motor_ports
      pros::c::motor_set_reversed(motor_ports[i], motor_reversals[i]);//Set current motor to be reversed or not based on the current item in motor_reversals

    int left_x = master.get_analog(ANALOG_LEFT_X);  //Stores value of the x-axis on the left joystick
    int left_y = master.get_analog(ANALOG_LEFT_Y);  //Stores value of the y-axis on the left joystick
    int right_x = master.get_analog(ANALOG_RIGHT_X);//Stores value of the x-axis on the right joystick
    int right_y = master.get_analog(ANALOG_RIGHT_Y);//Stores value of the y-axis on the right joystick

    if(drivesettings == "tank"){                    //If the control model is tank control
      left_speed = left_y;                          //left_speed is set to the value of the y-axis of the left joystick
      right_speed = right_y;                        //right_speed is set to the value of the y-axis of the right joystick
    }
    else if(drivesettings == "arcadeleft"){         //If the control model is arcade control (left joystick)
      left_speed = left_y + left_x;                 //left_speed is set to the value of the y-axis of the left joystick plus the value of the x-axis on the left joystick
      right_speed = left_y - left_x;                //right_speed is set to the value of the y-axis of the left joystick minus the value of the x-axis on the left joystick
    }
    else if(drivesettings == "arcaderight"){        //If the control model is arcade control (right joystick)
      left_speed = right_y + right_x;               //left_speed is set to the value of the y-axis of the right joystick plus the value of the x-axis on the right joystick
      right_speed = right_y - right_x;              //right_speed is set to the value of the y-axis of the right joystick minus the value of the x-axis on the right joystick
    }
    else{                                           //If the control model is split-arcade control
      left_speed = left_y + right_x;                //left_speed is set to the value of the y-axis of the left joystick plus the value of the x-axis on the right joystick
      right_speed = left_y - right_x;               //right_speed is set to the value of the y-axis of the left joystick minus the value of the x-axis on the right joystick
    }
    move_voltage("Left Drive", left_speed * 95);
    move_voltage("Right Drive", right_speed * 95);
    pros::delay(5);                                 //Delay 5 milliseconds every time the loop is run through
  }
}
