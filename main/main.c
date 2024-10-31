#include "library.h"
#include "DHT22.h"
#include "STA.h"
#include "webserver.h"
int DHTgpio = 4;				// my default DHT pin = 4
float humidity = 0.;
float temperature = 0.;
const char *TAG;
void DHT_task(void *pvParameter)
{
	setDHTgpio( 4 );
	printf( "Starting DHT Task\n\n");

	while(1) {
	
		printf("=== Reading DHT ===\n" );
		int ret = readDHT();
		
		errorHandler(ret);

		printf( "Hum %.1f\n", getHumidity() );
		printf( "Tmp %.1f\n", getTemperature() );
		
		// -- wait at least 5 sec before reading again ------------
		// The interval of whole process must be beyond 5 seconds !! 
		vTaskDelay( 2000 / portTICK_PERIOD_MS );
	}
}

// void app_main()
// {
// 	nvs_flash_init();
// 	vTaskDelay( 1000 / portTICK_PERIOD_MS );
// 	xTaskCreate( &DHT_task, "DHT_task", 2048, NULL, 5, NULL );
// }


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

    // Khởi động Web Server
    start_webserver();

    // Khởi chạy DHT task để đọc dữ liệu cảm biến định kỳ
    xTaskCreate(&DHT_task, "DHT_task", 2048, NULL, 5, NULL);
}