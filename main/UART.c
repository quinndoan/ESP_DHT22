#include"UART.h"
#include"DHT22.h"
#include "ADC.h"
#define UART_BAUD_RATE     115200
#define BUF_SIZE           1024
const char* TAG_UART = "UART_RECEIVED";
// Initialize UART
void initialize_uart() {
   const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    // We won't use a buffer for sending data.
    uart_driver_install(UART, BUF_SIZE * 2, 0, 0, NULL, 0);
    uart_param_config(UART, &uart_config);
    uart_set_pin(UART, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

int sendData(const char* logName, const char* data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(UART, data, len);
    ESP_LOGI(logName, "Wrote %d bytes", txBytes);
    return txBytes;
}

 void tx_task(void *arg)
{
    // Nhận thông điệp từ tham số truyền vào (arg)
    const char* message = (const char*) arg;

        while (1){// Gửi thông điệp qua UART
        uart_write_bytes(UART, message, strlen(message));
        vTaskDelay(5000/portTICK_PERIOD_MS);
        
        }
       
}
// void send_temperature_and_humidity() {
//     char message[64];
//     snprintf(message, sizeof(message), "Temperature: %.1f°C, Humidity: %.1f%%\r\n", getTemperature(), getHumidity());
//     sendData(TAG_UART, message);
// }

void send_environment_data() {
    // Đọc nhiệt độ, độ ẩm và điện áp
    float temperature = getTemperature();
    float humidity = getHumidity();
    uint32_t voltage = read_voltage_mV();

    // Tạo chuỗi chứa cả ba thông số
    char message[96];
    snprintf(message, sizeof(message), "Temperature: %.1f°C, Humidity: %.1f%%, Voltage: %lu mV\r\n", 
             temperature, humidity, (unsigned long)voltage);

    // Gửi chuỗi qua UART
    sendData(TAG_UART, message);
}
