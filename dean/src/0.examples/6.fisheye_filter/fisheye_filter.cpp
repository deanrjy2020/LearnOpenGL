#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader.h>
#include <utils.h>

#include <opencv2/opencv.hpp>

#include <iostream>
using namespace std;

// this demo is not completed yet, todo later.

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void char_callback(GLFWwindow* window, unsigned int codepoint);
void renderQuad();

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
// set based on your camera hardware output.
const unsigned int CAMERA_FRAME_WIDTH = 1280;
const unsigned int CAMERA_FRAME_HEIGHT = 720;

int fisheyeSelect = 1;
// down and up key
bool printPara = true;
bool keyPressed[2] = {false, false};

// fisheye parameters
// // todo, rename, remove m_
// float m_scale_x = 1.0f;//(float)SCR_WIDTH/(float)CAMERA_FRAME_WIDTH;
// float m_scale_y = 1.0f;//(float)SCR_HEIGHT /(float)CAMERA_FRAME_HEIGHT;
// // how to change? todo
// float m_shift_x = 0.5f;
// float m_shift_y = 0.5f;
// float m_center_x = 0.5f;
// float m_center_y = 0.5f;
// // effect level range [0.6, 1.8]
// float m_distort_x = 0.6f;
// float m_distort_y = 1.0f; // won't change.
// float m_MAT[16] = {   1.0,    1.0,    1.0, 0.0,
//                     1.402, -0.714,    0.0, 0.0,
//                       0.0, -0.344,  1.772, 0.0,
//                    -0.701,  0.529, -0.886, 1.0};
// glm::mat4 rgb2yuvMat = glm::make_mat4(m_MAT);
// //glm::mat4 rgb2yuvMat = glm::transpose(glm::make_mat4(m_MAT));

// fisheye1
float sss = 1.0f;

// fisheye2
//The center about which to apply the distortion, with a default of (0.5, 0.5)
glm::vec2 center = glm::vec2(1.0f, 0.5f);
/// The radius of the distortion, ranging from 0.0 to 1.0, with a default of 0.25
float radius = 0.5f;
/// The amount of distortion to apply, from -1.0 to 1.0, with a default of 0.5
float scale = 1.0f;
float aspectRatio = 1280.0/720;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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
    glfwSetCharCallback(window, char_callback);

    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader fisheyeShader("fisheye.vs", "fisheye.fs");
    Shader fisheyeShader2("fisheye.vs", "fisheye2.fs");

    // Initialize OpenCV camera capture
    cv::VideoCapture cap(0);  // Open default camera (0)
    if (!cap.isOpened()) {
        std::cerr << "Error: Camera could not be opened!" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Set to a very high resolution to attempt to get max resolution
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 100000);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 100000);

    double dWidth  = cap.get(cv::CAP_PROP_FRAME_WIDTH); //获取视频帧宽度
    double dHeight = cap.get(cv::CAP_PROP_FRAME_HEIGHT); //获取视频帧高度
    double fps     = cap.get(cv::CAP_PROP_FPS);
    // CAP_PROP_FORMAT is not always reliable, as some camera backends do not expose this property.
    double format  = cap.get(cv::CAP_PROP_FORMAT);
    cout << "Resolution of the video : " << dWidth << " x " << dHeight \
        << ", Frames per seconds : " << fps \
        << ", format: " << format \
        << endl;
    // int fourcc = static_cast<int>(cap.get(cv::CAP_PROP_FOURCC));
    // char codec[] = {
    //     (char)(fourcc & 0xFF),
    //     (char)((fourcc >> 8) & 0xFF),
    //     (char)((fourcc >> 16) & 0xFF),
    //     (char)((fourcc >> 24) & 0xFF),
    //     '\0' // Null-terminated string
    // };
    // std::cout << "Camera format (FOURCC): " << codec << std::endl;

    cv::Mat frame;

    // Create an OpenGL texture for displaying frames
    GLenum frameFormat = GL_BGR;
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, CAMERA_FRAME_WIDTH, CAMERA_FRAME_HEIGHT, 0, frameFormat, GL_UNSIGNED_BYTE, nullptr);

    while (!glfwWindowShouldClose(window)) {
	    // input
        // -----
        processInput(window);

        // Capture a new frame
        cap >> frame;
        if (frame.empty()) {
            std::cerr << "Error: Could not grab a frame!" << std::endl;
            break;
        }

        // Determine pixel format based on Mat type
        int depth = frame.depth();
        int channels = frame.channels();

        static bool printCameraFrameInfo = false;
        if (!printCameraFrameInfo) {
            std::cout << "Frame depth: " << depth << " (";
            switch (depth) {
                case CV_8U:  std::cout << "8-bit unsigned integer"; break;
                case CV_16U: std::cout << "16-bit unsigned integer"; break;
                case CV_32F: std::cout << "32-bit float"; break;
                default:     std::cout << "Unknown"; break;
            }
            std::cout << ")" << ", Number of channels: " << channels << std::endl;
            printCameraFrameInfo = true;
        }

        // Update the OpenGL texture with the new frame
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, frameFormat, GL_UNSIGNED_BYTE, frame.data);

        // Render the texture to the screen
        glClear(GL_COLOR_BUFFER_BIT);

        if (fisheyeSelect == 1) {
            fisheyeShader.use();
            // fisheyeShader.setVec2("u_distort", glm::vec2(m_distort_x, m_distort_y));
            // fisheyeShader.setVec2("u_shift", glm::vec2(m_shift_x, m_shift_y));
            // fisheyeShader.setVec2("u_scale", glm::vec2(m_scale_x, m_scale_y));
            // fisheyeShader.setVec2("u_center", glm::vec2(m_center_x, m_center_y));
            // fisheyeShader.setMat4("u_mat_yuv2rgb", rgb2yuvMat);

            float pi = 3.14159265f;
            float scale[2] = {1.0f, 1.0f};
            if (CAMERA_FRAME_WIDTH > CAMERA_FRAME_HEIGHT) {
            scale[0] = 1.0f;
            scale[1] = ((float) CAMERA_FRAME_HEIGHT) / CAMERA_FRAME_WIDTH;
            } else {
            scale[0] = ((float) CAMERA_FRAME_WIDTH) / CAMERA_FRAME_HEIGHT;
            scale[1] = 1.0f;
            }
            float alpha = sss * 2.0f + 0.75f;
            float bound2 = 0.25f * (scale[0] * scale[0] + scale[1] * scale[1]);
            float bound = (float) sqrt(bound2);
            float radius = 1.15f * bound;
            float radius2 = radius * radius;
            float max_radian = 0.5f * pi -             (float) atan(alpha / bound * (float) sqrt(radius2 - bound2));
            float factor = bound / max_radian;
            fisheyeShader.setVec2("scale", glm::vec2(scale[0],scale[1]));
            fisheyeShader.setFloat("radius2",radius2);
            fisheyeShader.setFloat("factor", factor);
            fisheyeShader.setFloat("alpha", alpha);
        } else {
            fisheyeShader2.use();
            fisheyeShader2.setVec2("center", center);
            fisheyeShader2.setFloat("aspectRatio",aspectRatio);
            fisheyeShader2.setFloat("radius", radius);
            fisheyeShader2.setFloat("scale", scale);
        }
        // finally render quad
        renderQuad();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();
    cap.release();

    return 0;
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

void updateParameter(int key)
{
    if (key == GLFW_KEY_DOWN) {
        sss -= 0.1;
    } else {
        sss +=0.1;
    }
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    for (int idx = 0; idx <=1; ++idx) {
        int key = idx + GLFW_KEY_DOWN;
        if (glfwGetKey(window, key) == GLFW_PRESS && !keyPressed[idx])
        {
            printPara = true;
            updateParameter(key);
            keyPressed[idx] = true;
        }
        if (glfwGetKey(window, key) == GLFW_RELEASE)
        {
            keyPressed[idx] = false;
        }
    }

    if (printPara) {
        printPara = false;
        std::cout << "sss = " << sss << std::endl;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// use this fun to select the fisheye program.
void char_callback(GLFWwindow* window, unsigned int codepoint)
{
    if (codepoint >= '1' && codepoint <= '2') {
        fisheyeSelect = codepoint - '1' + 1;
    } else if ((codepoint == 'c' || codepoint == 'r' || codepoint == 's' || codepoint == 'a') &&
               fisheyeSelect == 2) {

    }
    std::cout << "Typed: " << (char)codepoint << ", program = " << fisheyeSelect << std::endl;
}
