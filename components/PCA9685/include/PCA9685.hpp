#pragma once
#include "I2CDevice.hpp"

class PCA9685 : private I2CDevice {

  friend class PCA9685ABWheel;

public:
  enum PCA9685_CHANNEL {
    PCA9685_CHANNEL_0 = 0,
    PCA9685_CHANNEL_1 = 1,
    PCA9685_CHANNEL_2 = 2,
    PCA9685_CHANNEL_3 = 3,
    PCA9685_CHANNEL_4 = 4,
    PCA9685_CHANNEL_5 = 5,
    PCA9685_CHANNEL_6 = 6,
    PCA9685_CHANNEL_7 = 7,
    PCA9685_CHANNEL_8 = 8,
    PCA9685_CHANNEL_9 = 9,
    PCA9685_CHANNEL_10 = 10,
    PCA9685_CHANNEL_11 = 11,
    PCA9685_CHANNEL_12 = 12,
    PCA9685_CHANNEL_13 = 13,
    PCA9685_CHANNEL_14 = 14,
    PCA9685_CHANNEL_15 = 15,
    PCA9685_CHANNEL_ALL = 16,
  };
  PCA9685(i2c_master_bus_handle_t &bus, uint16_t device_address,
          uint16_t frequency);

protected:
  void set_pwm(PCA9685_CHANNEL channel, uint16_t duty_cycle);
};
