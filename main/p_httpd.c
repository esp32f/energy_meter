#include <esp_http_server.h>
#include "p_httpd.h"
#include "macros.h"


#define FILE_PATH_MAX     1024
#define FILE_BUFFER_SIZE  1024


const char* httpd_media_type(const char *path) {
  const char *ext = strrchr(path, '.');
  if (ext == NULL) return "text/plain";
  if (strcmp(ext, ".html") == 0) return "text/html";
  if (strcmp(ext, ".css") == 0) return "text/css";
  if (strcmp(ext, ".js") == 0) return "text/javascript";
  if (strcmp(ext, ".png") == 0) return "image/png";
  if (strcmp(ext, ".jpg") == 0) return "image/jpeg";
  return "text/plain";
}


esp_err_t httpd_resp_send_json(httpd_req_t *req, const char *json) {
  ERET( httpd_resp_set_type(req, "application/json") );
  ERET( httpd_resp_sendstr(req, json) );
  return ESP_OK;
}


esp_err_t httpd_resp_send_file(httpd_req_t *req, const char *path) {
  const char *type = httpd_media_type(path);
  ERET( httpd_resp_set_type(req, type) );
  FILE *f = fopen(path, "r");
  if (f == NULL) {
    printf("* Cannot open file %s!\n", path);
    return ESP_FAIL;
  }
  char buff[FILE_BUFFER_SIZE];
  size_t read;
  do {
    read = fread(buff, 1, sizeof(buff), f);
    ERET( httpd_resp_send_chunk(req, buff, read) );
  } while(read == sizeof(buff));
  ERET( httpd_resp_send_chunk(req, NULL, 0) );
  fclose(f);
  return ESP_OK;
}


esp_err_t httpd_on(httpd_handle_t handle, const char *uri, httpd_method_t method,
  httpd_resp_handler_t handler) {
  httpd_uri_t reg = {
    .uri = uri,
    .method = method,
    .handler = handler,
    .user_ctx = NULL,
  };
  return httpd_register_uri_handler(handle, &reg);
};


esp_err_t httpd_on_static(httpd_req_t *req) {
  printf("@ HTTPD Static: uri=%s\n", req->uri);
  const char *index = strcmp(req->uri, "/") == 0? "index.html" : "";
  char path[FILE_PATH_MAX];
  sprintf(path, "/spiffs%s%s", req->uri, index);
  return httpd_resp_send_file(req, path);
}


esp_err_t httpd_init(httpd_handle_t *handle) {
  printf("# Init HTTP server\n");
  httpd_config_t c = HTTPD_DEFAULT_CONFIG();
  c.uri_match_fn = httpd_uri_match_wildcard;
  return httpd_start(handle, &c);
}
