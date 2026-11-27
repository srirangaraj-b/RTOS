#include <stdint.h>
#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>


#define EN_PIN DT_ALIAS(en)
#define RS_PIN DT_ALIAS(rs)
#define D4_PIN DT_ALIAS(d4)
#define D5_PIN DT_ALIAS(d5)
#define D6_PIN DT_ALIAS(d6)
#define D7_PIN DT_ALIAS(d7)

#define CLEAR DT_ALIAS(clear)
#define RESET DT_ALIAS(reset)

/* Commands */
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

#define LCD_ROWS 2
#define LCD_COLS 16

static uint8_t disp_ctrl = DISP_ON;

static const struct gpio_dt_spec en_pin = GPIO_DT_SPEC_GET(EN_PIN, gpios);
static const struct gpio_dt_spec rs_pin = GPIO_DT_SPEC_GET(RS_PIN, gpios);
static const struct gpio_dt_spec d4_pin = GPIO_DT_SPEC_GET(D4_PIN, gpios);
static const struct gpio_dt_spec d5_pin = GPIO_DT_SPEC_GET(D5_PIN, gpios);
static const struct gpio_dt_spec d6_pin = GPIO_DT_SPEC_GET(D6_PIN, gpios);
static const struct gpio_dt_spec d7_pin = GPIO_DT_SPEC_GET(D7_PIN, gpios);

static const struct gpio_dt_spec reset_pin = GPIO_DT_SPEC_GET(RESET, gpios);
static const struct gpio_dt_spec clear_pin = GPIO_DT_SPEC_GET(CLEAR, gpios);

static void pulse_enable(void)
{
    gpio_pin_set_dt(&en_pin, 0);
    k_busy_wait(1);
    gpio_pin_set_dt(&en_pin, 1);
    k_busy_wait(1);
    gpio_pin_set_dt(&en_pin, 0);
    k_busy_wait(50);
}

static void write_nibble(uint8_t nibble)
{
    gpio_pin_set_dt(&d4_pin, (nibble >> 0)&1 );
    gpio_pin_set_dt(&d5_pin, (nibble >> 1)&1 );
    gpio_pin_set_dt(&d6_pin, (nibble >> 2)&1 );
    gpio_pin_set_dt(&d7_pin, (nibble >> 3)&1 );

    pulse_enable();
}

void send(uint8_t value, int rs_value)
{
    gpio_pin_set_dt(&rs_pin, rs_value);
    write_nibble(value >> 4);
    write_nibble(value & 0x0F);
}

void command(uint8_t cmd)
{
    send(cmd, 0); //0 is for sending command
}

void lcd_clear(void)
{
    command(CMD_CLEAR);
    k_msleep(5);
}

void lcd_init(void)
{
    //setting everything to the output

    gpio_pin_configure_dt(&rs_pin, GPIO_OUTPUT);
    gpio_pin_configure_dt(&en_pin, GPIO_OUTPUT);
    gpio_pin_configure_dt(&d4_pin, GPIO_OUTPUT);
    gpio_pin_configure_dt(&d5_pin, GPIO_OUTPUT);
    gpio_pin_configure_dt(&d6_pin, GPIO_OUTPUT);
    gpio_pin_configure_dt(&d7_pin, GPIO_OUTPUT);

    //setting the display in 4 bit mode,
    gpio_pin_set_dt(&rs_pin, 0); //sending command instead of character
    write_nibble(0x03); //sending the command to  inital 8-bit configuration mode
    k_msleep(5);
    write_nibble(0x03); 
    k_busy_wait(150);
    write_nibble(0x03); 
    k_busy_wait(150);
    write_nibble(0x02);  // 0x02 setting it to 4 bit configuration
    k_busy_wait(150);

    command(FUNC_4BIT_2LINE_5X8);
    k_busy_wait(50);
    disp_ctrl |= DISP_ON;
    command(CMD_DISPLAY_CTRL | disp_ctrl);
    lcd_clear();
    command(CMD_ENTRY_MODE | ENTRY_INC); //making the cmd entry mode and also make the curon go to next line after putting a character

}

void update_cmd(void){
    command(CMD_DISPLAY_CTRL | disp_ctrl);
}

void lcd_home(void)
{
    command(CMD_HOME);
    k_msleep(5);
}

void lcd_display_on(void)
{
    disp_ctrl |= DISP_ON;
    update_cmd();
}
void lcd_display_off(void)
{
    disp_ctrl &= ~DISP_ON;
    update_cmd();
}
void lcd_curson_on(void)
{
    disp_ctrl |= CURSOR_ON;
    update_cmd();
}
void lcd_curson_off(void)
{
    disp_ctrl &= ~CURSOR_ON;
    update_cmd();
}
void lcd_blink_on(void)
{
    disp_ctrl |= BLINK_ON;
    update_cmd();
}
void lcd_blink_off(void)
{
    disp_ctrl &= ~BLINK_ON;
    update_cmd();
}

void set_cursor(uint8_t row,uint8_t col)
{
    uint8_t row_offset[3] = {0x00, 0x40, 0x14, 0x54};

    if(row >= LCD_ROWS)
    {
        row = LCD_ROWS - 1;
    }
    if(col >= LCD_COLS)
    {
        col = LCD_COLS -1;
    }

    command(CMD_SET_DDRAM | (row_offset[row] + col));
    
}

void lcd_write_char(char ch)
{
    send(ch, 1);
}

void lcd_print(const char* str)
{
    while( str != NULL && *str != '\0'){
        lcd_write_char((uint8_t)*str);
        str++;
    }
}

int main()
{
    lcd_init();

    gpio_pin_configure_dt(&reset_pin, GPIO_INPUT);
    gpio_pin_configure_dt(&clear_pin, GPIO_INPUT);

        
    set_cursor(0,0);
    lcd_print("Hello World");
    set_cursor(1,0);
    lcd_print("Welcome Home");
    while(1){
        printk("Hello LCD Init !!!\n");
        k_msleep(1000);

        int clr_val = gpio_pin_get_dt(&clear_pin);
        if(clr_val == 0)
        {
            lcd_clear();
        }

        int res_val = gpio_pin_get_dt(&reset_pin);
        if(res_val == 0)
        {
            lcd_clear();
            set_cursor(0,0);
            lcd_print("Creation Sri Ranga Raj");
        }


    }
    return 0;
}


