#include <Arduino.h>
#include "detection_task.h"
#include "server_task.h"
#include "config.h"

TaskHandle_t detectionTask;

void setup() {
    // Init serial
    Serial.begin(115200);
    Serial.setDebugOutput(true);
    Serial.println();

    if (!WiFi.config(local_IP, gateway, subnet)) {
        Serial.println("STA Failed to configure");
    }
    WiFi.begin(ssid, password);
    WiFi.setSleep(false);
    int i = 0;
    while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
        delay(1000);
        Serial.print(++i); Serial.print(' ');
    }
    Serial.println('\n');
    Serial.println("Connection established!");  
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());         // Send the IP address of the ESP8266 to the computer

#if DEBUG >= 2
    Serial.setDebugOutput(true);
#endif

    xTaskCreatePinnedToCore(
                    apriltag::task_apriltag_pose_estimate,   /* Task function. */
                    "ApprilTag",     /* name of task. */
                    100000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &detectionTask,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 1 */
    delay(500);

    setup_tcp_server();
    // setup_websocketserver();
    setup_http_handlers();
}

void loop() {
    loop_tcp_server();
    // loop_websocketserver();
    delay(10);
}
