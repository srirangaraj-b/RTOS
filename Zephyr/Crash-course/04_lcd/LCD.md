
16x2 Display:
* 16 columns: character per row
* 2 rows: no of character like
* 32 character: total characters from the 2 rows
* 5x8 pixels: standard character pattern.
* 16 pins: Hardware connections pins

|Pin|Name|Direction|Function|
|---|---|---|---|
|1|VSS|Power|Ground|
|2|VDD|Power|Supply, typically 5V|
|3|VO / V0|Analog|Contrast adjustment|
|4|RS|Input|Register Select|
|5|R/W|Input|Read/Write selection|
|6|E|Input|Enable signal|
|7|D0|I/O|Data bit 0|
|8|D1|I/O|Data bit 1|
|9|D2|I/O|Data bit 2|
|10|D3|I/O|Data bit 3|
|11|D4|I/O|Data bit 4|
|12|D5|I/O|Data bit 5|
|13|D6|I/O|Data bit 6|
|14|D7|I/O|Data bit 7|
|15|A / LED+|Power|Backlight anode|
|16|K / LED-|Power|Backlight cathode|
HERE WE HAVE USED THE 4 BIT CONFIGURATION, FROM D4-D7

 rs_pin=Pin(8), 
 enable_pin=Pin(9), 
 d4_pin=Pin(10), 
 d5_pin=Pin(11), 
 d6_pin=Pin(12), 
 d7_pin=Pin(13) 


THE CONTROLLER IN THE LCD CONTAIN 

|Internal block|Responsibility|
|---|---|
|IR|Instruction Register|
|DR|Data Register|
|DDRAM|Display Data RAM|
|CGRAM|Character Generator RAM|
|CGROM|Character Generator ROM|
|AC|Address Counter|
|BF|Busy Flag|
|Timing generator|Internal timing|
|LCD driver circuits|Drive LCD segments and commons|
### **2.1 Instruction Register (IR)**

The Instruction Register receives commands from your microcontroller.

Examples:

- Clear display
- Return home
- Set cursor position
- Configure display mode
- Configure interface width
- Shift display

### **2.2 Data Register (DR)**

The Data Register receives character data.

For example:

```
lcd_send_data('A');
```

The controller interprets this as character data and displays the corresponding character pattern.

The distinction is controlled using the RS pin.

|RS|Meaning|
|---|---|
|0|Instruction/command|
|1|Character data|

This is one of the most important concepts in the LCD communication protocol.

### **2.3 DDRAM (Display Data RAM)**

DDRAM stores the character codes that should appear on the display.

A 16×2 LCD generally has 80 bytes of DDRAM.

Its memory arrangement is important:

|Display location|DDRAM address|
|---|---|
|Row 1, Column 1|0x00|
|Row 1, Column 2|0x01|
|Row 1, Column 16|0x0F|
|Row 2, Column 1|0x40|
|Row 2, Column 2|0x41|
|Row 2, Column 16|0x4F|

Notice something interesting?

The second row does not start at address `0x10`.

It starts at `0x40`.

This is because the controller's internal DDRAM organization is not the same as the visible display arrangement.

Your driver must account for this.

### **2.4 CGROM (Character Generator ROM)**

CGROM contains predefined character patterns.

When you send an ASCII-compatible character code, the controller uses its character generator to determine which pixels should turn on.

For example:

```
lcd_send_data('A');
```

The controller retrieves the character pattern for `A`.

You do not have to manually generate the pixel pattern for standard characters.

### **2.5 CGRAM (Character Generator RAM)**

CGRAM allows you to create your own characters.

A typical 5×8 character occupies 8 bytes.

For example, you can create:

- Battery symbols
    
- Custom arrows
    
- Degree symbols
    
- Progress indicators
    
- Special icons
    

A standard HD44780 controller provides 64 bytes of CGRAM, enough for eight 5×8 custom characters.

### **2.6 Address Counter (AC)**

The Address Counter maintains the current DDRAM or CGRAM address.

It automatically increments or decrements depending on the entry mode.

For example:

```
lcd_send_command(0x80);
lcd_send_data('A');
lcd_send_data('B');
lcd_send_data('C');
```

The LCD writes the characters sequentially because the address counter automatically advances.

### **2.7 Busy Flag (BF)**

The Busy Flag tells you whether the LCD controller is currently executing an instruction.

- BF = 1: Controller is busy.
    
- BF = 0: Controller is ready.
    

Your driver should avoid sending another instruction while the controller is busy.

You can check the flag through the data interface when using read mode, or use appropriate execution delays.

### **3.1 RS: Register Select**

RS determines whether you are sending an instruction or data.

|RS|Operation|
|---|---|
|0|Command|
|1|Data|

For example:

```
// Send command
gpio_pin_set_dt(&rs, 0);

// Send character
gpio_pin_set_dt(&rs, 1);
```

### **3.2 R/W: Read/Write**

This selects the direction of communication.

|R/W|Operation|
|---|---|
|0|Write to LCD|
|1|Read from LCD|

When writing, your microcontroller sends information to the LCD.

When reading, the LCD sends information to your microcontroller.

For a basic driver, you can connect R/W permanently to GND.

This means you only write to the LCD. You cannot read the busy flag or DDRAM through the interface.

This is perfectly acceptable for most applications.

### **3.3 E: Enable**

This is the synchronization signal.

The LCD reads the data placed on its data pins during the appropriate Enable transition, specifically the falling edge.

A typical write operation:

1. Set RS.
    
2. Set R/W = 0.
    
3. Put data on D4-D7 or D0-D7.
    
4. Set E = 1.
    
5. Wait for the required pulse width.
    
6. Set E = 0.
    
7. Wait for execution time.

### **Complete command reference**

|Instruction|Command|Execution time (approx.)|
|---|---|---|
|Clear display|0x01|1.52 ms|
|Return home|0x02|1.52 ms|
|Entry mode|0x06|37 µs|
|Display ON, cursor OFF|0x0C|37 µs|
|Display ON, cursor ON|0x0E|37 µs|
|Display OFF|0x08|37 µs|
|Cursor left|0x10|37 µs|
|Cursor right|0x14|37 µs|
|Display left|0x18|37 µs|
|Display right|0x1C|37 µs|
|Function set|0x28|37 µs|
|CGRAM address|0x40|37 µs|
|DDRAM row 1 start|0x80|37 µs|
|DDRAM row 2 start|0xC0|37 µs|

These are typical HD44780 datasheet execution times, not universal guarantees for every clone or module. Consult the actual controller datasheet.

```C

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

void lcd_create_char(uint8_t location, const uint8_t pattern[8]);

void lcd_scroll_left(void);
void lcd_scroll_right(void);

```

THESE ARE THE NECESSARY FUNCTION THAT ARE REQUIRED TO USE THE LCD DISPLAY 