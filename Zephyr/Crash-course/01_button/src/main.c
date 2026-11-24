#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#define LED DT_ALIAS(my_led)
#define BUTTON DT_ALIAS(my_button)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON, gpios);

int main(){
    gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    gpio_pin_configure_dt(&button, GPIO_INPUT);

    while(1){
        int state = gpio_pin_get_dt(&button);
        if(state == 1){
            gpio_pin_set_dt(&led,1);
            k_msleep(10);
        }
        else{
            gpio_pin_set_dt(&led,0);
            k_msleep(10);
        }


    }
    return 0;
}