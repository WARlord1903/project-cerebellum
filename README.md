# Project Cerebellum

![Main Menu](/screenshots/main_menu.png)

A semi-intuitive graphical settings editor and autonomous selector for the VEX V5 Brain!

## Important Notice

Though this program was written 100% independently, without any external programs (other than PROS, Okapi, and LVGL), the Ini_file class defined in ini.hpp is inspired by [Vini by devvoid](https://github.com/devvoid/vini), who created a similar ini editor.

## Features
* A motor configuration menu where you can alter:
  * Motor ports
  * Motor types
  * Motor reversals
* A sensor configuration menu where you can alter:
  * Sensor ports
  * Sensor types
* A controller configuration menu where you can alter:
  * What button the configuration is assigned to
  * What type of motors the configuration moves
  * What voltage the button moves the motor at (including reversed voltages)
* A drive model configuration menu where you can select whether you want your drive model to be Tank, Arcade (Left), Arcade (Right), or Split-Arcade.
* A complex autonomous editor, that allows the user to select a pre-defined autonomous, or create their own using an on-screen representation of the field.
* 15 custom autonomous slots

## Requirements
* A MicroSD Card
* pros-cli

## Missing Features
* Togglable button configurations
* Proportional-loop motor controls
* Tipping-Point Configurations (Change Up only)

## Known Bugs
* Adding a controller button in the button mapping menu crashes the program unless a motor has already been defined.
  * Workaround: Define a motor beforehand.
