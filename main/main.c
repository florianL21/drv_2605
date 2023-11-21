#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "DRV_2605.h"

static const char *TAG = "Haptics";

#define I2C_PORT I2C_NUM_0

void i2c_setup(){
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 5,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = 4,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 400000,
        .clk_flags = 0,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_PORT, &conf));

    ESP_ERROR_CHECK(i2c_driver_install(I2C_PORT, conf.mode, 0, 0, 0));
}

void app_main(void)
{
    ESP_LOGI(TAG, "Initializing I2C...");
    i2c_setup();
    ESP_LOGI(TAG, "Initializing Haptics driver...");
    DRV2605_autocalibration_inputs_t cal_settings = haptic_init(I2C_PORT, DRV2605_MOTOR_TYPE_LRA);
    // haptic_calculate_ERM_calibration(&cal_settings, 3, 3.3, 19);
    haptic_calculate_LRA_calibration(&cal_settings, 2, 2.3, 100);
    // bool claibration_sucess = haptic_calibrate(I2C_PORT, &cal_settings);
    // if(claibration_sucess) {
    //     ESP_LOGI(TAG, "Calibration sucessful");
    // } else {
    //     ESP_LOGE(TAG, "Calibration error");
    //     return;
    // }
    vTaskDelay(1000 / portTICK_PERIOD_MS);
    haptic_register_dump(I2C_PORT);
    while(true) {
        haptic_click(I2C_PORT);
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}