idf_component_register(SRCS "STM32Portal.cpp" "./serialhub/serial_hub.c"
                    INCLUDE_DIRS "include" "serialhub"
                    REQUIRES esp_driver_uart
                )
