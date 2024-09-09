#include "controls.h"

float visibility = 0.2f;
vec3 world_up    = { 0.0f, 1.0f,  0.0f };
vec3 cameraPos   = { 0.0f, 20.0f,  0.0f };
vec3 cameraFront = { 0.0f, 0.0f, -1.0f };
vec3 cameraUp    = { 0.0f, 1.0f,  0.0f };
vec3 cameraTmp;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float lastPitch, lastYaw;
float yaw = -90.0f;
float pitch;
float fov = 45.0f;

float lastX = 400, lastY = 300;
bool firstMouse = true;
bool fly = true;
float vy = 0.0f; // for jumping
int speed_multiplier = 1;

static void calculate_time(void) {
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
}

bool fly_repeat = false;

static void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    visibility = fmin(visibility + 0.01f, 1.0f);
  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    visibility = fmax(visibility - 0.01f, 0.0f);
  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && !fly_repeat) {
    fly_repeat = true;
    if (fly) {
      printf("Fly off\n");
      fly = false;
    } else {
      printf("Fly on\n");
      fly = true;
      vy = 0.0f; // cancel momentum
    }
  }
  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE && fly_repeat)
    fly_repeat = false;

  float cameraSpeed = 50.3f * deltaTime; // adjust accordingly
  //float cameraSpeed = 2.3f * deltaTime; // adjust accordingly
  float cameraPerpSpeed = 30.8f * deltaTime; // adjust accordingly
  float cameraRotSpeed = 30.0f * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    speed_multiplier = 1;
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    speed_multiplier = 2;
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    speed_multiplier = 4;
  if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    speed_multiplier = 8;
  if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    speed_multiplier = 16;
  if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    speed_multiplier = 32;
  if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    speed_multiplier = 64;
  if (glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
    speed_multiplier = 128;
  if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
    speed_multiplier = 254;
  if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    speed_multiplier = 512;

  cameraSpeed *= speed_multiplier;
  cameraPerpSpeed *= speed_multiplier;
  cameraRotSpeed *= fmin(fmax(1.0, speed_multiplier/32.0), 1.8);

  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
    cameraSpeed *= 5.0f; // gotta go fast
    cameraPerpSpeed *= 5.0f;
    cameraRotSpeed *= 5.0f;
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_RELEASE) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      glm_vec3_add((vec3) {cameraSpeed*cos(glm_rad(yaw)), 0, cameraSpeed*sin(glm_rad(yaw))}, cameraPos, cameraPos);
      //glm_vec3_muladds(cameraFront, +cameraSpeed, cameraPos);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      glm_vec3_add((vec3) {-cameraSpeed*cos(glm_rad(yaw)), 0, -cameraSpeed*sin(glm_rad(yaw))}, cameraPos, cameraPos);
      //glm_vec3_muladds(cameraFront, -cameraSpeed, cameraPos);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
      glm_vec3_crossn(cameraFront, cameraUp, cameraTmp);
      glm_vec3_muladds(cameraTmp, -cameraPerpSpeed, cameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
      glm_vec3_crossn(cameraFront, cameraUp, cameraTmp);
      glm_vec3_muladds(cameraTmp, +cameraPerpSpeed, cameraPos);
    }
  }
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
    if (fly)
      glm_vec3_muladds(cameraUp, +cameraSpeed, cameraPos);
    else if (cameraPos[1] <= 20.1f)
      vy = 140.0f;
      //vy = 190.0f; // too jumpy
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    glm_vec3_muladds(cameraUp, -cameraSpeed, cameraPos);

  lastYaw   = yaw;
  lastPitch = pitch;

  if (glfwGetKey(window, GLFW_KEY_LEFT_ALT) == GLFW_PRESS) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
      pitch += cameraRotSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
      pitch -= cameraRotSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      yaw -= cameraRotSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      yaw += cameraRotSpeed;
  }

  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    pitch += cameraRotSpeed;
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    pitch -= cameraRotSpeed;
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    yaw -= cameraRotSpeed;
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    yaw += cameraRotSpeed;

  if (pitch > 89.0f)
    pitch =  89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;

  const float fov_speed = 0.5f;
  if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS)
    fov -= fov_speed;
  if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS)
    fov += fov_speed;

  if (fov < 1.0f)
    fov = 1.0f;
  if (fov > 45.0f)
    fov = 45.0f;

  if (!fly) {
    vy -= 200.0f*deltaTime;
    cameraPos[1] += vy*deltaTime;
    if (cameraPos[1] <= 10.0f) {
      cameraPos[1] = 10.0f;
    }
  }
}

static void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;

  const float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  lastYaw   = yaw;
  lastPitch = pitch;

  yaw   += xoffset;
  pitch += yoffset;

  if (pitch > 89.0f)
    pitch =  89.0f;
  if (pitch < -89.0f)
    pitch = -89.0f;
}

static void update_camera(void) {
  cameraFront[0] = cos(glm_rad(pitch)) * cos(glm_rad(yaw));
  cameraFront[1] = sin(glm_rad(pitch));
  cameraFront[2] = cos(glm_rad(pitch)) * sin(glm_rad(yaw));
}

static void focus_callback(GLFWwindow* window, int focused) {
  if (!focused) {
    // rollback last mouse move
    yaw   = lastYaw;
    pitch = lastPitch;
  }
}

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  fov -= (float)yoffset;
  if (fov < 1.0f)
    fov = 1.0f;
  if (fov > 45.0f)
    fov = 45.0f;
}

void handle_control_callbacks(GLFWwindow* window) {
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetWindowFocusCallback(window, focus_callback);
  glfwSetScrollCallback(window, scroll_callback);
}

void handle_controls(GLFWwindow* window) {
  process_input(window);
  calculate_time();
  update_camera();
}
