#ifndef CONTROLS_H_
#define CONTROLS_H_

#include "glad.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "cglm/cglm.h"

extern float visibility;
extern float world_up[3], cameraPos[3];
extern float cameraFront[3], cameraUp[3], cameraTmp[3];

extern float deltaTime, lastFrame;

extern float lastPitch, lastYaw, yaw, pitch, fov;

extern float lastX, lastY;
extern bool firstMouse;

void handle_control_callbacks(GLFWwindow* window);
void handle_controls(GLFWwindow* window);

#endif // CONTROLS_H_
