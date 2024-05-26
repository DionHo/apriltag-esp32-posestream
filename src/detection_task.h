#ifndef DETECTION_TASK_H
#define DETECTION_TASK_H

struct AprilTagDetection {
    int id;
    float rot[9];
    float t[3];
};

char* sprint_last_detections(char* buffer_ptr);

int get_last_detections(AprilTagDetection* last_detection_out);

void task_apriltag_pose_estimate(void * pvParameters);

#endif // DETECTION_TASK_H