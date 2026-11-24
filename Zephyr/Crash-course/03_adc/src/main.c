#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>

#define ADC_NODE DT_ALIAS(my_adc)

static const struct adc_dt_spec adc_a1 = ADC_DT_SPEC_GET_BY_NAME(ADC_NODE, a1);
static const struct adc_dt_spec adc_a2 = ADC_DT_SPEC_GET_BY_NAME(ADC_NODE, a2);

/* 2 samples of room per buffer to avoid -ENOMEM */
static uint16_t raw_a1[2];
static uint16_t raw_a2[2];

static struct adc_sequence seq_a1 = {
    .buffer = raw_a1,
    .buffer_size = sizeof(raw_a1),
};

static struct adc_sequence seq_a2 = {
    .buffer = raw_a2,
    .buffer_size = sizeof(raw_a2),
};

int main(void)
{
    int ret;
    int32_t voltage;

    adc_sequence_init_dt(&adc_a1, &seq_a1);
    adc_sequence_init_dt(&adc_a2, &seq_a2);

    while (1) {
        ret = adc_read_dt(&adc_a1, &seq_a1);
        voltage = raw_a1[0];
        adc_raw_to_millivolts_dt(&adc_a1, &voltage);
        printk("A1: Raw = %u, Voltage = %d mV\n", (unsigned int)raw_a1[0], (int)voltage);

        printk("--------------------------\n");

        ret = adc_read_dt(&adc_a2, &seq_a2);
        voltage = raw_a2[0];
        adc_raw_to_millivolts_dt(&adc_a2, &voltage);
        printk("A2: Raw = %u, Voltage = %d mV\n", (unsigned int)raw_a2[0], (int)voltage);

        printk("--------------------------\n");

        k_msleep(1000);
    }

    return 0;
}