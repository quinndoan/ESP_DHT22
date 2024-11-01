#include "ADC.h"
const char *TAG_ADC = "Voltage_Measurement";
#define ADC_CHANNEL ADC1_CHANNEL_0       // Sử dụng kênh ADC1_CHANNEL_0 (GPIO 36)
#define ADC_WIDTH ADC_WIDTH_BIT_12       // Độ rộng ADC 12 bit
#define ADC_ATTEN ADC_ATTEN_DB_11        // Độ suy giảm để đo từ 0V đến 3.3V
#define DEFAULT_VREF 1100  
esp_adc_cal_characteristics_t *adc_chars;
void initialize_adc() {
    adc1_config_width(ADC_WIDTH);
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);

    // Khởi tạo và hiệu chỉnh ADC với giá trị Vref
    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN, ADC_WIDTH, DEFAULT_VREF, adc_chars);
}

uint32_t read_voltage_mV() {
    int adc_reading = 0;

    // Đọc giá trị ADC và tính trung bình
    for (int i = 0; i < 10; i++) {
        adc_reading += adc1_get_raw(ADC_CHANNEL);
        vTaskDelay(pdMS_TO_TICKS(10));  // Delay ngắn giữa các lần đo để ổn định
    }
    adc_reading /= 10;  // Lấy trung bình

    // Chuyển đổi giá trị ADC thành mV
    uint32_t voltage_mV = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);

    return voltage_mV;
}
