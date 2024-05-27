#include <WiFi.h>
#include "esp_http_server.h"
#include <WebSocketsServer.h>

#include "server_task.h"
#include "detection_task.h"

httpd_handle_t      apriltag_httpd = NULL;
WebSocketsServer    websocket_server(11399);
WiFiServer          tcp_server(8080);

// Array to hold client connections
WiFiClient clients[5]; // You can handle up to 5 clients


static esp_err_t apriltag_handler(httpd_req_t *req)
{
    esp_err_t res = ESP_OK;

    char buffer[500];
    char* buffer_ptr = buffer;
    buffer_ptr = apriltag::sprint_last_detections(buffer_ptr);

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

void broadcast_tcp_on_new_data(){
    char buffer[500];
    char* buffer_ptr = apriltag::sprint_last_detections(buffer);
    // String buffer_str = String(buffer,buffer_ptr-buffer);
    Serial.printf("Broadcast data to tcp clients");
    // Broadcast the message to all connected clients
    for (int j = 0; j < 5; j++) {
        if (clients[j] && clients[j].connected()) {
            clients[j].write(buffer,buffer_ptr-buffer);
        }
    }
}

void setup_tcp_server() {
    tcp_server.begin();
    apriltag::add_subscriber(broadcast_tcp_on_new_data);
}

void loop_tcp_server() {
  // Check for new client connections
  WiFiClient newClient = tcp_server.available();
  if (newClient) {
    bool added = false;
    for (int i = 0; i < 5; i++) {
      if (!clients[i] || !clients[i].connected()) {
        clients[i] = newClient;
        Serial.print("New client connected, assigned to slot ");
        Serial.println(i);
        added = true;
        break;
      }
    }
    if (!added) {
      Serial.println("No available slots for new clients");
      newClient.stop();
    }
  }

  // Handle data from connected clients
  for (int i = 0; i < 5; i++) {
    if (clients[i] && clients[i].connected()) {
      if (clients[i].available()) {
        String message = clients[i].readStringUntil('\n');
        Serial.print("Message from client ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(message);

      }
    }
  }
}

// Callback: receiving any WebSocket message
void onWebSocketEvent(uint8_t client_num, WStype_t type, uint8_t *payload, size_t length)
{
    // Figure out the type of WebSocket event
    switch (type)
    {

        // Client has disconnected
        case WStype_DISCONNECTED:
        {
            Serial.printf("[%u] Disconnected!\n", client_num);
            break;
        }
        // New client has connected
        case WStype_CONNECTED:
        {
            IPAddress ip = websocket_server.remoteIP(client_num);
            Serial.printf("[%u] Connection from ", client_num);
            Serial.println(ip.toString());
            break;
        }
        // Handle text messages from client
        case WStype_TEXT:
        {
            // Print out raw message
            Serial.printf("[%u] Received text: %s\n", client_num, payload);
            break;
        }
        // For everything else: do nothing
        case WStype_BIN:
        case WStype_ERROR:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
        default:
            break;
    }
}

void broadcast_on_new_data(){
    char buffer[500];
    char* buffer_ptr = apriltag::sprint_last_detections(buffer);
    websocket_server.broadcastTXT(buffer, buffer_ptr - buffer);
    Serial.printf("Broadcast data to clients");
}

void setup_websocketserver() {
    websocket_server.begin();
    websocket_server.onEvent(onWebSocketEvent);
    apriltag::add_subscriber(broadcast_on_new_data);
}

void loop_websocketserver() {
    websocket_server.loop();
}
