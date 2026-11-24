#include <zephyr/kernel.h>
#include <zephyr/sys/printk.h>

int main(void)
{

	while (1) {
		printk("Hello World!!!, from Zephyr RTOS\n");
		k_msleep(1000);
	}
}

// for pico devices west build -p always -b rpi_pico . -S cdc-acm-console

// Either each time add -S cdc-acm-console or add this in overlay


// / {
// 	chosen {
// 		zephyr,console = &cdc_acm_uart0;
// 	};
// };

// &zephyr_udc0 {
// 	cdc_acm_uart0: cdc_acm_uart0 {
// 		compatible = "zephyr,cdc-acm-uart";
// 	};
// };
