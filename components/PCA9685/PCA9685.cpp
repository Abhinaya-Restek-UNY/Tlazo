#include "PCA9685.hpp"
#include <cmath>

PCA9685::PCA9685(i2c_master_bus_handle_t &bus, uint16_t device_address,
                 uint16_t frequency)
    : I2CDevice(bus, device_address) {

  if (frequency < 24)
    frequency = 24;
  if (frequency > 1526)
    frequency = 1526;

  uint8_t prescale =
      (uint8_t)(std::round(25000000.0 / (4096.0 * frequency))) - 1;

  // 2. Put the PCA9685 to sleep (required to change the prescaler)
  this->writeByte(0x00, 0x10);

  // 3. Write the calculated prescale value to the PRE_SCALE register (0xFE)
  this->writeByte(0xFE, prescale);

  // 4. Wake the chip up and enable Auto-Increment for multi-byte writes
  this->writeByte(0x00, 0x20);

  // 5. Allow the internal 25MHz oscillator 5ms to stabilize
  this->delay(5);

  // 6. Set the RESTART bit to resume PWM generation
  this->writeByte(0x00, 0xA0);
}

void PCA9685::set_pwm(PCA9685::PCA9685_CHANNEL channel, uint16_t duty_cycle) {
  uint8_t reg_base =
      (channel == PCA9685::PCA9685_CHANNEL_ALL) ? 0xFA : (0x06 + (4 * channel));

  uint8_t data[4] = {0, 0, 0, 0};

  if (duty_cycle == 0) {
    data[3] = 0x10;
  } else if (duty_cycle >= 4095) {
    data[1] = 0x10;
  } else {
    data[0] = 0x00;
    data[1] = 0x00;
    data[2] = duty_cycle & 0xFF;
    data[3] = (duty_cycle >> 8) & 0x0F;
  }

  this->writeBytes(reg_base, 4, data);
}
