#include "main.h"

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  style_init();

  if(!pros::usd::is_installed()){
    auton_id = 0;
    no_sdcard_menu(NULL);
  }
  else{
    config_read();
    main_menu(NULL);
  }

  for(int i = 0; i < motor_ports.size(); i++){
    pros::c::motor_set_reversed(motor_ports[i], motor_reversals[i]);
    pros::c::motor_set_encoder_units(motor_ports[i], MOTOR_ENCODER_DEGREES);
    pros::c::motor_set_brake_mode(motor_ports[i], MOTOR_BRAKE_COAST);
  }

  for(int i = 0; i < motor_types.size(); i++){
    if(motor_types[i] == "Left Drive")
      left_drive_ports.push_back(motor_ports[i]);
    if(motor_types[i] == "Right Drive")
      right_drive_ports.push_back(motor_ports[i]);
    if(motor_types[i] == "Left Drive Front Motor")
      top_left_motor = motor_ports[i];
    if(motor_types[i] == "Left Drive Rear Motor")
      bottom_left_motor = motor_ports[i];
    if(motor_types[i] == "Right Drive Front Motor")
      top_right_motor = motor_ports[i];
    if(motor_types[i] == "Right Drive Rear Motor")
      bottom_right_motor = motor_ports[i];
  }

  for(int i = 0; i < sensor_types.size(); i++)
    if(sensor_types[i] == "Optical")
      pros::c::optical_set_led_pwm(sensor_ports[i], 100);

  if(!pros::c::usd_is_installed())
    return;

  okapi::ChassisScales scales({3.25_in, 10.5_in}, imev5GreenTPR * (3.0 /5.0));


  if(left_drive_ports.size() == 1){
    chassis = ChassisControllerBuilder()
        .withMotors(left_drive_ports[0],
                    right_drive_ports[0])
       .withDimensions(AbstractMotor::gearset::green, scales)
        .build();
  }
  if(left_drive_ports.size() == 2){
    chassis = ChassisControllerBuilder()
        .withMotors({left_drive_ports[0], left_drive_ports[1]},
                    {right_drive_ports[0], right_drive_ports[1]})
       .withDimensions(AbstractMotor::gearset::green, scales)
        .build();
  }
  else{
    chassis = ChassisControllerBuilder()
       .withMotors(1, 2)
       .withDimensions(AbstractMotor::gearset::green, scales)
        .build();
  }
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  if(!pros::usd::is_installed())
    return;

  pros::Task check_balls_task(check_balls);
  switch(auton_id){
    case 0:
      break;
    case 1:
      top_red_auton();
      break;
    case 2:
      bottom_red_auton();
      break;
    case 3:
      top_blue_auton();
      break;
    case 4:
      bottom_blue_auton();
      break;
    case 5:
      skills();
      break;
    default:
      custom_auton(custom_paths[auton_id - 6]);
      break;
  }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol(){
  okapi::ChassisScales scales({3.25_in, 10.5_in}, imev5GreenTPR * (3.0 /5.0));
  if(left_drive_ports.size() == 1){
    chassis = ChassisControllerBuilder()
        .withMotors(left_drive_ports[0],
                    right_drive_ports[0])
       .withDimensions(AbstractMotor::gearset::green, scales)
        .build();
  }
  if(left_drive_ports.size() >= 2){
    chassis = ChassisControllerBuilder()
        .withMotors({left_drive_ports[0], left_drive_ports[1]},
                    {right_drive_ports[0], right_drive_ports[1]})
       .withDimensions(AbstractMotor::gearset::green, scales)
        .build();
  }
  else{
    chassis = ChassisControllerBuilder()
       .withMotors(1, 2)
       .withDimensions(AbstractMotor::gearset::green, scales)
        .build();
  }
  pros::Task check_balls_task(check_balls);
  pros::Task drive_task(drive, NULL, TASK_PRIORITY_MAX);
  pros::Task controls_task(control_start);
}
