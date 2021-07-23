/**
 *
 * @file files.hpp
 * @author William Reinhardt
 *
 * Contains functionality for the SD Card.
 */

 #ifndef FILES_H
 #define FILES_H

#include "main.h"

/**
 *
 * config_read
 *
 * Reads values from a .ini file on the SD Card into variables.
 */
 void config_read();

 /**
  *
  * config_updates
  *
  * Writes values from a .ini file on the SD Card.
  */
 void config_update();

 /**
  *
  * update_controls
  *
  * Clears and rewrites control values
  */
 void update_controls();

 #endif
