#include "webserver.h"
#include "DHT22.h"
#include <stdio.h>
#include "ADC.h"
// // Định nghĩa HTML code cho trang chủ
// static const char index_html[] = R"rawliteral(
// <!DOCTYPE HTML><html>
// <head>
//   <meta name="viewport" content="width=device-width, initial-scale=1">
//   <style>
//     html { font-family: Arial; display: inline-block; text-align: center; }
//     h2 { font-size: 2.5rem; }
//     p { font-size: 2.0rem; }
//     .units { font-size: 1.2rem; }
//     .dht-labels { font-size: 1.5rem; vertical-align: middle; padding-bottom: 15px; }
//     .temperature { color: #FF9900; }
//     .humidity { color: #00ADD6; }
//   </style>
// </head>
// <body>
//   <h2>ESP32 Weather Report</h2>
//   <p>
//     <span class="dht-labels">Temperature</span> 
//     <span id="temperature" class="temperature">--</span>
//     <sup class="units">&deg;C</sup>
//   </p>
//   <p>
//     <span class="dht-labels">Humidity</span>
//     <span id="humidity" class="humidity">--</span>
//     <sup class="units">%</sup>
//   </p>
//   <script>
//     setInterval(function () {
//       // Fetch temperature
//       fetch("/temperature").then(response => response.text()).then(data => {
//         document.getElementById("temperature").innerHTML = data;
//       });
//       // Fetch humidity
//       fetch("/humidity").then(response => response.text()).then(data => {
//         document.getElementById("humidity").innerHTML = data;
//       });
//     }, 5000);  // Update every 5 seconds
//   </script>
// </body>
// </html>
// )rawliteral";
static const char index_html[] = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html { font-family: Arial; display: inline-block; text-align: center; }
    h2 { font-size: 2.5rem; }
    p { font-size: 2.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels { font-size: 1.5rem; vertical-align: middle; padding-bottom: 15px; }
    .temperature { color: #FF9900; }
    .humidity { color: #00ADD6; }
    .voltage { color: #3333FF; }
  </style>
</head>
<body>
  <h2>ESP32 Weather and Voltage Report</h2>
  <p>
    <span class="dht-labels">Temperature</span> 
    <span id="temperature" class="temperature">--</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <span class="dht-labels">Humidity</span>
    <span id="humidity" class="humidity">--</span>
    <sup class="units">%</sup>
  </p>
  <p>
    <span class="dht-labels">Voltage</span>
    <span id="voltage" class="voltage">--</span>
    <sup class="units">mV</sup>
  </p>
  <script>
    setInterval(function () {
      // Fetch temperature
      fetch("/temperature").then(response => response.text()).then(data => {
        document.getElementById("temperature").innerHTML = data;
      });
      // Fetch humidity
      fetch("/humidity").then(response => response.text()).then(data => {
        document.getElementById("humidity").innerHTML = data;
      });
      // Fetch voltage
      fetch("/voltage").then(response => response.text()).then(data => {
        document.getElementById("voltage").innerHTML = data;
      });
    }, 5000);  // Update every 5 seconds
  </script>
</body>
</html>
)rawliteral";


// Định nghĩa hàm handler cho trang chủ
esp_err_t root_get_handler(httpd_req_t *req) {
    httpd_resp_send(req, index_html, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Định nghĩa hàm handler cho endpoint /temperature
esp_err_t temperature_get_handler(httpd_req_t *req) {
    char temp_str[32];
    snprintf(temp_str, sizeof(temp_str), "%.1f", (double)getTemperature());
    httpd_resp_send(req, temp_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// Định nghĩa hàm handler cho endpoint /humidity
esp_err_t humidity_get_handler(httpd_req_t *req) {
    char hum_str[32];
    snprintf(hum_str, sizeof(hum_str), "%.1f", (double)getHumidity());
    httpd_resp_send(req, hum_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

esp_err_t voltage_get_handler(httpd_req_t *req) {
    uint32_t voltage = read_voltage_mV();
    char voltage_str[32];
    snprintf(voltage_str, sizeof(voltage_str), "%lu mV", (unsigned long)voltage);
    httpd_resp_send(req, voltage_str, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

// // Khởi tạo web server và đăng ký các URI handler
// void start_webserver(void) {
//     httpd_config_t config = HTTPD_DEFAULT_CONFIG();
//     httpd_handle_t server = NULL;

//     if (httpd_start(&server, &config) == ESP_OK) {
//         // Đăng ký URI cho trang gốc
//         httpd_uri_t root_uri = {
//             .uri = "/",
//             .method = HTTP_GET,
//             .handler = root_get_handler
//         };
//         httpd_register_uri_handler(server, &root_uri);

//         // Đăng ký URI cho nhiệt độ
//         httpd_uri_t temperature_uri = {
//             .uri = "/temperature",
//             .method = HTTP_GET,
//             .handler = temperature_get_handler
//         };
//         httpd_register_uri_handler(server, &temperature_uri);

//         // Đăng ký URI cho độ ẩm
//         httpd_uri_t humidity_uri = {
//             .uri = "/humidity",
//             .method = HTTP_GET,
//             .handler = humidity_get_handler
//         };
//         httpd_register_uri_handler(server, &humidity_uri);
//     } else {
//         ESP_LOGE("WEBSERVER", "Error starting server!");
//     }
// }


void start_webserver(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    httpd_handle_t server = NULL;

    if (httpd_start(&server, &config) == ESP_OK) {
        // Đăng ký URI cho trang gốc
        httpd_uri_t root_uri = {
            .uri = "/",
            .method = HTTP_GET,
            .handler = root_get_handler
        };
        httpd_register_uri_handler(server, &root_uri);

        // Đăng ký URI cho nhiệt độ
        httpd_uri_t temperature_uri = {
            .uri = "/temperature",
            .method = HTTP_GET,
            .handler = temperature_get_handler
        };
        httpd_register_uri_handler(server, &temperature_uri);

        // Đăng ký URI cho độ ẩm
        httpd_uri_t humidity_uri = {
            .uri = "/humidity",
            .method = HTTP_GET,
            .handler = humidity_get_handler
        };
        httpd_register_uri_handler(server, &humidity_uri);

        // Đăng ký URI cho điện áp
        httpd_uri_t voltage_uri = {
            .uri = "/voltage",
            .method = HTTP_GET,
            .handler = voltage_get_handler
        };
        httpd_register_uri_handler(server, &voltage_uri);

    } else {
        ESP_LOGE("WEBSERVER", "Error starting server!");
    }
}
