#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#define LED1 DT_ALIAS(led1)
#define LED2 DT_ALIAS(led2)

#define BUTTON1 DT_ALIAS(button1)
#define BUTTON2 DT_ALIAS(button2)

static const struct gpio_dt_spec led_1 = GPIO_DT_SPEC_GET(LED1, gpios);
static const struct gpio_dt_spec led_2 = GPIO_DT_SPEC_GET(LED2, gpios);
static const struct gpio_dt_spec button_1 = GPIO_DT_SPEC_GET(BUTTON1, gpios);
static const struct gpio_dt_spec button_2 = GPIO_DT_SPEC_GET(BUTTON2, gpios);

int main(){

    gpio_pin_configure_dt(&led_1, GPIO_OUTPUT);
    gpio_pin_configure_dt(&led_2, GPIO_OUTPUT);
    gpio_pin_configure_dt(&button_1, GPIO_INPUT);
    gpio_pin_configure_dt(&button_2, GPIO_INPUT);

    bool b1_present_state;
    bool b2_present_state;

    bool b1_past_state = false;
    bool b2_past_state = false;

    bool l1_state = false;
    bool l2_state = false;

    while(1){

        b1_present_state = gpio_pin_get_dt(&button_1);
        b2_present_state = gpio_pin_get_dt(&button_2);

        if (b1_present_state == 0 && b1_past_state == 1){

            l1_state = !l1_state;
            gpio_pin_set_dt(&led_1, l1_state);
            k_msleep(100);
        }

        if (b2_present_state == 0 && b2_past_state == 1){

            l2_state = !l2_state;
            gpio_pin_set_dt(&led_2, l2_state);
            k_msleep(100);
        }

        b1_past_state = b1_present_state;
        b2_past_state = b2_present_state;
    }

    return 0;

}