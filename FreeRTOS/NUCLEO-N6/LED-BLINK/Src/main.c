
#include "stm32n657xx.h"

/* LED pin definitions */
#define LED_RED       (1U << 10)
#define LED_GREEN     (1U << 0)
#define LED_BLUE      (1U << 8)

#define LED_ALL       (LED_RED | LED_GREEN | LED_BLUE)

/* Simple software delay */
static void delay(volatile uint32_t count)
{
    while (count--)
    {
        __NOP();
    }
}

/* Configure GPIOG */
static void GPIO_Init(void)
{
    /* 1. Enable GPIOG peripheral clock */
    RCC->AHB4ENSR = RCC_AHB4ENSR_GPIOGENS;

    /* Read back clock register */
    (void)RCC->AHB4ENR;

    /* 2. Configure PG0, PG8 and PG10 as outputs */

    GPIOG->MODER &= ~(
        (3U << (0U * 2U))  |
        (3U << (8U * 2U))  |
        (3U << (10U * 2U))
    );

    GPIOG->MODER |= (
        (1U << (0U * 2U))  |
        (1U << (8U * 2U))  |
        (1U << (10U * 2U))
    );

    /* 3. Push-pull output */
    GPIOG->OTYPER &= ~LED_ALL;

    /* 4. No pull-up or pull-down */
    GPIOG->PUPDR &= ~(
        (3U << (0U * 2U))  |
        (3U << (8U * 2U))  |
        (3U << (10U * 2U))
    );

    /* 5. Turn OFF all LEDs (active LOW) */
    GPIOG->BSRR = LED_ALL;
}

int main(void)
{
    GPIO_Init();

    while (1)
    {
        /* RED ON */
        GPIOG->BSRR = (LED_RED << 16);
        delay(2000000);

        /* RED OFF */
        GPIOG->BSRR = LED_RED;

        /* GREEN ON */
        GPIOG->BSRR = (LED_GREEN << 16);
        delay(2000000);

        /* GREEN OFF */
        GPIOG->BSRR = LED_GREEN;

        /* BLUE ON */
        GPIOG->BSRR = (LED_BLUE << 16);
        delay(2000000);

        /* BLUE OFF */
        GPIOG->BSRR = LED_BLUE;
    }
}
