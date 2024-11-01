#ifndef ADC_H_  
#define ADC_H_

#include "library.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_log.h"

void initialize_adc();
uint32_t read_voltage_mV();

#endif