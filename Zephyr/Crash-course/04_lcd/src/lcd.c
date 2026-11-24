#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>
#include <errno.h>

#include "lcd.h" //its userdefined library 

static struct lcd_pins pins;
#define lcd_rs (pins.rs)
#define lcd_e  (pins.e)
#define lcd_d  (pins.d)


void lcd_set_pins(const struct lcd_pins *p){
    pins = *p
}

//COMMANDS

#define CMD_CLEAR        0x01
#define CMD_HOME         0x02
#define CMD_ENTRY_MODE   0x04
#define CMD_DISPLAY_CTRL 0x08
#define CMD_SHIFT        0x10
#define CMD_FUNC_SET     0x20
#define CMD_SET_CGRAM    0x40
#define CMD_SET_DDRAM    0x80

/* Bits */
#define ENTRY_INC        0x02
#define DISP_ON          0x04
#define CURSOR_ON        0x02
#define BLINK_ON         0x01
#define SHIFT_DISPLAY    0x08
#define SHIFT_RIGHT      0x04
#define FUNC_4BIT_2LINE_5X8 0x28


static uint8_t disp_ctrl = DISP_ON;

static void pulse_enable(void){

    gpio_pin_set_dt(&lcd_e,0);
    k_msleep(10);
    

}