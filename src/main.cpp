#include <Arduino.h>
#include "detection_task.h"


TaskHandle_t detectionTask;

void setup() {
  // Init serial
  Serial.begin(115200);
#if DEBUG >= 2
  Serial.setDebugOutput(true);
#endif

  xTaskCreatePinnedToCore(
                    task_apriltag_pose_estimate,   /* Task function. */
                    "ApprilTag",     /* name of task. */
                    100000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &detectionTask,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 1 */
    delay(500);
}

void loop() {
  // Nothing here, the real loop is already at the end of setup()
}
