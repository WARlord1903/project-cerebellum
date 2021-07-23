/**
 *
 * @file auton.cpp
 * @author William Reinhardt
 *
 * Contains functionality for the autonomous routines.
 */

 #include "main.h"

 bool cycle_off = false;
 int scored_balls = 0;

 /**
  *
  * check_balls
  *
  * Actively uses the optical sensors to detect what color balls are being held
  * by the robot
  *
  * @param ign ignore
  */
void check_balls(void * ign){
  while(1){                                         //While the program is running
     for(int i = 0; i < sensor_types.size(); i++){  //For every item in sensor_types
       if(sensor_types[i] == "Optical" && sensor_locations[i] == "Intakes"){    //If current item in sensor_types is equal to "Optical" and the current item in sensor_locations is equal to "Intakes"
         if((pros::c::optical_get_hue(sensor_ports[i]) <= 30 || pros::c::optical_get_hue(sensor_ports[i]) >= 330) && pros::c::optical_get_proximity(sensor_ports[i]) > 230)   //If the ball is red
           balls_carried[0] = 'r';                  //Set ball in intakes to be red
         else if((pros::c::optical_get_hue(sensor_ports[i]) >= 210 && pros::c::optical_get_hue(sensor_ports[i]) <= 270) && pros::c::optical_get_proximity(sensor_ports[i]) > 230)   //IF the ball is blue
           balls_carried[0] = 'b';                 //Set ball in intakes to be blue
         else                                      //If there is no ball in intakes
           balls_carried[0] = ' ';                 //Set ball in intakes to be nonexistant
       }
       else if(sensor_types[i] == "Optical" && sensor_locations[i] == "Sorter"){//If current item in sensor_types is equal to "Optical" and the current item in sensor_locations is equal to "Sorter"
         if((pros::c::optical_get_hue(sensor_ports[i]) <= 30 || pros::c::optical_get_hue(sensor_ports[i]) >= 330) && pros::c::optical_get_proximity(sensor_ports[i]) > 230)   //If the ball is red
           balls_carried[1] = 'r';                  //Set ball in sorter to be red
         else if((pros::c::optical_get_hue(sensor_ports[i]) >= 210 && pros::c::optical_get_hue(sensor_ports[i]) <= 270) && pros::c::optical_get_proximity(sensor_ports[i]) > 230)   //If the ball is blue
           balls_carried[1] = 'b';                  //Set ball in sorter to be blue
         else                                       //If there is no ball in sorter
           balls_carried[1] = ' ';                  //Set ball in sorter to be nonexistant
        }
        else if(sensor_types[i] == "Optical" && sensor_locations[i] == "Top-Roller"){   //If current item in sensor_types is equal to "Optical" and the current item in sensor_locations is equal to "Top-Roller"
          if((pros::c::optical_get_hue(sensor_ports[i]) <= 30 || pros::c::optical_get_hue(sensor_ports[i]) >= 330) && pros::c::optical_get_proximity(sensor_ports[i]) > 230)    //If the ball is red
            balls_carried[2] = 'r';                 //Set ball in top-roller to  be red
          else if((pros::c::optical_get_hue(sensor_ports[i]) >= 210 && pros::c::optical_get_hue(sensor_ports[i]) <= 270) && pros::c::optical_get_proximity(sensor_ports[i]) > 230)    //If the ball is blue
            balls_carried[2] = 'b';                 //Set ball in top-roller to be blue
          else                                      //If there is no ball in top-roller
            balls_carried[2] = ' ';                 //Set ball in top-roller to be non-existant
         }
     }
     pros::delay(5);                                //Delay 5 milliseconds when the loop runs through
  }
}

/**
 *
 * reverse_intakes
 *
 * Reverses the intakes if a ball of the opposite color is detected in the
 * intakes
 *
 * @param ign ignore
 */
void reverse_intakes(void * ign){
  while(1){                                         //While the program is running
    if(balls_carried[0] != alliance_color[0] && balls_carried[0] != ' ')        //If the ball is not your alliance color or non-existant
      move_voltage("Intake", -12000);               //Run the intakes at -12000 mV
      pros::delay(5);                               //Wait 5 milliseconds when the loop runs through
  }
}

/**
 *
 * count_scored
 *
 * Counts how many balls have been scored
 *
 * @param ign ignore
 */
void count_scored(void * ign){
  bool cooldown = false;                            //Creates a cooldown bool
  while(1){                                         //While the program is running
    if(balls_carried[2] == alliance_color[0] && !cooldown){           //If the ball in the top-roller is the alliance color and cooldown is false
      scored_balls++;                               //Increment scored_balls
      cooldown = true;                              //Set cooldown to true
    }
    else if(balls_carried[2] == ' ' && cooldown)    //If there is no ball in the top-roller and cooldown is true
      cooldown = false;                             //Set cooldown to false
    pros::delay(5);                                 //Wait 5 milliseconds when the loop runs through
  }
}

/**
 *
 * end_cycle
 *
 * Ends the cycling function when a ball of the opposite color is detected in
 * the intakes
 *
 * @param ign ignore
 */
void end_cycle(void * ign){
  while(!cycle_off){                                //While cycle_off is false
    if(balls_carried[0] != alliance_color[0] && balls_carried[0] != ' '){       //If a ball of the opposite color is detected in the intakes
      cycle_off = true;                             //Set cycle_off to true
    }
    pros::delay(5);                                 //Wait 5 milliseconds when the loop runs through
  }
}


/**
 *
 * hold_cycle
 *
 * Cycles a tower until a ball of the opposite color is detected
 *
 */
void hold_cycle(){
  move_voltage("Intake", 12000);                    //Set intakes to run at 12000 mV
  chassis->setMaxVelocity(200);                     //Sets maximum chassis velocity to 200 RPM
  pros::Task end_cycle_task(end_cycle);             //Creates end_cycle task
  pros::Task count_scored_task(count_scored);       //Creates count_scored task
  pros::Task reverse_intakes_task(reverse_intakes); //Creates reverse_intakes task
  while(!cycle_off){                                //While cycle_off is false
    move_voltage("Top-Roller", 12000);              //Run the top-roller at 12000 mV
    pros::delay(400);                               //Wait 400 milliseconds
    move_voltage("Sorter", 12000);                  //Run the sorter at 12000 mV
    if(scored_balls >= 2){                          //If 2 balls or more have been scored
      chassis->moveDistance(-2_in);                 //Move back 2 inches
      chassis->waitUntilSettled();                  //Wait until the chassis stops
      move_degrees("Left Drive", 200, 12000);       //Move the Left Drive motors 200 degrees
      move_degrees("Right Drive", 200, 12000);      //Move the Right Drive motors 200 degrees
    }
  }
  while(balls_carried[2] == alliance_color[0] || balls_carried[2] == ' ')       //While the opposite color ball is not in the top-roller
    pros::delay(2);                                 //Wait 2 milliseconds

  move_voltage("Intake", 0);                        //Stop the intakes
  move_voltage("Sorter", 0);                        //Stop the sorter
  move_voltage("Top-Roller", 0);                    //Stop the top-roller
  scored_balls = 0;                                 //Reset score count
}

/**
 *
 * deposit_ball
 *
 * Moves the top-roller enough to deposit a ball
 *
 */
void deposit_ball(){
  move_degrees("Top-Roller", 135, 12000);           //Moves top-roller 125 degrees at max speed
  pros::delay(400);                                 //Delay 400 milliseconds
}

/**
 *
 * deposit_ball
 *
 * Moves the top-roller enough to deposit a ball in the center tower
 *
 */
void deposit_ball_center(){
  move_degrees("Top-Roller", 185, 12000);           //Moves top-roller 185 degrees at max speed
  pros::delay(400);                                 //Delay 200 milliseconds
}

/************* Paths ************/
void upper_left_to_middle(){
  move_voltage("Intake", 12000);
  move_voltage("Sorter", 12000);
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(30_in);
  pros::delay(1500);
  brake("Top-Roller");
  chassis->setMaxVelocity(100);
  chassis->turnAngle(100_deg);
  chassis->waitUntilSettled();
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(30_in);
  pros::delay(1500);
  chassis->setMaxVelocity(100);
  chassis->turnAngle(-102_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
  pros::delay(600);
  move_voltage("Left Drive", 3000);
  move_voltage("Right Drive", 3000);
  pros::delay(500);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  move_voltage("Intake", 0);
}

void upper_right_to_middle(){
  move_voltage("Intake", 12000);
  move_voltage("Sorter", 12000);
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(30_in);
  pros::delay(1500);
  brake("Top-Roller");
  chassis->setMaxVelocity(100);
  chassis->turnAngle(-105_deg);
  chassis->waitUntilSettled();
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(38_in);
  pros::delay(2000);
  chassis->setMaxVelocity(100);
  chassis->turnAngle(105_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
  pros::delay(600);
  move_voltage("Left Drive", 3000);
  move_voltage("Right Drive", 3000);
  pros::delay(500);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  move_voltage("Intake", 0);
}

void lower_left_to_middle(){
  upper_right_to_middle();
}

void lower_right_to_middle(){
  upper_left_to_middle();
}

void upper_left_to_top_tower(){
  move_voltage("Intake", 12000);
  move_voltage("Sorter", 12000);
  chassis->setMaxVelocity(150);
  chassis->moveDistanceAsync(22_in);
  pros::delay(1500);
  chassis->setMaxVelocity(75);
  chassis->turnAngle(-160_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
  pros::delay(600);
  move_voltage("Left Drive", 3000);
  move_voltage("Right Drive", 3000);
  pros::delay(500);
  move_voltage("Intake", -12000);
  pros::delay(500);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
}

void upper_right_to_top_tower(){
  move_voltage("Intake", 12000);
  move_voltage("Sorter", 12000);
  chassis->setMaxVelocity(150);
  chassis->moveDistanceAsync(24_in);
  pros::delay(1500);
  chassis->setMaxVelocity(75);
  chassis->turnAngle(150_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
  pros::delay(500);
  move_voltage("Left Drive", 3000);
  move_voltage("Right Drive", 3000);
  pros::delay(500);
  move_voltage("Intake", -12000);
  pros::delay(500);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
}

void lower_left_to_bottom_tower(){
  upper_right_to_top_tower();
}

void lower_right_to_bottom_tower(){
  upper_left_to_top_tower();
}

void top_left_tower_to_middle_tower(){
  move_degrees("Top-Roller", -120, 12000);
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(-24_in);
  pros::delay(2000);
  chassis->setMaxVelocity(125);
  chassis->turnAngle(-155_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", -9000);
  move_voltage("Right Drive", -9000);
  pros::delay(1750);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  chassis->moveDistanceAsync(67_in);
  pros::delay(2250);
  chassis->setMaxVelocity(125);
  chassis->turnAngle(100_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
  move_voltage("Intake", -12000);
  pros::delay(300);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
}

void top_right_tower_to_middle_tower(){
  move_degrees("Top-Roller", -120, 12000);
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(-24_in);
  pros::delay(2000);
  chassis->setMaxVelocity(125);
  chassis->turnAngle(155_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", -9000);
  move_voltage("Right Drive", -9000);
  pros::delay(1750);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(65_in);
  pros::delay(2500);
  chassis->setMaxVelocity(125);
  chassis->turnAngle(-105_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
  move_voltage("Intake", -12000);
  pros::delay(300);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
}

  void bottom_left_tower_to_middle_tower(){
    top_right_tower_to_middle_tower();
  }

  void bottom_right_tower_to_middle_tower(){
    top_left_tower_to_middle_tower();
  }

  void top_left_tower_to_middle(){
    move_degrees("Top-Roller", -120, 12000);
    chassis->setMaxVelocity(175);
    chassis->moveDistanceAsync(-24_in);
    pros::delay(1500);
    chassis->setMaxVelocity(125);
    chassis->turnAngle(-125_deg);
    chassis->waitUntilSettled();
    move_voltage("Left Drive", -9000);
    move_voltage("Right Drive", -9000);
    pros::delay(2000);
    move_voltage("Left Drive", 0);
    move_voltage("Right Drive", 0);
    chassis->moveDistanceAsync(66_in);
    pros::delay(2250);
    chassis->turnAngle(-105_deg);
    chassis->waitUntilSettled();
    move_voltage("Left Drive", 9000);
    move_voltage("Right Drive", 9000);
    pros::delay(700);
    move_voltage("Left Drive", 3000);
    move_voltage("Right Drive", 3000);
    pros::delay(350);
    move_voltage("Intake", -12000);
    pros::delay(350);
    move_voltage("Intake", 12000);
    move_voltage("Left Drive", 0);
    move_voltage("Right Drive", 0);
  }

  void top_right_tower_to_middle(){
    move_degrees("Top-Roller", -120, 12000);
    chassis->setMaxVelocity(175);
    chassis->moveDistanceAsync(-24_in);
    pros::delay(1500);
    chassis->setMaxVelocity(125);
    chassis->turnAngle(125_deg);
    chassis->waitUntilSettled();
    move_voltage("Left Drive", -9000);
    move_voltage("Right Drive", -9000);
    pros::delay(1750);
    move_voltage("Left Drive", 0);
    move_voltage("Right Drive", 0);
    chassis->setMaxVelocity(175);
    chassis->moveDistanceAsync(66_in);
    pros::delay(2250);
    chassis->setMaxVelocity(125);
    chassis->turnAngle(105_deg);
    chassis->waitUntilSettled();
    move_voltage("Left Drive", 9000);
    move_voltage("Right Drive", 9000);
    pros::delay(700);
    move_voltage("Left Drive", 3000);
    move_voltage("Right Drive", 3000);
    pros::delay(350);
    move_voltage("Intake", -12000);
    pros::delay(350);
    move_voltage("Left Drive", 0);
    move_voltage("Right Drive", 0);
  }

void bottom_left_tower_to_middle(){
  top_right_tower_to_middle();
}

void bottom_right_tower_to_middle(){
  top_left_tower_to_middle();
}

void middle_to_middle_tower(){
  chassis->moveDistance(-24_in);
  chassis->waitUntilSettled();
  chassis->turnAngle(180_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
  pros::delay(1000);
  move_voltage("Intake", 0);
  move_degrees("Sorter", 2960, 12000);
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
}

 void skills(){
  alliance_color = "red";
  move_voltage("Intake", 12000);
  move_voltage("Sorter", 12000);
  move_degrees("Top-Roller", 30, 12000);
  chassis->setMaxVelocity(150);
  chassis->moveDistanceAsync(29_in);
  pros::delay(1500);
  brake("Top-Roller");
  chassis->setMaxVelocity(75);
  chassis->turnAngle(-160_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
  pros::delay(700);
  move_voltage("Left Drive", 3000);
  move_voltage("Right Drive", 3000);
  move_voltage("Intake", -12000);
  pros::delay(1200);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  deposit_ball();
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(-24_in);
  pros::delay(1500);
  chassis->setMaxVelocity(75);
  chassis->turnAngle(-60_deg);
  chassis->waitUntilSettled();
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(-18_in);
  pros::delay(1500);
  chassis->setMaxVelocity(75);
  chassis->turnAngle(-100_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", -7000);
  move_voltage("Right Drive", -7000);
  pros::delay(1500);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  move_voltage("Intake", 12000);
  chassis->setMaxVelocity(150);
  chassis->moveDistanceAsync(66_in);
  pros::delay(2250);
  chassis->setMaxVelocity(75);
  chassis->turnAngle(100_deg);
  chassis->waitUntilSettled();
  move_voltage("Sorter", 12000);
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
  pros::delay(700);
  move_voltage("Left Drive", 1500);
  move_voltage("Right Drive", 1500);
  move_voltage("Intake", -12000);
  pros::delay(1500);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  deposit_ball();
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(-34_in);
  pros::delay(1500);
  move_voltage("Intake", 12000);
  chassis->setMaxVelocity(125);
  chassis->turnAngle(-65_deg);
  chassis->waitUntilSettled();
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(36_in);
  pros::delay(2750);
  move_voltage("Sorter", 12000);
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
  pros::delay(505);
  move_voltage("Left Drive", 3000);
  move_voltage("Right Drive", 3000);
  move_voltage("Intake", -12000);
  pros::delay(1000);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  deposit_ball();
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(-44_in);
  pros::delay(2000);
  chassis->setMaxVelocity(125);
  chassis->turnAngle(-140_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", -9000);
  move_voltage("Right Drive", -9000);
  pros::delay(1500);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  move_voltage("Intake", 12000);
  chassis->setMaxVelocity(150);
  chassis->moveDistanceAsync(66_in);
  pros::delay(2250);
  chassis->setMaxVelocity(100);
  chassis->turnAngle(105_deg);
  chassis->waitUntilSettled();
  move_voltage("Sorter", 12000);
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
  pros::delay(570);
  move_voltage("Left Drive", 3000);
  move_voltage("Right Drive", 3000);
  pros::delay(1000);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  move_voltage("Intake", -12000);
  deposit_ball();
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(-20_in);
  pros::delay(1500);
  move_voltage("Intake", 12000);
  chassis->setMaxVelocity(125);
  chassis->turnAngle(-100_deg);
  chassis->waitUntilSettled();
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(48_in);
  pros::delay(2000);
  chassis->setMaxVelocity(125);
  chassis->turnAngle(65_deg);
  chassis->waitUntilSettled();
  move_voltage("Sorter", 12000);
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
  pros::delay(400);
  move_voltage("Left Drive", 3000);
  move_voltage("Right Drive", 3000);
  move_voltage("Intake", -12000);
  pros::delay(1000);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  deposit_ball();
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(-24_in);
  pros::delay(1500);
  chassis->setMaxVelocity(125);
  chassis->turnAngle(-65_deg);
  chassis->waitUntilSettled();
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(-20_in);
  pros::delay(1500);
  chassis->setMaxVelocity(125);
  chassis->turnAngle(-100_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", -7000);
  move_voltage("Right Drive", -7000);
  pros::delay(1250);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  move_voltage("Intake", 12000);
  chassis->setMaxVelocity(150);
  chassis->moveDistanceAsync(66_in);
  pros::delay(2250);
  chassis->setMaxVelocity(100);
  chassis->turnAngle(105_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", 9000);
  move_voltage("Right Drive", 9000);
  pros::delay(570);
  move_voltage("Left Drive", 3000);
  move_voltage("Right Drive", 3000);
  move_voltage("Intake", -12000);
  pros::delay(600);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  deposit_ball();
  chassis->setMaxVelocity(175);
  chassis->moveDistanceAsync(-24_in);
  pros::delay(1500);
  chassis->turnAngle(180_deg);
  chassis->waitUntilSettled();
  move_voltage("Left Drive", 12000);
  move_voltage("Right Drive", 12000);
  move_voltage("Intake", -12000);
  pros::delay(600);
  move_voltage("Left Drive", 0);
  move_voltage("Right Drive", 0);
  deposit_ball_center();
  chassis->setMaxVelocity(200);
  chassis->moveDistanceAsync(-24_in);
  pros::delay(1500);
  move_voltage("Left Drive", 12000);
  move_voltage("Right Drive", 12000);
 }
 /********************************/


 /**
  *
  * top_red_auton
  *
  * The default autonomous routine run in the upper red corner (defunct).
  */
 void top_red_auton(){
   upper_left_to_top_tower();
   deposit_ball();
   top_left_tower_to_middle_tower();
   deposit_ball();
   chassis->moveDistance(-8_in);
   chassis->waitUntilSettled();
 }

 /**
  *
  * top_blue_auton
  *
  * The default autonomous routine run in the upper blue corner (defunct).
  */
 void top_blue_auton(){
   upper_right_to_top_tower();
   deposit_ball();
   top_right_tower_to_middle_tower();
   deposit_ball();
   chassis->moveDistance(-8_in);
   chassis->waitUntilSettled();
 }

 /**
  *
  * bottom_red_auton
  *
  * The default autonomous routine run in the lower red corner (defunct).
  */
 void bottom_red_auton(){
   lower_left_to_bottom_tower();
   deposit_ball();
   bottom_left_tower_to_middle_tower();
   deposit_ball();
   chassis->moveDistance(-8_in);
   chassis->waitUntilSettled();
 }

 /**
  *
  * bottom_blue_auton
  *
  * The default autonomous routine run in the lower blue corner (defunct).
  */
 void bottom_blue_auton(){
   lower_right_to_bottom_tower();
   deposit_ball();
   bottom_right_tower_to_middle_tower();
   deposit_ball();
   chassis->moveDistance(-8_in);
   chassis->waitUntilSettled();
 }

/**
 *
 * custom_auton
 *
 * Parses a string of autonomous steps (each step separated by ':') into a vector and
 * processes each step one by one
 *
 * @param s is the string of autonomous steps to be parsed
 */
void custom_auton(const std::string &str){
  std::vector<std::string> auton_split = split_string(str, ':');      //Creates vector of auton steps from str
  move_degrees("Top-Roller", 30, 12000);
  for(int i = 0; i < auton_split.size(); i++){      //For every item in auton_split
    if(auton_split[i] == "lower_left_to_middle")
      lower_left_to_middle();
    if(auton_split[i] == "lower_right_to_middle")
      lower_right_to_middle();
    if(auton_split[i] == "upper_left_to_middle")
      upper_left_to_middle();
    if(auton_split[i] == "upper_right_to_middle")
      upper_right_to_middle();
    else if(auton_split[i] == "lower_left_to_bottom_ball")
      lower_left_to_bottom_tower();
    else if(auton_split[i] == "lower_right_to_bottom_ball")
      lower_right_to_bottom_tower();
    else if(auton_split[i] == "upper_left_to_top_ball")
      upper_left_to_top_tower();
    else if(auton_split[i] == "upper_right_to_top_ball")
      upper_right_to_top_tower();
    else if(auton_split[i] == "middle_to_middle_tower")
      middle_to_middle_tower();
    else if(auton_split[i] == "top_left_tower_to_middle_left_tower")
      top_left_tower_to_middle_tower();
    else if(auton_split[i] == "top_right_tower_to_middle_right_tower")
      top_right_tower_to_middle_tower();
    else if(auton_split[i] == "bottom_left_tower_to_middle_left_tower")
      bottom_left_tower_to_middle_tower();
    else if(auton_split[i] == "bottom_right_tower_to_middle_right_tower")
      bottom_right_tower_to_middle_tower();
    else if (auton_split[i] == "top_left_tower_to_middle")
      top_left_tower_to_middle();
    else if (auton_split[i] == "top_right_tower_to_middle")
      top_right_tower_to_middle();
    else if (auton_split[i] == "bottom_left_tower_to_middle")
      bottom_left_tower_to_middle();
    else if (auton_split[i] == "bottom_right_tower_to_middle")
      bottom_right_tower_to_middle();
    else if(auton_split[i] == "db"){
      if(auton_split[i-1].find("to_middle") != std::string::npos && auton_split[i-1].find("to_middle_tower") == std::string::npos){
        move_voltage("Intake", 0);
        while(pros::millis() < 14250)
          pros::delay(2);
        deposit_ball_center();
        chassis->setMaxVelocity(200);
        chassis->moveDistanceAsync(-24_in);
      }
      else
        deposit_ball();
    }
    else if(auton_split[i] == "ch")
      hold_cycle();
    else if(auton_split[i] == "end"){
      sort_hold = false;
      move_voltage("Left Drive", 0);
      move_voltage("Right Drive", 0);
      move_voltage("Sorter", 0);
      if(auton_split[i-1].find("to_middle") != std::string::npos)
        move_voltage("Intake", -12000);
      else
        move_voltage("Intake", 0);
      chassis->moveDistance(-12_in);
      chassis->waitUntilSettled();
    }
  }
  pros::delay(5);
}
