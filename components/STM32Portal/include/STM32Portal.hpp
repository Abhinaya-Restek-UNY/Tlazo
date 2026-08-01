#include "freertos/idf_additions.h"
#include "serial_hub.h"
#include "soc/gpio_num.h"
#include <cstdint>
struct __attribute__((packed)) MecanumCmd {
  float x;
  float y;
  float turn;
};

class STM23Portal {
public:
  STM23Portal(gpio_num_t rx, gpio_num_t tx);

  void update_mecanum(float x, float y, float turn);

  void set_suction(float power);

private:
  serial_hub_handle_t serial_hub_handle;

  TaskHandle_t uart_rx_task_handle;
  static void stm32_uart_read_task(void *_this);

  static void stm32_uart_write_cb(void *_this, uint8_t *data, fsize_t size);
};
