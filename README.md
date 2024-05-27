# ESP32-Cam AprilTag streaming project

This platform.io project is basically an example of how the [apriltag-esp32](https://github.com/raspiduino/apriltag-esp32) library can be used to provide a stream of pose estimates over network. Big thanks to the AprilTag team for developing and @raspiduino for porting this fiducial marker system!

This project runs the apriltag detector on the esp32 in a loop and provides access to the pose estimates via
  * HTTP-Get (on port 80)
  * TCP-socket (on port 8080)
  * and Websocket (currently disabled, otherwise on port 80)

It also includes a client example as a mini-godot3 project (see `examples/`).

## How to build

This project relies on PlatformIO. Please install it to build this project.

Also, you need to add a `src/config.h` file with the following contents and adapt the variables to your needs:

``` C++
#ifndef CONFIG_H
#define CONFIG_H

#include <WiFi.h>

// =========================== //
/////// WiFi credentials ////////
// =========================== //
const char *ssid = "";
const char *password = "";
// Static IP address
IPAddress local_IP(192, 168, 1, 11);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);



#endif // CONFIG_H
```
