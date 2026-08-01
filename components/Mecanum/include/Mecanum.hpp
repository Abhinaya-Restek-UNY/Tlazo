#pragma once
#include "PCA9685ABWheel.hpp"
#pragma once

class Mecanum {
public:
  Mecanum(PCA9685ABWheel *fr, PCA9685ABWheel *fl, PCA9685ABWheel *br,
          PCA9685ABWheel *bl);

  void update(int16_t x, int16_t y, double turn);

private:
  double angle_target = 0.0;

  double power = 0.0;

  double hypot = 0.0;

  double fl_val = 0.0;
  double fr_val = 0.0;
  double bl_val = 0.0;
  double br_val = 0.0;

  PCA9685ABWheel *fr;
  PCA9685ABWheel *fl;
  PCA9685ABWheel *br;
  PCA9685ABWheel *bl;
};
