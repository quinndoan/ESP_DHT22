#ifndef WEBSERVER_H_
#define WEBSERVER_H_
#include "library.h"
#include "esp_err.h"
#include "esp_http_server.h"  

esp_err_t temperature_get_handler(httpd_req_t *req);
esp_err_t humidity_get_handler(httpd_req_t *req);
esp_err_t voltage_get_handler(httpd_req_t *req);
void start_webserver(void);
#endif