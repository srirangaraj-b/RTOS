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

// COMMANDS

#define CMD_CLEAR        0x01
#define CMD_HOME         0x02
#define CMD_ENTRY_MODE   0x04
#define CMD_DISPLAY_CTRL 0x08
#define CMD_SHIFT        0x10
#define CMD_FUNC_SET     0x20
#define CMD_SET_CGRAM    0x40
#define CMD_SET_DDRAM    0x80

// Bits
#define ENTRY_INC        0x02
#define DISP_ON          0x04
#define CURSOR_ON        0x02
#define BLINK_ON         0x01
#define SHIFT_DISPLAY    0x08
#define SHIFT_RIGHT      0x04
#define FUNC_4BIT_2LINE_5X8 0x28


static uint8_t disp_ctrl = DISP_ON;

static void lcd_init(void)
{
    const struct gpio_dt_spec *list[] = {
        &lcd_rs,
        &lcd_en,
        &lcd_d[0],
        &lcd_d[1],
        &lcd_d[2],
        &lcd_d[3]
    };

    for(size_t i=0; i<ARRAY_SIZE(list); i++)
    {
        if(!gpio_is_ready_dt(list[i]))
        {
            return -ENODEV;
        }
        
        int ret = gpio_pin_configure_dt(list[i], GPIO_OUTPUT_INACTIVE);
        
        if (ret < 0)
        {
            return ret;
        }
    }

    k_msleep(50);

	gpio_pin_set_dt(&lcd_rs, 0);
	write4(0x03);
	k_msleep(5);
	write4(0x03);
	k_busy_wait(150);
	write4(0x03);
	k_busy_wait(150);
	write4(0x02);                    //switch to 4-bit mode
	k_busy_wait(150);

    command(FUNC_4BIT_2LINE_5X8);
	disp_ctrl = DISP_ON;
	command(CMD_DISPLAY_CTRL | disp_ctrl);
	lcd_clear();
	command(CMD_ENTRY_MODE | ENTRY_INC);
	return 0;


}

static void pulse_enable(void)
{
    gpio_pin_set_dt(&lcd_e, 0);
    k_busy_wait(1); //1us
    gpio_pin_set_dt(&lcd_e, 1);
    k_busy_wait(1); //1us
    gpio_pin_set_dt(&lcd_e, 0);
    k_busy_wait(50); //50us
}

static void write4(uint8_t nibble)
{
    for(int i=0; i<4; i++){
        gpio_pin_set_dt(d[i], (nibble >> i)&1 );
    }

    pusle_enable();
}

static void send(uint8_t value, int rs)
{
    gpio_pin_set_dt(&lcd_rs, rs);
    write4(value >> 4);
    write4(value & 0x0F);
}

static void command(uint8_t cmd) 
{
    send(cmd, 0); 
}

static void update_ctrl(void) { command(CMD_DISPLAY_CTRL | disp_ctrl); }

void lcd_display_on(void)  { disp_ctrl |=  DISP_ON;   update_ctrl(); }
void lcd_display_off(void) { disp_ctrl &= ~DISP_ON;   update_ctrl(); }
void lcd_cursor_on(void)   { disp_ctrl |=  CURSOR_ON; update_ctrl(); }
void lcd_cursor_off(void)  { disp_ctrl &= ~CURSOR_ON; update_ctrl(); }
void lcd_blink_on(void)    { disp_ctrl |=  BLINK_ON;  update_ctrl(); }
void lcd_blink_off(void)   { disp_ctrl &= ~BLINK_ON;  update_ctrl(); }


void lcd_write_char(uint8_t data) { send(data, 1); }

void lcd_print(const char *str)
{
	while (str && *str) {
		lcd_write_char((uint8_t)*str++);
	}
}

void lcd_scroll_left(void)  { command(CMD_SHIFT | SHIFT_DISPLAY); }
void lcd_scroll_right(void) { command(CMD_SHIFT | SHIFT_DISPLAY | SHIFT_RIGHT); }

void lcd_create_char(uint8_t location, const uint8_t pattern[8])
{
	location &= 0x07;
	command(CMD_SET_CGRAM | (location << 3));
	for (int i = 0; i < 8; i++) {
		lcd_write_char(pattern[i]);
	}
	command(CMD_SET_DDRAM);          /* back to DDRAM */
}

void lcd_set_cursor(uint8_t row, uint8_t col)
{
	static const uint8_t row_offset[] = { 0x00, 0x40, 0x14, 0x54 };

	if (row >= LCD_ROWS) { row = LCD_ROWS - 1; }
	if (row > 3)         { row = 3; }
	if (col >= LCD_COLS) { col = LCD_COLS - 1; }

	command(CMD_SET_DDRAM | (row_offset[row] + col));
}