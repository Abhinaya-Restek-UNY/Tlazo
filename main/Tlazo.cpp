#include "Gamepad.hpp"
#include "Mecanum.hpp"
#include "PCA9685.hpp"
#include "PCA9685ABWheel.hpp"
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "nvs_flash.h"
#include <stdio.h>

extern "C" void app_main(void) {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }

  ESP_ERROR_CHECK(ret);
  i2c_master_bus_handle_t bus_hande;
  i2c_master_bus_config_t bus_conf = {
      .i2c_port = I2C_NUM_0,
      .sda_io_num = GPIO_NUM_21,
      .scl_io_num = GPIO_NUM_22,
      .clk_source = I2C_CLK_SRC_DEFAULT,
      .glitch_ignore_cnt = 7,
      .flags = {.enable_internal_pullup = true}};

  ESP_ERROR_CHECK(i2c_new_master_bus(&bus_conf, &bus_hande));
  vTaskDelay(pdMS_TO_TICKS(100));

  PCA9685 pca(bus_hande, 0x40, 1526);
  PCA9685ABWheel fr(&pca, PCA9685::PCA9685_CHANNEL_2,
                    PCA9685::PCA9685_CHANNEL_1);

  PCA9685ABWheel fl(&pca, PCA9685::PCA9685_CHANNEL_3,
                    PCA9685::PCA9685_CHANNEL_4);

  PCA9685ABWheel br(&pca, PCA9685::PCA9685_CHANNEL_6,
                    PCA9685::PCA9685_CHANNEL_5);

  PCA9685ABWheel bl(&pca, PCA9685::PCA9685_CHANNEL_7,
                    PCA9685::PCA9685_CHANNEL_8);
  Gamepad gamepad;
  gamepad.start();
  Gamepad::joy_data_t left_joy;
  Gamepad::joy_data_t right_joy;
  Mecanum mecanum(&fr, &fl, &br, &bl);

  while (!gamepad.is_connected()) {
    printf("Waiting..\n");
    vTaskDelay(10);
  }
  while (1) {
    gamepad.get_l_joy(&left_joy);
    gamepad.get_r_joy(&right_joy);

    mecanum.update(left_joy.x, left_joy.y, (float)right_joy.x / 32768.0);

    vTaskDelay(pdMS_TO_TICKS(5));
  }
}
