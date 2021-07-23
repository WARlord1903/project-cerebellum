#ifndef TILE_H
#define TILE_H

#include "main.h"

typedef struct {
  double x_coor;
  double y_coor;
  bool disabled = false;
} tile_t;

typedef struct {
  double x_coor;
  double y_coor;
  std::vector<char> balls;
  bool disabled = false;
} tower_t;

typedef struct {
  double x_coor;
  double y_coor;
  std::vector<char> balls;
} robot_t;

typedef struct {
  double x_coor;
  double y_coor;
  char color;
  bool collected;
  bool disabled = false;
} ball_t;

#endif
