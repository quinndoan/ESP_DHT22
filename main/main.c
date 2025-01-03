#include "library.h"
#include "DHT22.h"
#include "STA.h"
#include "webserver.h"
#include "UART.h"
#include "ADC.h"
int DHTgpio = 4;				// my default DHT pin = 4
float humidity = 0.;
float temperature = 0.;
const char *TAG;
void DataRead_task(void *pvParameter)
{
	setDHTgpio( 4 );
	printf( "Starting DHT & ADC Task\n\n");

	while(1) {
	
		printf("=== Reading DHT & ADC ===\n" );
		int ret = readDHT();
		
		errorHandler(ret);

            // Gửi dữ liệu qua UART
        send_environment_data();

		printf( "Hum %.1f\n", getHumidity() );
		printf( "Tmp %.1f\n", getTemperature() );
        printf("Voltage %lu mV\n", (unsigned long)read_voltage_mV());

		vTaskDelay( 2000 / portTICK_PERIOD_MS );
	}
}
// Hàm app_main chính
void app_main() {
    // Khởi tạo NVS để lưu dữ liệu không biến đổi (cần thiết cho WiFi)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Kết nối vào WiFi ở chế độ Station
    wifi_init_sta();
     initialize_adc();

    // Khởi động Web Server
    start_webserver();
    initialize_uart();
    // Khởi chạy DHT task để đọc dữ liệu cảm biến định kỳ
    xTaskCreate(&DataRead_task, "DataRead_task", 2048, NULL, 5, NULL);

}