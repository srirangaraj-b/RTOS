#include <zephyr/kernel.h>
#include "lcd.h"

#define LCD_PORT DEVICE_DT_GET(DT_NODELABEL(gpio0))

#define LCD_PIN(num) { .port = LCD_PORT, .pin = (num), .dt_flags = 0 }

static const struct lcd_pins my_lcd_pins = {
	.rs = LCD_PIN(8),
	.e  = LCD_PIN(9),
	.d  = { LCD_PIN(10),    /* D4 */
			LCD_PIN(11),    /* D5 */
			LCD_PIN(12),    /* D6 */
			LCD_PIN(13) },  /* D7 */
};

static const uint8_t heart[8] = {
	0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00
};

int main(void)
{
	lcd_set_pins(&my_lcd_pins);

	if (lcd_init() < 0) {
		printk("LCD init failed\n");
		return 0;
	}

	lcd_create_char(0, heart);

	lcd_set_cursor(0, 0);
	lcd_print("Hello Zephyr!");
	lcd_set_cursor(1, 0);
	lcd_print("I ");
	lcd_write_char(0);               /* custom heart */
	lcd_print(" RTOS");

	k_sleep(K_SECONDS(2));
	lcd_cursor_on();
	lcd_blink_on();
	k_sleep(K_SECONDS(2));
	lcd_cursor_off();
	lcd_blink_off();

	while (1) {
		lcd_scroll_left();
		k_msleep(300);
	}
	return 0;
}