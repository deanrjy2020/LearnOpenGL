#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/shader_c.h>
#include <learnopengl/camera.h>

#include <utils.h>

#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

/*
todo, 在cube里面, camera的移动怎么反过来了?
*/

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void character_callback(GLFWwindow* window, unsigned int codepoint);
void processInput(GLFWwindow *window);
void renderQuad();
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// quad texture size
const unsigned int QUAD_WIDTH = 1024, QUAD_HEIGHT = 1024;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f; // time of last frame

// Which compute program to use
int shaderIdx = 0;
unsigned int shaderNum = 1;

float random(vec2 seed) {
    return fract(sin(dot(seed, vec2(12.9898, 78.233))) * 43758.5453123);
}

vec2 randSeed = vec2(0.5 / QUAD_WIDTH, 0.5 / QUAD_HEIGHT); // 第一个pixel的seed. 不用测试很多坐标, 一个就够了.
float randomFloat() {
    randSeed += vec2(1.0, 1.0);  // 推进种子
    float rnd = random(randSeed);
    randSeed = vec2(rnd);
    return rnd;
}

int main(int argc, char* argv[])
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetCharCallback(window, character_callback);
    glfwSwapInterval(0);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // query limitations
    // -----------------
    int max_compute_work_group_count[3];
    int max_compute_work_group_size[3];
    int max_compute_work_group_invocations;

    for (int idx = 0; idx < 3; idx++) {
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, idx, &max_compute_work_group_count[idx]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, idx, &max_compute_work_group_size[idx]);
    }
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &max_compute_work_group_invocations);

    std::cout << "OpenGL Limitations: " << std::endl;
    std::cout << "maximum number of work groups in X dimension " << max_compute_work_group_count[0] << std::endl;
    std::cout << "maximum number of work groups in Y dimension " << max_compute_work_group_count[1] << std::endl;
    std::cout << "maximum number of work groups in Z dimension " << max_compute_work_group_count[2] << std::endl;

    std::cout << "maximum size of a work group in X dimension " << max_compute_work_group_size[0] << std::endl;
    std::cout << "maximum size of a work group in Y dimension " << max_compute_work_group_size[1] << std::endl;
    std::cout << "maximum size of a work group in Z dimension " << max_compute_work_group_size[2] << std::endl;

    std::cout << "Number of invocations in a single local work group that may be dispatched to a compute shader " << max_compute_work_group_invocations << std::endl;

    // build and compile shaders
    // -------------------------
    // compute shader 完全可以移植到FS里面, 在shadertoy上跑.
    vector<ComputeShader> computeShaders = {
        // key 0, 必须放最前面, CS里面会检查是否是第一帧, 同时是基于image内容的, 换到其他再换回来就有问题了.
        ComputeShader("random.cs"),
        // key 1, copy of LearnOpenGL/src/8.guest/2022/5.computeshader_helloworld
        ComputeShader("hello_world.cs"),
        // key 2, https://yangwc.com/2019/05/08/RayTracer-Basis/
        ComputeShader("simple_ray_tracing.cs"),
    };
    shaderNum = computeShaders.size();

    Shader screenQuad("screenQuad.vs", "screenQuad.fs");
    screenQuad.use();
    screenQuad.setInt("tex", 0);

    // load textures for compute shader to read.
    // -----------------------------------------
    unsigned int brickTexture  = loadTexture(FileSystem::getPath("resources/textures/bricks2.jpg").c_str());

    // Create texture for opengl operation
    // -----------------------------------
    unsigned int quadTexture;

    glGenTextures(1, &quadTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, quadTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, QUAD_WIDTH, QUAD_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

    // In ray tracing or compute shaders:
    // GL_RGBA32F (每个channel是32bit float)
    //   You store floating point data (e.g., positions, normals, colors with HDR range)
    //   You do math in shaders — 8-bit loses precision quickly
    //   RGBA8 clamps values between 0–255 (or 0.0–1.0 normalized),
    //     while RGBA32F lets you store any float values like vec3(4.1, -1.2, 0.7) directly
    glBindImageTexture(0, quadTexture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    unordered_map<float, int> rndMap; // rnd, i
    for (int i = 0; i < 1000000; i++) {
        float rnd = randomFloat();
        cout << "i = " << i << ", rnd = " << rnd << endl;
        if (rndMap.count(rnd)) {
            cout << "Warning: the rnd is in the map already, collision at i = " << rndMap.at(rnd) << ", current map size = " << rndMap.size() << endl;
            // 一旦碰到重复的, 后面全部是重复的.
            break;
        }
        rndMap[rnd] = i;
    }

    // render loop
    // -----------
    int fCounter = 0;
    int frameIdx = 0; // start from 1
    while (!glfwWindowShouldClose(window)) {
        // Set frame time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        fCounter++;
        frameIdx++;
        if(fCounter > 50) {
            std::cout << "FPS: " << 1 / deltaTime
            << ", camPos = (" << camera.Position.x << ", " << camera.Position.y << ", " << camera.Position.z << ")"
            << ", camFront = (" << camera.Front.x << ", " << camera.Front.y << ", " << camera.Front.z << ")"
            << ", camUp = (" << camera.Up.x << ", " << camera.Up.y << ", " << camera.Up.z << ")"
            << std::endl;
            fCounter = 0;
        }

        // input
        // -----
        processInput(window);

        // compute
        computeShaders[shaderIdx].use();
        computeShaders[shaderIdx].setInt("brickTexture", 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, brickTexture);

        computeShaders[shaderIdx].setFloat("t", currentFrame);
        computeShaders[shaderIdx].setInt("frameIdx", frameIdx);
        computeShaders[shaderIdx].setVec3("position", camera.Position);
        computeShaders[shaderIdx].setVec3("front", camera.Front);
        computeShaders[shaderIdx].setVec3("up", camera.Up);
        computeShaders[shaderIdx].setVec3("right", camera.Right);

        glDispatchCompute((unsigned int)QUAD_WIDTH/16, (unsigned int)QUAD_HEIGHT/16, 1);

        // make sure writing to image has finished before read
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        // render image to quad
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        screenQuad.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, quadTexture);

        renderQuad();

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteTextures(1, &brickTexture);
    glDeleteTextures(1, &quadTexture);
    glDeleteProgram(screenQuad.ID);
    for (unsigned int i = 0; i < shaderNum; ++i) {
        glDeleteProgram(computeShaders[i].ID);
    }

    glfwTerminate();

    return EXIT_SUCCESS;
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    // Select which shader program to use.
    if ('0' <= codepoint && codepoint < '0' + shaderNum) {
        shaderIdx = codepoint - '0';
    } else if ('p' == codepoint) {
        dumpFramebuffer("fb.png", SCR_WIDTH, SCR_HEIGHT);
    }
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}