#pragma once
#include "PCA9685.hpp"
#include <cstdlib>
class PCA9685ABWheel {

public:
  PCA9685ABWheel(PCA9685 *pca, PCA9685::PCA9685_CHANNEL A,
                 PCA9685::PCA9685_CHANNEL B)
      : pca(pca), A(A), B(B) {
    this->set_direction(0);
  }

  void set_direction(int16_t direction) {
    if (direction == 0) {
      this->pca->set_pwm(this->A, 0);
      this->pca->set_pwm(this->B, 0);
      return;
    }

    uint16_t speed = (uint32_t)abs(direction) >> 3;

    if (direction > 0) {
      this->pca->set_pwm(this->B, 0);
      this->pca->set_pwm(this->A, speed);
    } else {
      this->pca->set_pwm(this->A, 0);
      this->pca->set_pwm(this->B, speed);
    }
  };

private:
  PCA9685 *pca;
  PCA9685::PCA9685_CHANNEL A;
  PCA9685::PCA9685_CHANNEL B;
};
