#pragma once
#include <esp_http_server.h>


typedef esp_err_t (*httpd_resp_handler_t)(httpd_req_t *r);


const char* httpd_media_type(const char *path);
esp_err_t httpd_resp_send_json(httpd_req_t *req, const char *json);
esp_err_t httpd_resp_send_file(httpd_req_t *req, const char *path);
esp_err_t httpd_on(httpd_handle_t handle, const char *uri, httpd_method_t method,
  httpd_resp_handler_t handler
);
esp_err_t httpd_on_static(httpd_req_t *req);
esp_err_t httpd_init(httpd_handle_t *handle);
