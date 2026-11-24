# Zephyr RTOS

## Zephyr File Structure

```
zephyr-learn/
│
└── project/
    │
    ├── CMakeLists.txt
    ├── prj.conf
    │
    ├── boards/
    │   └── <board>.overlay
    │
    ├── include/
    │   └── app_config.h
    │
    ├── src/
    │   ├── main.c
    │   │
    │   ├── gpio/
    │   │   ├── gpio_app.c
    │   │   └── gpio_app.h
    │   │
    │   ├── adc/
    │   │   ├── adc_app.c
    │   │   └── adc_app.h
    │   │
    │   ├── uart/
    │   │   ├── uart_app.c
    │   │   └── uart_app.h
    │   │
    │   ├── i2c/
    │   ├── spi/
    │   └── pwm/
    │
    └── build/
```

# GPIO

## 1. CMakeLists.txt

```
cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(project_name)

target_sources(app PRIVATE src/main.c)
```

### Understanding the Statements

|Statement|Purpose|
|---|---|
|`cmake_minimum_required()`|Specifies the minimum CMake version|
|`find_package(Zephyr)`|Loads Zephyr's build system|
|`project()`|Defines our application|
|`target_sources()`|Adds application source files|

## 2. prj.conf

This is where we configure which peripheral we are going to use in the application.

```
CONFIG_GPIO=y
```

## 3. boards/app.overlay

This file may be named `app.overlay` if only one board is going to be used. If we use more than one board for porting a single codebase, then we may name it using the board name, such as `<boardname>.overlay`.

A **Devicetree overlay** is an additional Devicetree source file that modifies or extends the base Devicetree for a particular build.

```
/ {
    model = "My Development Board";
    compatible = "example,my-board";

    soc {
        gpio0: gpio@50000000 {
            compatible = "example,gpio";
            reg = <0x50000000 0x400>;
            status = "okay";
        };
    };
};
```

### a. Root Node

```
/ {
};
```

This is the start and ending of the root node. Everything in the hardware description exists inside this node.

### b. Child Node

```
soc {
};
```

This describes the child node inside the root node. `soc` is the node name, generally representing the System on Chip.

### c. Node Label

```
gpio0: gpio@50000000 {
};
```

Here:

- `gpio0` is the node label.
    
- `gpio` is the node name.
    
- `50000000` is the unit address.
    

The label lets us refer to this node elsewhere (in `main.c`).

For example:

```
DT_NODELABEL(gpio0)
```

### d. Properties

```
reg = <0x50000000 0x400>;
status = "okay";
```

|Property|Meaning|
|---|---|
|`reg`|Register address and size|
|`status`|Whether the device is enabled|
|`compatible`|Hardware compatibility identifier|
|`label`|Optional descriptive property|

These properties are interpreted according to Devicetree bindings.

When Zephyr processes the overlay, it merges the changes into the original Devicetree.

The resulting hardware description contains the updated LED node.

The important thing is that you didn't need to modify the original board DTS.

## 4. How Overlay Merging Works

### Base Board DTS

Original hardware description:

```
/ {
    leds {
        led_0 {
            status = "okay";
        };
    };
};
```

### Application Overlay

Your modifications:

```
/ {
    leds {
        led_0 {
            status = "disabled";
        };
    };
};
```

### Merged Devicetree

Result after overlay processing:

```
/ {
    leds {
        led_0 {
            status = "disabled";
        };
    };
};
```

**Notice something important:**

The overlay didn't replace the entire board description. It modified the matching node.

This is called **Devicetree overlay merging**.

### Devicetree Syntax Reference

|Syntax|Meaning|
|---|---|
|`/`|Root node|
|`{ }`|Defines a node's contents|
|`;`|Terminates a property or node declaration|
|`:`|Defines a node label|
|`&`|References an existing node|
|`=`|Assigns a property value|
|`< >`|Defines integer cells|
|`[ ]`|Defines byte arrays|
|`" "`|Defines strings|
|`,`|Separates values|
|`/include/`|Includes another DTS file|
|`/delete-node/`|Deletes a node|
|`/delete-property/`|Deletes a property|
|`/omit-if-no-ref/`|Conditional node omission|

## 5. Adding GPIO Devices

Here is an example.

### a. Adding GPIO-OUT (LED)

```
/ {
    aliases {
        app-led = &led0;
    };
};
```

**What did we do?**

- `aliases` is a special Devicetree node. It helps us create a custom alias for our project.
    
- `app-led` is our custom alias.
    
- `&led0` references the existing board LED node (that is already specified in the original DTS of the board).
    

Then we can call it in the C main program:

```
#define APP_LED_NODE DT_ALIAS(app-led)
```

### b. Adding GPIO-IN (Button)

```
/ {
    aliases {
        app-led = &led0;
        app-button = &sw0;
    };
};
```

Application can use:

```
#define APP_BUTTON_NODE DT_ALIAS(app_button)
```

In the above two examples, we specify the pre-declared buttons and LED (`&led0` and `&sw0`) for our projects.

### c. Custom GPIO Pin Selection

```
#include <zephyr/dt-bindings/gpio/gpio.h>

/ {
    leds {
        compatible = "gpio-leds";

        my_led: led_1 {
            gpios = <&gpioa 5 GPIO_ACTIVE_HIGH>;
        };
    };

    buttons {
        compatible = "gpio-keys";

        my_button: button_1 {
            gpios = <&gpioc 13
                    (GPIO_PULL_DOWN | GPIO_ACTIVE_HIGH)>;
        };
    };

    aliases {
        my_led = &my_led;
        my_button = &my_button;
    };
};
```

Here the import:

```
#include <zephyr/dt-bindings/gpio/gpio.h>
```

Imports the GPIO-related definitions provided by Zephyr.

For example:

|Definition|Meaning|
|---|---|
|`GPIO_ACTIVE_HIGH`|The signal is active when the voltage is high|
|`GPIO_ACTIVE_LOW`|The signal is active when the voltage is low|
|`GPIO_PULL_UP`|Pull-up configuration flag|
|`GPIO_PULL_DOWN`|Pull-down configuration flag|

```
zephyr,user {
    led-gpios = <&gpioa 5 GPIO_ACTIVE_HIGH>;
    button-gpios = <&gpioc 13 GPIO_ACTIVE_LOW>;
};
```

## 6. Understanding `leds` and `buttons`

```
leds {
    compatible = "gpio-leds";
};
```

This is a node name.

**You defined this name yourself.**

### What about `compatible`?

```
compatible = "gpio-leds";
```

`compatible` is a standard Device Tree property.

Its value identifies the type of device represented by the node.

Here:

```
"gpio-leds"
```

Tells Zephyr that this node describes a collection of GPIO-controlled LEDs.

Zephyr uses this compatible string to find the appropriate Device Tree binding.

Think of it this way:

| Part              | Meaning                                               |
| ----------------- | ----------------------------------------------------- |
| `my_gpio_devices` | Your chosen node name                                 |
| `compatible`      | Standard Device Tree property                         |
| `"gpio-leds"`     | Predefined compatible identifier for leds (output)    |
| `"gpio-keys"`     | Predefined compatible identifier for buttons (inputs) |

You can change `leds`, but you shouldn't arbitrarily change `"gpio-leds"`.

The compatible must match an existing Zephyr binding.

## 7. Understanding `my_led: led_1`

This is one of the most important parts.

```
my_led: led_1 {
    gpios = <&gpioa 5 GPIO_ACTIVE_HIGH>;
};
```

There are two separate names here.

### User-defined Label

- `my_led`
    

This is the Devicetree label.

### User-defined Node Name

- `led_1`
    

This is the actual node name.

The colon separates the label from the node name.

### What is a Label?

A label is an identifier that allows you to reference a Devicetree node from somewhere else.

For example:

```
my_led: led_1 {
    gpios = <&gpioa 5 GPIO_ACTIVE_HIGH>;
};
```

Later, you can reference this node using:

```
&my_led
```

For example:

```
aliases {
    my-led = &my_led;
};
```

You could even rename the label:

```
status_led: led_1 {
    gpios = <&gpioa 5 GPIO_ACTIVE_HIGH>;
};
```

Now you must reference it using:

```
&status_led
```

### Remember

- `my_led` is a label.
    
- `led_1` is a node name.
    
- `&my_led` references the node using its label.
    

**This distinction is extremely important when writing overlays.**

## 8. Understanding GPIOs: `gpios`

```
gpios = <&gpioa 5 GPIO_ACTIVE_HIGH>;
```

It describes which physical GPIO pin the LED uses.

Let's break it into pieces.

### GPIO Specification

`**&gpioa**`

GPIO controller.

References the GPIOA controller node in the board's Devicetree.

`**5**`

Pin number.

Selects pin number 5 on that controller. On STM32, this is PA5.

`**GPIO_ACTIVE_HIGH**`

GPIO flag.

Specifies that the signal is active when the pin is HIGH.

### What is `&gpioa`?

This is a reference to a GPIO controller node defined in the underlying board or SoC Devicetree.

For an STM32 board:

```
&gpioa
```

Typically refers to GPIO port A.

For a Raspberry Pi Pico, you would typically reference its GPIO controller using the label provided by its SoC Devicetree, often:

```
&gpio0
```

The exact label depends on the board's Devicetree.

### What does `5` mean?

For STM32:

```
<&gpioa 5 GPIO_ACTIVE_HIGH>
```

Means PA5.

For Raspberry Pi Pico:

```
<&gpio0 18 GPIO_ACTIVE_HIGH>
```

Means GPIO18.

Notice that the controller and pin number are separate.

### What does `GPIO_ACTIVE_HIGH` mean?

It describes the logical active state.

|Flag|Active Condition|
|---|---|
|`GPIO_ACTIVE_HIGH`|Physical HIGH means active|
|`GPIO_ACTIVE_LOW`|Physical LOW means active|

For example, an LED connected between GPIO and GND generally uses active-high logic.

An LED connected between VCC and GPIO generally uses active-low logic.

The electrical circuit determines which one is appropriate.

## 9. Understanding `aliases`

```
aliases {
    my-led = &my_led;
};
```

`aliases` is a special Devicetree node convention recognized by Zephyr.

It lets you give a node a convenient alternative name.

Here:

```
my-led = &my_led;
```

Means:

- `my-led` is the alias name.
    
- `&my_led` references the actual node.
    

You can use that alias in your C code:

```
#define LED_NODE DT_ALIAS(my_led)
```

And retrieve the GPIO specification:

```
static const struct gpio_dt_spec led =
    GPIO_DT_SPEC_GET(LED_NODE, gpios);
```

Notice something interesting:

In Devicetree, you wrote:

```
my-led
```

But in C, you wrote:

```
DT_ALIAS(my_led)
```

Zephyr converts hyphens in property and alias names into underscores for its C macros.

You can create multiple aliases:

```
aliases {
    my-led = &my_led;
    status-led = &my_led;
};
```

Both aliases point to the same node.

## 10. Understanding `zephyr,user`

Now let's examine:

```
zephyr,user {
    my-gpios = <&gpioa 5 GPIO_ACTIVE_HIGH>,
               <&gpioc 13 GPIO_ACTIVE_LOW>;
};
```

This is particularly useful when you're developing your own hardware drivers or applications.

`zephyr,user` is a special convention provided by Zephyr.

It is intended for application-specific Devicetree information.

Unlike `gpio-leds`, it doesn't describe a specific hardware driver.

You can put your own properties inside it.

For example:

```
zephyr,user {
    my-gpios = <&gpioa 5 GPIO_ACTIVE_HIGH>;
    my-uart = <&usart1>;
    my-i2c = <&i2c1>;
};
```

Here:

|Name|Type|
|---|---|
|`zephyr,user`|Zephyr-defined node name|
|`my-gpios`|User-defined property|
|`my-uart`|User-defined property|
|`my-i2c`|User-defined property|

The property names are yours to choose.

However, their values must follow the appropriate Devicetree format.

### What does `my-gpios` contain?

```
my-gpios = <&gpioa 5 GPIO_ACTIVE_HIGH>,
           <&gpioc 13 GPIO_ACTIVE_LOW>;
```

It contains two GPIO specifications.

# src/main.c:

## **Understand the GPIO programming model**

Before writing code, understand the three basic operations.

**GPIO Input**

The microcontroller reads an external electrical signal.

Example: button, switch, digital sensor

**GPIO Output**

The microcontroller drives an electrical signal.

Example: LED, relay control, enable pin

**GPIO Interrupt**

The microcontroller reacts when an input signal changes or reaches a configured level.

Example: button press detection without continuous polling

A typical GPIO application follows this sequence:

1. Identify the GPIO pin through Devicetree.
    
2. Retrieve its GPIO specification in C.
    
3. Check whether the GPIO controller is ready.
    
4. Configure the pin's direction and electrical behavior.
    
5. Read or write the pin.
    
6. Handle errors and timing appropriately.

## **Required header files**

Start your `main.c` with:

```
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/devicetree.h>
```

Let's understand each header.

| Header                  | Why we need it                                         |
| ----------------------- | ------------------------------------------------------ |
| `zephyr/kernel.h`       | Kernel functions, threads, delays, synchronization     |
| `zephyr/drivers/gpio.h` | GPIO structures, configuration, read/write, interrupts |
| `zephyr/devicetree.h`   | Devicetree macros to identify hardware                 |
| `zephyr/device.h`       |                                                        |
```C
#define LED_NODE DT_ALIAS(my_led)
#define BUTTON_NODE DT_ALIAS(my_button)
```

accessing the node names and importing to the c program to use those

```c

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(BUTTON_NODE, gpios);
```

its get the node details from the device tree to the main program 

Conceptually, the resulting structures contain:

**LED structure**

**led**

|Member|Value|
|---|---|
|`port`|GPIOA device|
|`pin`|5|
|`dt_flags`|GPIO_ACTIVE_HIGH|

**Button structure**

**button**

|Member|Value|
|---|---|
|`port`|GPIOC device|
|`pin`|12|
|`dt_flags`|GPIO_ACTIVE_LOW|

These are compile-time initializations. Zephyr generates the necessary references using Device Tree macros.

```c
gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
gpio_pin_configure_dt(&button, GPIO_INPUT);
```

then configuring the set pins as input and output, 

## Output:

```C
gpio_pin_set_dt(&led, 1);
gpio_pin_set_dt(&led, 0);
```

`gpio_pin_set_dt` is used to set the pin high or low

## Input:

```C
int state = gpio_pin_get_dt(&button);
```

`gpio_pin_get_dt`  is used to get the state of the pin, whether if the pin is high or low.

Main Macros:
- `GPIO_DT_SPEC_GET()` retrieves the GPIO specification.
    
- `gpio_pin_configure_dt()` configures the actual GPIO hardware.
    
- `gpio_pin_set_dt()` changes the output.
    
- `gpio_pin_get_dt()` reads the input.

### Without using the Device tree:

```C
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define LED_PORT "GPIOA"
#define LED_PIN  5

int main(void)
{
    const struct device *gpio;

    gpio = device_get_binding(LED_PORT);

    if (gpio == NULL) {
        return 0;
    }

    gpio_pin_configure(gpio, LED_PIN, GPIO_OUTPUT_INACTIVE);

    while (1) {
        gpio_pin_toggle(gpio, LED_PIN);
        k_msleep(500);
    }

    return 0;
}
```

- `device_get_binding()` retrieves the GPIO controller.
    
- `gpio_pin_configure()` configures pin 5 as output.
    
- `gpio_pin_toggle()` toggles the output.
    
- `k_msleep(500)` waits 500 milliseconds.

## Using Zephyr GPIO API

```C
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>

#define GPIO_CONTROLLER DT_NODELABEL(gpioa)
#define LED_PIN 5

int main(void)
{
    const struct device *gpio = DEVICE_DT_GET(GPIO_CONTROLLER);

    if (!device_is_ready(gpio)) {
        return 0;
    }

    gpio_pin_configure(gpio, LED_PIN, GPIO_OUTPUT_INACTIVE);

    while (1) {
        gpio_pin_toggle(gpio, LED_PIN);
        k_msleep(500);
    }
}
```

## Using HAL STM32

```C
#include <zephyr/kernel.h>
#include <stm32g0xx_hal.h>

int main(void)
{
    while (1) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
        k_msleep(500);
    }

    return 0;
}
```

## Using Baremetal Register Level

```C
#include <zephyr/kernel.h>
#include <stm32g070xx.h>

int main(void)
{
    /* Enable GPIOA clock */
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    /* Configure PA5 as output */
    GPIOA->MODER &= ~(3U << (5 * 2));
    GPIOA->MODER |=  (1U << (5 * 2));

    while (1) {
        GPIOA->ODR ^= (1U << 5);

        k_msleep(500);
    }

    return 0;
}
```

# ADC

## prj.conf

```ini
CONFIG_ADC=y
CONFIG_PRINTK=y
CONFIG_SERIAL=y
```

## OVERLAY

For GPIO, you previously wrote:

```
gpios = <&gpiog 10 GPIO_ACTIVE_HIGH>;
```

For ADC, we'll use a different property:

```
io-channels = <&adc1 10>;
```

