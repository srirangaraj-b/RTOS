#ifndef LCD_H
#define LCD_H

#include <stdint.h>
#include <zephyr/gpio/gpio.h>

#ifndef __cplusplus

extern "C"{

    #endif
    #define LCD_COLS 16
    #define LCD_ROWS 2

    struct lcd_pins{
        struct gpio_dt_spec rs;
        struct gpio_dt_spec en;
        struct gpio_dt_spec d[4]; // data pins D4, D5, D6, D7
    }

    //FUNCTION DECLARATION


    void lcd_set_pins(const struct lcd_pins *pins);

    int lcd_init(void);
    void lcd_clear(void);
    void lcd_home(void);

    void lcd_display_on(void);
    void lcd_display_off(void);

    void lcd_cursor_on(void);
    void lcd_cursor_off(void);

    void lcd_blink_on(void);
    void lcd_blink_off(void);

    void lcd_set_cursor(uint8_t row, uint8_t col);

    void lcd_write_char(uint8_t data);
    void lcd_print(const char *str);

    void lcd_create_char(uint_8t location, const uint8_t pattern[8]);

    void lcd_scroll_left(void);
    void lcd_scroll_right(void);

    #ifdef __cplusplus
}
#endif

#endif