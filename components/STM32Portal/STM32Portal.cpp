#include "STM32Portal.hpp"
#include "driver/uart.h"
#include "freertos/idf_additions.h"
#include "serial_hub.h"

#define BUF_SIZE 256

STM23Portal::STM23Portal(gpio_num_t rx, gpio_num_t tx) {
  uart_config_t uart_config = {
      .baud_rate = 115200,
      .data_bits = UART_DATA_8_BITS,
      .parity = UART_PARITY_DISABLE,
      .stop_bits = UART_STOP_BITS_1,
      .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
      .source_clk = UART_SCLK_DEFAULT,
  };

  ESP_ERROR_CHECK(uart_param_config(UART_NUM_2, &uart_config));

  ESP_ERROR_CHECK(
      uart_set_pin(UART_NUM_2, tx, rx, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

  // Install UART driver
  ESP_ERROR_CHECK(
      uart_driver_install(UART_NUM_2, BUF_SIZE, BUF_SIZE, 0, NULL, 0));

  serial_hub_initialize(&this->serial_hub_handle,
                        (write_cb_t)(this->stm32_uart_write_cb), this);

  serial_hub_reserve_memory(&this->serial_hub_handle, 32);

  xTaskCreate(TaskFunction_t(stm32_uart_read_task), "uart_rx", 4096, this, 10,
              &this->uart_rx_task_handle);

  serial_hub_attach_topic(&this->serial_hub_handle, 1, sizeof(MecanumCmd),
                          nullptr, nullptr);

  serial_hub_attach_topic(&this->serial_hub_handle, 2, sizeof(float), nullptr,
                          nullptr);
}

void STM23Portal::stm32_uart_write_cb(void *ctx, uint8_t *data, fsize_t size) {

  uart_write_bytes(UART_NUM_2, data, size);
};

void STM23Portal::stm32_uart_read_task(void *pvParameter) {
  auto _this = static_cast<STM23Portal *>(pvParameter);
  uint8_t rx_buffer[128];

  uart_flush(UART_NUM_2);
  while (1) {
    int len = uart_read_bytes(UART_NUM_2, rx_buffer, sizeof(rx_buffer),
                              portMAX_DELAY);
    if (len > 0) {
      serial_hub_on_read(&_this->serial_hub_handle, rx_buffer, len);
    }
  }
};

void STM23Portal::update_mecanum(float x, float y, float turn) {
  MecanumCmd cmd = {x, y, turn};

  serial_hub_write_topic(&this->serial_hub_handle, 1, (uint8_t *)(&cmd),
                         sizeof(cmd));
}

void STM23Portal::set_suction(float power) {
  serial_hub_write_topic(&this->serial_hub_handle, 2, (uint8_t *)(&power),
                         sizeof(power));
}
