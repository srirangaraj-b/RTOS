#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/devicetree.h>

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(my_led), gpios);

int main(){

    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);

    while(1){
        gpio_pin_set_dt(&led,1);
        k_msleep(500);
        gpio_pin_set_dt(&led,0);
        k_msleep(500);

    }
    return 0;
}