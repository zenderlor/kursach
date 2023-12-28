#include "cam.h"

void camInit(Cam *cam) {
    memset(cam, 0, sizeof(Cam));
    cam->eye[2] = 10.0f;
    cam->look_dir[2] = -1.0f;
    cam->up[1] = 1.0f;
    cam->yaw = -90.0f;
    cam->firstMouse = 1;
}

void camDoMovement(Cam *cam, float delta_time, float speed) {
    float camSpeed = speed * delta_time;
    if (cam->forwardActivated) {
        vec3 delta;
        memcpy(delta, cam->look_dir, sizeof(vec3));
        //delta[1] = 0.f;
        m_vec3_normalize_to(delta, delta);
        m_vec3_scale(delta, camSpeed, delta);
        m_vec3_add(cam->eye, delta, cam->eye);
    }
    if (cam->backwardActivated) {
        vec3 delta;
        memcpy(delta, cam->look_dir, sizeof(vec3));
        //delta[1] = 0.f;
        m_vec3_normalize_to(delta, delta);
        m_vec3_scale(delta, camSpeed, delta);
        m_vec3_sub(cam->eye, delta, cam->eye);
    }
    if (cam->leftActivated) {
        vec3 right;
        m_vec3_crossn(cam->look_dir, cam->up, right);
        m_vec3_normalize(right);
        m_vec3_scale(right, camSpeed, right);
        m_vec3_sub(cam->eye, right, cam->eye);
    }
    if (cam->rightActivated) {
        vec3 right;
        m_vec3_crossn(cam->look_dir, cam->up, right);
        m_vec3_normalize(right);
        m_vec3_scale(right, camSpeed, right);
        m_vec3_add(cam->eye, right, cam->eye);
    }
}
void camMotion(Cam *cam, int mx, int my, int the_w, int the_h, float sensitivity) {
    //printf("%d %d\n", mx, my);
    static int warped = 0;
    if (warped) {
        //printf("miss\n");
        if (mx != cam->lastX || my != cam->lastY) {
            return;
        }
        warped = 0;
    }
    if (the_w == 0) {
        return;
    }
    if (cam->firstMouse) {
        cam->firstMouse = 0;
        cam->lastX = mx;
        cam->lastY = my;
    }
    float offset_x = (float)(mx - cam->lastX);
    float offset_y = (float)(cam->lastY - my);
    cam->lastX = mx;
    cam->lastY = my;

    offset_x *= sensitivity;
    offset_y *= sensitivity;

    cam->yaw += offset_x;
    cam->pitch += offset_y;

    if (cam->pitch > 89.0f) {
        cam->pitch = 89.0f;
    }
    if (cam->pitch < -89.0f) {
        cam->pitch = -89.0f;
    }

    cam->look_dir[0] = cosf(TO_RAD(cam->yaw)) * cosf(TO_RAD(cam->pitch));
    cam->look_dir[1] = sinf(TO_RAD(cam->pitch));
    cam->look_dir[2] = sinf(TO_RAD(cam->yaw)) * cosf(TO_RAD(cam->pitch));
    m_vec3_normalize(cam->look_dir);

    int margin = 100;
    if (mx < margin || mx > the_w - margin ||
        my < margin || my > the_h - margin) {
        cam->lastX = the_w / 2;
        cam->lastY = the_h / 2;
        warped = 1;
        glutWarpPointer(cam->lastX, cam->lastY);
    }
}

void camReshape(Cam *cam, int w, int h) {
    cam->lastX = w / 2;
    cam->lastY = h / 2;
    glutWarpPointer(cam->lastX, cam->lastY);
}

void camView(Cam *cam, mat4 view) {
    vec3 center;
    m_vec3_add(cam->eye, cam->look_dir, center);
    mLookAt(cam->eye, center, cam->up, view);
}