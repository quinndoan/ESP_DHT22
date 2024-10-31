#include "webserver.h"
#include "DHT22.h"
esp_err_t temperature_get_handler(httpd_req_t *req)
{
    char temp_str[32];
    snprintf(temp_str, sizeof(temp_str), "%.1f", (double)getTemperature());

    httpd_resp_send(req, temp_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t humidity_get_handler(httpd_req_t *req)
{
    char hum_str[32];
    snprintf(hum_str, sizeof(hum_str), "%.1f", (double)getHumidity());
    httpd_resp_send(req, hum_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        httpd_uri_t temperature_uri = {
            .uri = "/temperature",
            .method = HTTP_GET,
            .handler = temperature_get_handler
        };
        httpd_register_uri_handler(server, &temperature_uri);

        httpd_uri_t humidity_uri = {
            .uri = "/humidity",
            .method = HTTP_GET,
            .handler = humidity_get_handler
        };
        httpd_register_uri_handler(server, &humidity_uri);
    }
}
