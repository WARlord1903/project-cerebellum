/**
 *
 * @file intake.cpp
 * @author William Reinhardt
 *
 * Contains functions relating to SD Card functionality.
 */

 #include "main.h"

 /**
  *
  * config_read
  *
  * Reads values from a .ini file on the SD Card into variables.
  */
  void config_read(){

    /************************************ Config ***********************************/
    drivesettings = config.get("controls", "drivesettings", "splitarcade");
    auton_id = std::stoi(config.get("settings", "auton_id", "0"));
    alliance_color = config.get("settings", "alliance_color", "none");
    drive_model = config.get("settings", "drive_model", "skidsteer");

    for(int i = 0; i < 20; i++){
      if((config.get("motor_labels", "motor_label_" + std::to_string(i+1), "NULL")) != "NULL")
        motor_labels.push_back(config.get("motor_labels", "motor_label_" + std::to_string(i+1), "None"));
      else
        break;
    }

    for(int i = 0; i < 20; i++){
      if(config.get("motor_types", "motor_type_" + std::to_string(i+1), "NULL") != "NULL")
        motor_types.push_back(config.get("motor_types", "motor_type_" + std::to_string(i+1), "None"));
      else
        break;
    }

    for(int i = 0; i < 20; i++){
      if((config.get("sensor_labels", "sensor_label_" + std::to_string(i+1), "NULL")) != "NULL")
        sensor_labels.push_back(config.get("sensor_labels", "sensor_label_" + std::to_string(i+1), "None"));
      else
        break;
    }

    for(int i = 0; i < 28; i++){
      if(config.get("sensor_types", "sensor_type_" + std::to_string(i+1), "NULL") != "NULL")
        sensor_types.push_back(config.get("sensor_types", "sensor_type_" + std::to_string(i+1), "None"));
      else
        break;
    }
    /*******************************************************************************/


    /************************************ Ports ************************************/
    for(int i = 0; i < 20; i++){
      if(std::stoi(config.get("motor_ports", "motor_port_" + std::to_string(i+1), "255")) != 255)
        motor_ports.push_back((int8_t) std::stoi(config.get("motor_ports", "motor_port_" + std::to_string(i+1), "1")));
      else
        break;
    }

    for(int i = 0; i < 28; i++){
      if(std::stoi(config.get("sensor_ports", "sensor_port_" + std::to_string(i+1), "255")) != 255)
        sensor_ports.push_back((int8_t) std::stoi(config.get("sensor_ports", "sensor_port_" + std::to_string(i+1), "1")));
      else
        break;
    }
    /*******************************************************************************/

    /******************************* Motor Reversals *******************************/
    for(int i = 0; i < 20; i++){
      if(std::stoi((config.get("reverse", "motor_rev_" + std::to_string(i+1), "255"))) != 255)
        motor_reversals.push_back((bool) std::stoi(config.get("reverse", "motor_rev_" + std::to_string(i+1), "0")));
      else
        break;
    }
    /*******************************************************************************/

    for(int i = 0; i < 28; i++){
      if(config.get("sensor_locations", "sensor_location_" + std::to_string(i+1), "N/A") != "N/A")
        sensor_locations.push_back(config.get("sensor_locations", "sensor_location_" + std::to_string(i+1), "N/A"));
      else
        break;
    }

    /*********************************** Controls **********************************/

    for(int i = 0; i < 20; i++){
      if(config.get("controls", "control_" + std::to_string(i+1), "") != "")
        controls.push_back(config.get("controls", "control_" + std::to_string(i+1), ""));
      else
        break;
    }

    update_controls();
    /*******************************************************************************/

    for(int i = 0; i < 15; i++){
      auton_labels[i] = config.get("auton_labels", "auton" + std::to_string(i + 1) + "label", "Empty");
      custom_paths[i] = config.get("custom_paths", "path" + std::to_string(i + 1), "");
    }
  }

  /**
   *
   * config_update
   *
   * Writes values from a .ini file on the SD Card.
   */
  void config_update(){
    /************************************ Config ***********************************/
    config.set("controls", "drivesettings", drivesettings);
    config.set("settings", "auton_id", std::to_string(auton_id));
    config.set("settings", "alliance_color", alliance_color);
    config.set("settings", "drive_model", drive_model);

    for(int i = 0; i < 20; i++){
      if(i < motor_labels.size())
        config.set("motor_labels", "motor_label_" + std::to_string(i+1), motor_labels[i]);
      else
        config.set("motor_labels", "motor_label_" + std::to_string(i+1),"NULL");
    }

    for(int i = 0; i < 20; i++){
      if(i < motor_types.size())
        config.set("motor_types", "motor_type_" + std::to_string(i+1), motor_types[i]);
      else
        config.set("motor_types", "motor_type_" + std::to_string(i+1), "NULL");
    }

    for(int i = 0; i < 28; i++){
      if(i < sensor_labels.size())
        config.set("sensor_labels", "sensor_label_" + std::to_string(i+1), sensor_labels[i]);
      else
        config.set("sensor_labels", "sensor_label_" + std::to_string(i+1), "NULL");
    }

    for(int i = 0; i < 28; i++){
      if(i < sensor_types.size())
        config.set("sensor_types", "sensor_type_" + std::to_string(i+1), sensor_types[i]);
      else
        config.set("sensor_types", "sensor_type_" + std::to_string(i+1), "NULL");
    }
    /*******************************************************************************/



    /************************************ Ports ************************************/
    for(int i = 0; i < 20; i++){
      if(i < motor_ports.size())
        config.set("motor_ports", "motor_port_" + std::to_string(i+1), std::to_string(motor_ports[i]));
      else
        config.set("motor_ports", "motor_port_" + std::to_string(i+1), "255");
    }

    for(int i = 0; i < 28; i++){
      if(i < sensor_ports.size())
        config.set("sensor_ports", "sensor_port_" + std::to_string(i+1), std::to_string(sensor_ports[i]));
      else
        config.set("sensor_ports", "sensor_port_" + std::to_string(i+1), "255");
    }
    /*******************************************************************************/

    /******************************* Motor Reversals *******************************/
    for(int i = 0; i < 20; i++){
      if(i < motor_reversals.size())
        config.set("reverse", "motor_rev_" + std::to_string(i+1), std::to_string((int) motor_reversals[i]));
      else
        config.set("reverse", "motor_rev_" + std::to_string(i+1), "255");
    }
    /*******************************************************************************/
    for(int i = 0; i < 28; i++){
      if(i < sensor_locations.size())
        config.set("sensor_locations", "sensor_location_" + std::to_string(i+1), sensor_locations[i]);
      else
        config.set("sensor_locations", "sensor_location_" + std::to_string(i+1), "N/A");
    }

    /*********************************** Controls **********************************/
    controls.clear();

    for(int i = 0; i < 20; i++){
      if(i < control_labels.size()){
        std::vector<std::string> control_vec = {control_labels[i], std::to_string(control_btns[i]), control_motor_type[i], std::to_string(control_toggle[i]),
                                                std::to_string(control_rev[i]), std::to_string(control_speed[i]), std::to_string(control_p_speed[i]),
                                                std::to_string(control_p_degrees[i])};
        controls.push_back(join_string(control_vec, ':'));
      }
      else
        break;
    }

    for(int i = 0; i < 20; i++)
      if(i < controls.size())
        config.set("controls", "control_" + std::to_string(i+1), controls[i]);
      else
        config.set("controls", "control_" + std::to_string(i+1), "");
    /*******************************************************************************/

    for(int i = 0; i < 15; i++){
      config.set("auton_labels", "auton" + std::to_string(i + 1) + "label", auton_labels[i]);
      config.set("custom_paths", "path" + std::to_string(i + 1), custom_paths[i]);
    }
    config.save();                                             /// Saves config.ini to the SD Card with updated variables
  }


/**
 *
 * update_controls
 *
 * Clears and rewrites control values
 */
void update_controls(){
  control_labels.clear();
  control_btns.clear();
  control_motor_type.clear();
  control_toggle.clear();
  control_rev.clear();
  control_speed.clear();
  control_p_speed.clear();
  control_p_degrees.clear();
  for(int i = 0; i < controls.size(); i++){
    if(controls[i] != ""){
      std::vector<std::string> control_vec = split_string(controls[i], ':');
      control_labels.push_back(control_vec[0]);
      control_btns.push_back((pros::controller_digital_e_t) std::stoi(control_vec[1]));
      control_motor_type.push_back(control_vec[2]);
      control_toggle.push_back(std::stoi(control_vec[3]));
      control_rev.push_back(std::stoi(control_vec[4]));
      control_speed.push_back(std::stoi(control_vec[5]));
      control_p_speed.push_back(std::stoi(control_vec[6]));
      control_p_degrees.push_back(std::stoi(control_vec[7]));
    }
  }
}
