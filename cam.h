#ifndef CAM_H
#define CAM_H

#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

#include <string.h>
#include "m.h"

typedef struct {
    vec3 eye;
    vec3 look_dir;
    vec3 up;
    int forwardActivated;
    int backwardActivated;
    int leftActivated;
    int rightActivated;
    float yaw;
    float pitch;
    int lastX;
    int lastY;
    int firstMouse;
} Cam;

void camInit(Cam *cam);

void camDoMovement(Cam *cam, float delta_time, float speed);

void camMotion(Cam *cam, int mx, int my, int the_w, int the_h, float sensitivity);

void camReshape(Cam *cam, int w, int h);

void camView(Cam *cam, mat4 view);

#endif