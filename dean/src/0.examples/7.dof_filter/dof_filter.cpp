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
void initOpenCVCamera(cv::VideoCapture& cap);
void updateOpenCVFrame(cv::VideoCapture& cap, cv::Mat& frame);

// settings
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
// set based on your camera hardware output.
const unsigned int TEX_WIDTH = 1280;
const unsigned int TEX_HEIGHT = 720;

// t, h, v, d
char textureSelect = 'd';

// down and up key
bool printPara = true;
bool keyPressed[2] = {false, false};

const unsigned int KERNEL_SIZE = 15;
const float g_fGaussianKernel_sigma_0_4[] = {0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000003, 0.040388, 0.919218, 0.040388, 0.000003, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, };
const float g_fGaussianKernel_sigma_0_6[] = {0.000000, 0.000000, 0.000000, 0.000000, 0.000002, 0.002566, 0.165524, 0.663815, 0.165524, 0.002566, 0.000002, 0.000000, 0.000000, 0.000000, 0.000000, };
const float g_fGaussianKernel_sigma_0_8[] = {0.000000, 0.000000, 0.000000, 0.000002, 0.000441, 0.021910, 0.228310, 0.498675, 0.228310, 0.021910, 0.000441, 0.000002, 0.000000, 0.000000, 0.000000, };
const float g_fGaussianKernel_sigma_1_0[] = {0.000000, 0.000000, 0.000001, 0.000134, 0.004432, 0.053991, 0.241971, 0.398942, 0.241971, 0.053991, 0.004432, 0.000134, 0.000001, 0.000000, 0.000000, };
const float g_fGaussianKernel_sigma_1_2[] = {0.000000, 0.000001, 0.000056, 0.001285, 0.014607, 0.082898, 0.234927, 0.332452, 0.234927, 0.082898, 0.014607, 0.001285, 0.000056, 0.000001, 0.000000, };
const float g_fGaussianKernel_sigma_1_4[] = {0.000001, 0.000029, 0.000484, 0.004810, 0.028686, 0.102713, 0.220797, 0.284959, 0.220797, 0.102713, 0.028686, 0.004810, 0.000484, 0.000029, 0.000001, };
const float g_fGaussianKernel_sigma_1_6[] = {0.000017, 0.000220, 0.001889, 0.010955, 0.042992, 0.114156, 0.205101, 0.249339, 0.205101, 0.114156, 0.042992, 0.010955, 0.001889, 0.000220, 0.000017, };
const float g_fGaussianKernel_sigma_1_8[] = {0.000115, 0.000857, 0.004679, 0.018764, 0.055266, 0.119554, 0.189945, 0.221640, 0.189945, 0.119554, 0.055266, 0.018764, 0.004679, 0.000857, 0.000115, };
const float g_fGaussianKernel_sigma_2_0[] = {0.000436, 0.002216, 0.008765, 0.027000, 0.064769, 0.121004, 0.176059, 0.199501, 0.176059, 0.121004, 0.064769, 0.027000, 0.008765, 0.002216, 0.000436, };
const float g_fGaussianKernel_sigma_2_2[] = {0.001149, 0.004401, 0.013712, 0.034746, 0.071607, 0.120028, 0.163635, 0.181444, 0.163635, 0.120028, 0.071607, 0.034746, 0.013712, 0.004401, 0.001149, };
const float g_fGaussianKernel_sigma_2_4[] = {0.002367, 0.007316, 0.019008, 0.041517, 0.076229, 0.117657, 0.152656, 0.166500, 0.152656, 0.117657, 0.076229, 0.041517, 0.019008, 0.007316, 0.002367, };
const float g_fGaussianKernel_sigma_2_6[] = {0.004107, 0.010743, 0.024238, 0.047162, 0.079149, 0.114567, 0.143029, 0.154010, 0.143029, 0.114567, 0.079149, 0.047162, 0.024238, 0.010743, 0.004107, };
const float g_fGaussianKernel_sigma_2_8[] = {0.006305, 0.014446, 0.029134, 0.051723, 0.080828, 0.111186, 0.134630, 0.143496, 0.134630, 0.111186, 0.080828, 0.051723, 0.029134, 0.014446, 0.006305, };
const float g_fGaussianKernel_sigma_3_0[] = {0.008847, 0.018216, 0.033562, 0.055335, 0.081638, 0.107778, 0.127325, 0.134598, 0.127325, 0.107778, 0.081638, 0.055335, 0.033562, 0.018216, 0.008847, };

float fPixelStepX = (1.0f / TEX_WIDTH);
float fPixelStepY = (1.0f / TEX_HEIGHT);
float		m_fPixelStepH[28] = {
    -7.0f * fPixelStepX,  0.0f,
    -6.0f * fPixelStepX,  0.0f,
    -5.0f * fPixelStepX,  0.0f,
    -4.0f * fPixelStepX,  0.0f,
    -3.0f * fPixelStepX,  0.0f,
    -2.0f * fPixelStepX,  0.0f,
    -1.0f * fPixelStepX,  0.0f,
     1.0f * fPixelStepX,  0.0f,
     2.0f * fPixelStepX,  0.0f,
     3.0f * fPixelStepX,  0.0f,
     4.0f * fPixelStepX,  0.0f,
     5.0f * fPixelStepX,  0.0f,
     6.0f * fPixelStepX,  0.0f,
     7.0f * fPixelStepX,  0.0f,
};

float		m_fPixelStepV[28] = {
0.0f, -5.0f * fPixelStepY,
0.0f, -4.0f * fPixelStepY,
0.0f, -3.0f * fPixelStepY,
0.0f, -2.0f * fPixelStepY,
0.0f, -1.0f * fPixelStepY,
0.0f, 1.0f * fPixelStepY,
0.0f, 2.0f * fPixelStepY,
0.0f, 3.0f * fPixelStepY,
0.0f, 4.0f * fPixelStepY,
0.0f, 5.0f * fPixelStepY,
0.0f, -6.0f * fPixelStepY,
0.0f, -7.0f * fPixelStepY,
0.0f, 6.0f * fPixelStepY,
0.0f, 7.0f * fPixelStepY,
};
glm::vec2 u_center = glm::vec2(0.5f, 0.5f);
float u_fWHRatioSquare = (float)TEX_HEIGHT * (float)TEX_HEIGHT / TEX_WIDTH / TEX_WIDTH;
glm::vec2 u_distSquare = glm::vec2(0.36f, 0.49f);
float u_fMixWeight = 1.0f;

float m_MAT[] = {
1.0,		1.0,		1.0,	0.0,
1.402,-0.714,	0.0,	0.0,
0.0,		-0.344,1.772,	0.0,
-0.701,0.529,-0.886,	1.0

};
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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // OpenCV camera
    cv::VideoCapture cap;
    cv::Mat frame;
    initOpenCVCamera(cap);

    GLuint fboH, fboV;
    glGenFramebuffers(1, &fboH);
    glGenFramebuffers(1, &fboV);

    // Create OpenGL textures
    GLenum frameFormat = GL_BGR;
    GLuint texture, textureH, textureV;
    // src tex
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // 默认是repeat, 处理下面边缘的时候会去拿上面边缘的值, 效果不好, 用clamp to edge重复取自己边缘的值.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_WIDTH, TEX_HEIGHT, 0, frameFormat, GL_UNSIGNED_BYTE, nullptr);
    // texH
    glGenTextures(1, &textureH);
    glBindTexture(GL_TEXTURE_2D, textureH);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_WIDTH, TEX_HEIGHT, 0, frameFormat, GL_UNSIGNED_BYTE, nullptr);
    glBindFramebuffer(GL_FRAMEBUFFER, fboH);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureH, 0);
    // check if framebufferH is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    // texV
    glGenTextures(1, &textureV);
    glBindTexture(GL_TEXTURE_2D, textureV);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEX_WIDTH, TEX_HEIGHT, 0, frameFormat, GL_UNSIGNED_BYTE, nullptr);
    glBindFramebuffer(GL_FRAMEBUFFER, fboV);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureV, 0);
    // check if framebufferV is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    Shader blurShader("blur.vs", "blur.fs");
    blurShader.use();
    blurShader.setInt("s_texture0", 0);

    Shader mixShader("mix.vs", "mix.fs");
    mixShader.use();
    mixShader.setInt("s_texClear", 0);
    mixShader.setInt("s_texBlur", 1);

    initTexQuad(); // for debugging.

    while (!glfwWindowShouldClose(window)) {
	    // input
        // -----
        processInput(window);

        updateOpenCVFrame(cap, frame);

        // Update the OpenGL texture with the new frame
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame.cols, frame.rows, frameFormat, GL_UNSIGNED_BYTE, frame.data);

        glBindFramebuffer(GL_FRAMEBUFFER, fboH); // draw to fboH
        glClear(GL_COLOR_BUFFER_BIT);
        blurShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glUniform1fv(glGetUniformLocation(blurShader.ID, "u_fGaussian"), 15, g_fGaussianKernel_sigma_3_0);
        glUniform2fv(glGetUniformLocation(blurShader.ID, "u_fPixelShift"), 14, m_fPixelStepH);
        renderQuad();
        //glFinish();

        glBindFramebuffer(GL_FRAMEBUFFER, fboV); // draw to fboV
        blurShader.use(); // same shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureH);
        glUniform2fv(glGetUniformLocation(blurShader.ID, "u_fPixelShift"), 14, m_fPixelStepV);
        renderQuad();
        //glFinish();

        glBindFramebuffer(GL_FRAMEBUFFER, 0); // draw to default fbo
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureV);
        mixShader.use();
        // 0 ~ 1
        mixShader.setVec2("u_center", u_center);
        mixShader.setVec2("u_distSquare", u_distSquare);
        mixShader.setFloat("u_fWHRatioSquare", u_fWHRatioSquare);
        mixShader.setFloat("u_fMixWeight", u_fMixWeight);
        //glUniformMatrix4fv(glGetUniformLocation (mixShader.ID, "u_mat_yuv2rgb"), 1, GL_FALSE, (GLfloat*) m_MAT);
        renderQuad();

        // show which texture
        if (textureSelect == 't') {
            drawTexQuad(texture, -1.0, glm::vec2(-1.0, -1.0));
        } else if (textureSelect == 'h') {
            drawTexQuad(textureH, -1.0, glm::vec2(-1.0, -1.0));
        } else if (textureSelect == 'v') {
            drawTexQuad(textureV, -1.0, glm::vec2(-1.0, -1.0));
        }
        // else do thong to show the default output.

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

void initOpenCVCamera(cv::VideoCapture& cap)
{
    // Initialize OpenCV camera capture with the first camera device.
    cap.open(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Camera could not be opened!" << std::endl;
        glfwTerminate();
        return;
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

}

void updateOpenCVFrame(cv::VideoCapture& cap, cv::Mat& frame)
{
    // Capture a new frame
    cap >> frame;
    if (frame.empty()) {
        std::cerr << "Error: Could not grab a frame!" << std::endl;
        return;
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
    // if (key == GLFW_KEY_DOWN) {
    //     sss -= 0.1;
    // } else {
    //     sss +=0.1;
    // }
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
        //std::cout << "sss = " << sss << std::endl;
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
    switch (codepoint) {
        case 't':
        case 'h':
        case 'v':
        case 'd':
            textureSelect = codepoint;
            break;
        default:
            break;
    }

    std::cout << "Typed: " << (char) codepoint << std::endl;
}
