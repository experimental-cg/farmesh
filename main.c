#include "main.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define PLANE_SIDE 800

unsigned int VAO, VBO, EBO;
unsigned int shaderProgramCube;

static void cleanup(void) {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgramCube);
  glfwTerminate();
}

int main(void) {
  glfwInit();
  atexit(cleanup);
  GLFWwindow *window = setup_window();
  handle_control_callbacks(window);

  signed char plane_generator[] = {
    0, 0,   0,  1,   1,  1,
    0, 0,   1,  1,   1,  0,
    0, 0,   1,  0,   1, -1,
    0, 0,   1, -1,   0, -1,
    0, 0,   0, -1,  -1, -1,
    0, 0,  -1, -1,  -1,  0,
    0, 0,  -1,  0,  -1,  1,
    0, 0,  -1,  1,   0,  1
  };
  //unsigned int pgen_size = sizeof(plane_generator);

  unsigned long vert_len = PLANE_SIDE*PLANE_SIDE*sizeof(plane_generator)/sizeof(signed char);

  float *plane_vertices = malloc(vert_len * sizeof(float));

  float *pid = plane_vertices;
  for (int y = 0; y < PLANE_SIDE; y++) {
    for (int x = 0; x < PLANE_SIDE; x++) {
      for (unsigned long i = 0; i < sizeof(plane_generator); i += 2) {
        *pid = (plane_generator[i] + 2 * x - PLANE_SIDE)*1000.0f;
        *(pid + 1) = (plane_generator[i + 1] + 2 * y - PLANE_SIDE)*1000.0f;
        pid += 2;
      }
    }
  }

  /* for (unsigned long k = 0; k < vert_len; k++) { */
  /*   printf("%f ", plane_vertices[k]); */
  /*   if (k % pgen_size == 0) { */
  /*     printf("\n"); */
  /*   } */
  /* } */

  /* float plane_vertices_stat[] = { */
  /*   0.0f, 0.0f,   0.0f,  1.0f,   1.0f,  1.0f, */
  /*   0.0f, 0.0f,   1.0f,  1.0f,   1.0f,  0.0f, */
  /*   0.0f, 0.0f,   1.0f,  0.0f,   1.0f, -1.0f, */
  /*   0.0f, 0.0f,   1.0f, -1.0f,   0.0f, -1.0f, */
  /*   0.0f, 0.0f,   0.0f, -1.0f,  -1.0f, -1.0f, */
  /*   0.0f, 0.0f,  -1.0f, -1.0f,  -1.0f,  0.0f, */
  /*   0.0f, 0.0f,  -1.0f,  0.0f,  -1.0f,  1.0f, */
  /*   0.0f, 0.0f,  -1.0f,  1.0f,   0.0f,  1.0f */
  /* }; */

  /* plane_vertices = plane_vertices_stat; */

  /* for (int y = 0; y < PLANE_SIDE; y++) { */
  /*   for (int x = 0; x < PLANE_SIDE; x++) { */
  /*     int i = 2 * (y * PLANE_SIDE + x); */
  /*     plane_vertices[i] = x - PLANE_SIDE/2.0f; */
  /*     plane_vertices[i + 1] = y - PLANE_SIDE/2.0f; */
  /*   } */
  /* } */

  const char *vertexShaderSourceCube = "#version 330 core\n"
    //"#define PI 3.1415926538\n"
    "layout (location = 0) in vec2 aPos;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "uniform vec2 landPos;\n"
    //"uniform float time;\n"
    "out vec3 pos;\n"
    "out vec4 pos4;\n"
    "float rand(vec2 co){\n"
    "  return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);\n"
    "}\n"
    "float sn(vec2 s) {\n"
    "   float a0 = rand(floor(s) + vec2(0,0))*(1 - (s.x-floor(s.x))) + rand(floor(s) + vec2(1,0))*(s.x-floor(s.x));\n"
    "   float a1 = rand(floor(s) + vec2(0,1))*(1 - (s.x-floor(s.x))) + rand(floor(s) + vec2(1,1))*(s.x-floor(s.x));\n"
    "   return a0*(1 - (s.y-floor(s.y))) + a1*(s.y - floor(s.y));\n"
    "}\n"
    "float ws(int depth, float a, float b, float x) {\n"
    "  float y = 0.0;"
    "  for (int k = 1; k < depth; k++) {\n"
    "    y += sin(3.14 * pow(k, a) * pow(k, b) * x) / pow(k, a);\n"
    "  }\n"
    "  return y;\n"
    "}\n"
    "void main()\n"
    "{\n"
    "   vec2 pd = landPos - floor(landPos/200)*200;\n"
    "   vec2 p2 = (aPos + floor(landPos/200)*200);\n"
    //"   float y = 0.0 + 2000*sin(time/30);\n"
    "   float y = 0.0;\n"
    "   y += 50*(sn(p2/500 + vec2(1.2,3.4)) - 0.3);\n"
    "   y += 80*(sn(p2/1000 + vec2(12,34)) - 0.3);\n"
    "   y += 100*(sn(p2/2000 + vec2(2,4)) - 0.3);\n"
    "   y += 1000*(sn(p2/20000 + vec2(123,344)) - 0.3);\n"
    "   y += 5000*(sn(p2/50000 + vec2(13,34)) - 0.6);\n"
    "   y += 10000*(sn(p2/100000 + vec2(4.3,3.4)) - 0.6);\n"
    "   y *= 10;\n"

    "   float dx = 200*(sn(p2/3000 + vec2(0.21,0.32) + vec2(y/100)) - 0.3);\n"
    "   float dz = 200*(sn(p2/3000 + vec2(1.21,0.75) + vec2(y/100)) - 0.3);\n"
    "   dx += 50*(rand(p2 + vec2(0.21,0.32)) - 0.3);\n"
    "   dz += 50*(rand(p2 + vec2(1.21,0.75)) - 0.3);\n"

    "   gl_Position = projection * view * model * vec4(aPos.x - pd.x + dx, y, aPos.y - pd.y + dz, 1.0);\n"
    //"   gl_Position = projection * view * model * vec4(aPos.x - pd.x, y, aPos.y - pd.y, 1.0);\n"
    "   pos = vec3(p2.x, y, p2.y);\n"
    "   pos4 = gl_Position;\n"
    "}\0";
  const char *fragmentShaderSourceCube = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec3 pos;\n"
    "in vec4 pos4;\n"
    "uniform bool wireframe_mode;\n"
    "uniform float time;\n"
    "float rand(vec2 co){\n"
    "  return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);\n"
    "}\n"
    "float sn(vec2 s) {\n"
    "   float a0 = rand(floor(s) + vec2(0,0))*(1 - (s.x-floor(s.x))) + rand(floor(s) + vec2(1,0))*(s.x-floor(s.x));\n"
    "   float a1 = rand(floor(s) + vec2(0,1))*(1 - (s.x-floor(s.x))) + rand(floor(s) + vec2(1,1))*(s.x-floor(s.x));\n"
    "   return a0*(1 - (s.y-floor(s.y))) + a1*(s.y - floor(s.y));\n"
    "}\n"
    "void main()\n"
    "{\n"
    "    float y = pos.y + 100*sin(time + pos.x/1000);"
    "    if (wireframe_mode) {\n"
    "        FragColor = vec4(1.0);\n"
    "        return;\n"
    "    }\n"
    "    vec3 col;\n"
    "    if (y < 0) {\n"
    "        col = vec3(0.2,0.2,0.8);\n"
    "    } else if (pos.y > 4000 + 2000*sn(pos.xz/2000 + vec2(1,-2))) {\n"
    "        col = vec3(0.52,0.64,0.14);\n"
    "    } else if (pos.y > 3000 + 2000*sn(pos.xz/2000 + vec2(1,2))) {\n"
    "        col = vec3(0.52,0.24,0.14);\n"
    "    } else {"
    "        col = vec3(0.8,0.8,0.5);\n"
    "    }"
    "\n"
    "    if (y < 0)\n"
    "        col /= max((sqrt(abs(y))/10),1.0);\n"
    "    else {\n"
    "        col *= max((sqrt(abs(pos.y))/20)/3,1.0);\n"
    //"        col *= (2.0+sin(time/80 + pos.z/3000)*sin(time/80 + pos.x/3000))/2;"
    "    }\n"
    "\n"
    "    float s  = 100;\n"
    "    float gx = pos.x/s+0.04;\n"
    "    float gz = pos.z/s+0.04;\n"
    "    if (pos4.z < 20000 && ((gx - floor(gx) < 0.08) || (gz - floor(gz) < 0.08))) {\n"
    "        col *= 0.7;\n"
    "    }"
    "    s  = 200;\n"
    "    gx = (pos.x+pos.z)/s+0.02;\n"
    "    gz = (pos.x-pos.z)/s+0.02;\n"
    "    if (pos4.z < 20000 && ((gx - floor(gx) < 0.04) || (gz - floor(gz) < 0.04))) {\n"
    "        col *= 0.7;\n"
    "    }"
    "    FragColor = vec4(col, 1.0);\n"
    "    FragColor *= vec4(vec3(rand(mod(floor(pos.xz/5), 1024)))*0.1+0.8, 1.0);\n"
    "}\0";

  // generate vertex attribute object
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  // cube
  glBindVertexArray(VAO);

  // generate vertex buffer object
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vert_len * sizeof(float), plane_vertices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // texture attribute
  //glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
  //glEnableVertexAttribArray(1);

  //unsigned int textureContainer = load_texture("container.jpg", false);
  //unsigned int textureAwesome = load_texture("awesomeface.png", true);
  
  shaderProgramCube = setup_shaders(vertexShaderSourceCube, fragmentShaderSourceCube);

  unsigned int modelLoc = glGetUniformLocation(shaderProgramCube, "model");
  unsigned int viewLoc = glGetUniformLocation(shaderProgramCube, "view");
  unsigned int projectionLoc = glGetUniformLocation(shaderProgramCube, "projection");
  unsigned int visibilityLoc = glGetUniformLocation(shaderProgramCube, "visibility");
  unsigned int wireframeModeLoc = glGetUniformLocation(shaderProgramCube, "wireframe_mode");
  unsigned int timeLoc = glGetUniformLocation(shaderProgramCube, "time");
  unsigned int landPosLoc = glGetUniformLocation(shaderProgramCube, "landPos");

  glEnable(GL_DEPTH_TEST);
  // wireframe mode
  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // color mode (default)
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  mat4 view;
  vec3 lookAt;
  //vec2 landPos;
  float lastTimeValue = glfwGetTime();
  float dt;

  while(!glfwWindowShouldClose(window)) {
    handle_controls(window);

    glClearColor(0.6f, 0.7f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    float timeValue = glfwGetTime();
    dt = timeValue - lastTimeValue;
    lastTimeValue = timeValue;
    printf("FPS: %f\n", 1/dt);

    //landPos[0] = cameraPos[0];
    //landPos[1] = cameraPos[2];
    //printf("%f %f\n", landPos[0], landPos[1]);
    //cameraPos[0] = 0.0f;
    //cameraPos[2] = 0.0f;
    //lookAt[1] = 0.0f;
    glm_vec3_add((vec3) {0.0, cameraPos[1], 0.0}, cameraFront, lookAt);
    glm_lookat((vec3) {0.0, cameraPos[1], 0.0}, lookAt, cameraUp, view);
    /* glm_vec3_add(cameraPos, cameraFront, lookAt); */
    /* glm_lookat(cameraPos, lookAt, cameraUp, view); */

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, textureContainer);
    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, textureAwesome);
    glUseProgram(shaderProgramCube);
    //glUniform1i(glGetUniformLocation(shaderProgramCube, "textureContainer"), 0);
    //glUniform1i(glGetUniformLocation(shaderProgramCube, "textureAwesome"), 1);

    //glm_translate_z(view, -3.0f);

    mat4 projection = GLM_MAT4_IDENTITY_INIT;
    glm_perspective(glm_rad(fov), 800.0f / 600.0f, 0.1f, 1000000.0f, projection);

    glUniform1f(visibilityLoc, visibility);
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *)view);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (float *)projection);
    glBindVertexArray(VAO);

    mat4 model = GLM_MAT4_IDENTITY_INIT;

    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (float *)model);
    glUniform2f(landPosLoc, cameraPos[0], cameraPos[2]);

    glUniform1i(wireframeModeLoc, false);
    glUniform1f(timeLoc, timeValue);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, vert_len/2);

    /* glm_translate_y(view, 1.0f); */
    /* glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (float *)view); */
    /* glUniform1i(wireframeModeLoc, true); */
    /* glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); */
    /* glDrawArrays(GL_TRIANGLES, 0, vert_len/2); */

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  return 0;
}
