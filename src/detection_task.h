#ifndef DETECTION_TASK_H
#define DETECTION_TASK_H

namespace Apriltag {

    struct Detection {
        int id;
        float rot[9];
        float t[3];
    };

    typedef void apriltag_callback(void);

    void add_subscriber(apriltag_callback);

    char* sprint_last_detections(char* buffer_ptr);

    int get_last_detections(Detection* last_detection_out);

    void task_apriltag_pose_estimate(void * pvParameters);

}

#endif // DETECTION_TASK_H