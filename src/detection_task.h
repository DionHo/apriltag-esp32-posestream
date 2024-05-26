#ifndef DETECTION_TASK_H
#define DETECTION_TASK_H

struct AprilTagDetection {
    int id;
    float rot[9];
    float t[3];
};

void task_apriltag_pose_estimate(void * pvParameters);

#endif // DETECTION_TASK_H