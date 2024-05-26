#include "esp_http_server.h"

#include "server_task.h"
#include "detection_task.h"

httpd_handle_t apriltag_httpd = NULL;

static esp_err_t apriltag_handler(httpd_req_t *req)
{
    esp_err_t res = ESP_OK;

    char buffer[500];
    char* buffer_ptr = buffer;
    buffer_ptr = sprint_last_detections(buffer_ptr);

    httpd_resp_set_type(req, HTTPD_TYPE_JSON);
    res = httpd_resp_send(req, buffer, buffer_ptr-buffer);
    return res;
}

void setup_http_handlers() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 16;

    httpd_uri_t apriltag_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = apriltag_handler,
        .user_ctx = NULL
#ifdef CONFIG_HTTPD_WS_SUPPORT
        ,
        .is_websocket = true,
        .handle_ws_control_frames = false,
        .supported_subprotocol = NULL
#endif
    };
    if (httpd_start(&apriltag_httpd, &config) == ESP_OK)
    {
        httpd_register_uri_handler(apriltag_httpd, &apriltag_uri);
    }
}