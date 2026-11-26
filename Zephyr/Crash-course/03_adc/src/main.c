#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>

#define ADC_NODE DT_ALIAS(myadc)

static const struct adc_dt_spec adc_a1 = ADC_DT_SPEC_GET_BY_NAME(ADC_NODE, a1);
static const struct adc_dt_spec adc_a2 = ADC_DT_SPEC_GET_BY_NAME(ADC_NODE, a2);

uint16_t raw_a1[2];
uint16_t raw_a2[2];

static struct adc_sequence seq_a1 = {
    .buffer = raw_a1,
    .buffer_size = sizeof(raw_a1),
};

static struct adc_sequence seq_a2 = {
    .buffer = raw_a2,
    .buffer_size = sizeof(raw_a2),
};

int main(){

    uint32_t raw1;
    uint32_t raw2;

    adc_sequence_init_dt(&adc_a1, &seq_a1);
    adc_sequence_init_dt(&adc_a2, &seq_a2);

    while(1){

        adc_read_dt(&adc_a1, &seq_a1);
        adc_read_dt(&adc_a2, &seq_a2);

        raw1 = raw_a1[0];
        raw2 = raw_a2[0];

        printk("Raw value of a1: %d \n", (int)raw1);
        printk("Raw value of a2: %d \n", (int)raw2);

        k_msleep(500);
    }

}