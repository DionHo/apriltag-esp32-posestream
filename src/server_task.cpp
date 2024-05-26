#include "esp_http_server.h"

#include "server_task.h"
#include "detection_task.h"

httpd_handle_t apriltag_httpd = NULL;

static esp_err_t apriltag_handler(httpd_req_t *req)
{
    esp_err_t res = ESP_OK;
    AprilTagDetection detections[10];
    int detections_count;
    detections_count = get_last_detections(detections);

    char buffer[500];
    char* buffer_ptr = buffer;
    int n;
    n = sprintf(buffer_ptr,"[");
    buffer_ptr += n;
    for(int i=0; i<detections_count; ++i) {

        n = sprintf(buffer_ptr,"{\"id\": %d,"
                "\"rot\": [%9.6f,%9.6f,%9.6f,%9.6f,%9.6f,%9.6f,%9.6f,%9.6f,%9.6f], "
                "\"t\": [%9.6f,%9.6f,%9.6f]}",
                detections[i].id,
                detections[i].rot[0], detections[i].rot[1], detections[i].rot[2], 
                detections[i].rot[3], detections[i].rot[4], detections[i].rot[5], 
                detections[i].rot[6], detections[i].rot[7], detections[i].rot[8],
                detections[i].t[0],   detections[i].t[1],   detections[i].t[2]
            );
        buffer_ptr += n;
    }
    n = sprintf(buffer_ptr,"]");
    buffer_ptr += n;

    res = httpd_resp_send(req, buffer, buffer_ptr-buffer);
    return res;
}

void setup_http_handlers() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.max_uri_handlers = 16;

    httpd_uri_t stream_uri = {
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
        httpd_register_uri_handler(apriltag_httpd, &stream_uri);
    }
}