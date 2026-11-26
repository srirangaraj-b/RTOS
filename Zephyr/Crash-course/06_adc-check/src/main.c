#include <zephyr/kernel.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>

#define ADC_NODE DT_ALIAS(myadc)

static const struct adc_dt_spec temp_sen = ADC_DT_SPEC_GET_BY_NAME(ADC_NODE, temp);
static const struct adc_dt_spec pot = ADC_DT_SPEC_GET_BY_NAME(ADC_NODE, pot);

uint16_t raw_temp[2];
uint16_t raw_pot[2];

static struct adc_sequence seq_temp = {
    .buffer = raw_temp,
    .buffer_size = sizeof(raw_temp),
};

static struct adc_sequence seq_pot = {
    .buffer = raw_pot,
    .buffer_size = sizeof(raw_pot),
};

int main(){

    uint32_t raw_temp_d;
    uint32_t raw_pot_d;

    float volt_temp_f;
    float volt_pot_f;

    float temperature;
    int degree;

    adc_sequence_init_dt(&temp_sen, &seq_temp);
    adc_sequence_init_dt(&pot, &seq_pot);

    while(1){

        adc_read_dt(&temp_sen, &seq_temp);
        adc_read_dt(&pot, &seq_pot);

        raw_temp_d = raw_temp[0];
        raw_pot_d = raw_pot[0];

        volt_temp_f = ((float)raw_temp_d*3.3)/4095;
        volt_pot_f = ((float)raw_pot_d*3.3)/4095;

        temperature = ((float)raw_temp_d*330)/4095;
        degree = ((float)raw_pot_d*360)/4095;

        printk("Raw value of a1: %d Voltage of a1: %.2f Temperature: %.2f\n", (int)raw_temp_d, volt_temp_f, temperature);
        printk("Raw value of a2: %d Voltage of a2: %.2f Degree: %d \n", (int)raw_pot_d, volt_pot_f, degree);

       
        k_msleep(500);
    }

}