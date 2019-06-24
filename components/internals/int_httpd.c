#include <stdio.h>
#include <sdkconfig.h>
#include <esp_log.h>
#include <esp_http_server.h>
#include "macros.h"


static const char *TAG = "httpd";


esp_err_t httpd_init() {
  httpd_handle_t handle = NULL;
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.uri_match_fn = httpd_uri_match_wildcard;
  ERET( httpd_start(&handle, &config) );
}


esp_err_t httpd_on(
  httpd_handle_t handle, httpd_method_t method, const char *path,
  esp_err_t (*handler)(httpd_req_t *r)) {
  httpd_uri_t uri = {
    .method = method,
    .uri = path,
    .handler = handler,
    .user_ctx = NULL
  };
  return httpd_register_uri_handler(handle, handler);
}
