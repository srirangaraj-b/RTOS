#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>

#define TRIGG DT_ALIAS(trigg)
#define ECHO DT_ALIAS(echo)

static const struct gpio_dt_spec trigg_pin = GPIO_DT_SPEC_GET(TRIGG, gpios);
static const struct gpio_dt_spec echo_pin  = GPIO_DT_SPEC_GET(ECHO, gpios);

int main(){

    int count;
    float distance;

    gpio_pin_configure_dt(&trigg_pin, GPIO_OUTPUT);
    gpio_pin_configure_dt(&echo_pin, GPIO_INPUT);

    while(1)
    {
        count = 0;

        gpio_pin_set_dt(&trigg_pin, 1);
        k_busy_wait(10); //10uS delay
        gpio_pin_set_dt(&trigg_pin, 0);

        while(gpio_pin_get_dt(&echo_pin) == 0);

        while(gpio_pin_get_dt(&echo_pin) == 1)
        {
            k_busy_wait(10);
            count ++;
        }

        distance = (count * 10.0f) / 58.0f;

        printk("Distance: %.2f cm\n", (double)distance);

        k_msleep(100);

    }

    return 0;
}