#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader.h>

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void character_callback(GLFWwindow* window, unsigned int codepoint);
void renderQuad();
unsigned int loadTexture(char const *path);

// A static singleton Parameters class that stores multiple
// uniform variables passed to the shader.
class Parameters
{
public:
    // Resource storage
    // Screen size
    static unsigned int screenWidth;
    static unsigned int screenHeight;
    static bool screenResized;
    // Mouse
    static bool leftMouseButtonPressed;
    static float lastX;
    static float lastY;
    // Timing & frame
    static float currentTime;
    static float lastTime;
    // Note: When paused by the user, the app continues to render the same paused frame,
    // as some shaders rely on mouse input to display different visuals based on interaction.
    // Optimization is possible but not currently necessary.
    static bool timePaused;
    static int frameCount;
    static int lastFrameCount;

    // Which program to use
    static int shaderIdx;
    static unsigned int shaderNum;

    static void reset() {
        screenResized = true;

        leftMouseButtonPressed = false;

        currentTime = 0.0f;
        lastTime = 0.0f;
        timePaused = false;
        frameCount = 0;
        lastFrameCount = 0;
        glfwSetTime(0.0);
    }

    static void init() {
        reset();

        screenWidth = 800;
        screenHeight = 600;
        lastX = (float)screenWidth  / 2.0;
        lastY = (float)screenHeight / 2.0;
        shaderIdx = 0;
        shaderNum = 0;
    }
private:
    // Private constructor, that is we do not want any actual Parameters objects.
    // Its members and functions should be publicly available (static).
    Parameters() { }
};

// Instantiate static variables
unsigned int Parameters::screenWidth;
unsigned int Parameters::screenHeight;
bool Parameters::screenResized;
bool Parameters::leftMouseButtonPressed;
float Parameters::lastX;
float Parameters::lastY;
float Parameters::currentTime;
float Parameters::lastTime;
bool Parameters::timePaused;
int Parameters::frameCount;
int Parameters::lastFrameCount;
int Parameters::shaderIdx;
unsigned int Parameters::shaderNum;

int main()
{
    Parameters::init();

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
    GLFWwindow* window = glfwCreateWindow(Parameters::screenWidth, Parameters::screenHeight, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetCharCallback(window, character_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Adds up to 10 shaders since initializing/compiling these complex shaders takes a long time.
    vector<Shader> shaders = {
        // key 0, https://www.shadertoy.com/view/XtlSD7
        Shader("shared.vs", "mario_world_1_1.fs"),
        // key 1, https://www.shadertoy.com/view/4sccWr
        Shader("shared.vs", "into_you.fs"),
        // key 2, https://www.shadertoy.com/view/Xds3zN
        Shader("shared.vs", "raymarching_primitives.fs"),
        // key 3, https://www.shadertoy.com/view/ld3Gz2
        Shader("shared.vs", "snail.fs"),
        // key 4, https://www.shadertoy.com/view/3lsSzf
        Shader("shared.vs", "happy_jumping.fs"),
        // key 5, https://www.shadertoy.com/view/Ms2SD1
        // Rendering corruption observed locally; root cause is currently unknown.
        Shader("shared.vs", "seascape.fs"),
        // key 6, https://www.shadertoy.com/view/lssBD7
        Shader("shared.vs", "ray_tracing_in_one_weekend.fs"),
    };
    Parameters::shaderNum = shaders.size();

    // Load textures, source: https://github.com/beautypi/shadertoy-iOS-v2
    // -------------------------------------------------------------------
    // The LearnOpenGL code uses the relative path (to the root of the project) to read the textures
    // we do the same here.
    unsigned int snail_channel1  = loadTexture(FileSystem::getPath("dean/resources/Pebbles.png").c_str());
    unsigned int snail_channel2  = loadTexture(FileSystem::getPath("dean/resources/Organic_2.jpg").c_str());
    unsigned int snail_channel3  = loadTexture(FileSystem::getPath("dean/resources/RGBA_Noise_Medium.png").c_str());

    // Shader configuration
    // --------------------
    // snail
    shaders[3].use();
    shaders[3].setInt("iChannel1", 1);
    shaders[3].setInt("iChannel2", 2);
    shaders[3].setInt("iChannel3", 3);

    while (!glfwWindowShouldClose(window)) {
        // Per-frame time logic
        // --------------------
        if (!Parameters::timePaused) {
            Parameters::currentTime = static_cast<float>(glfwGetTime());
            Parameters::frameCount++;
        }
        // Check if a second has passed and update the FPS.
        // issue when the FPS is  < 1, todo, change the 09 and 010, to use the 011 FPS method?
        if (Parameters::currentTime - Parameters::lastTime >= 1.0) {
            std::cout << "shaderIdx: " << Parameters::shaderIdx
                << std::fixed << std::setprecision(2) << ", currentTime: " << Parameters::currentTime
                << ", FPS: " << Parameters::frameCount - Parameters::lastFrameCount
                << ", iResolution: " << Parameters::screenWidth << " x " << Parameters::screenHeight
                << std::endl;
                Parameters::lastFrameCount = Parameters::frameCount;
            Parameters::lastTime = Parameters::currentTime;  // Reset time
        }

        // Input
        // -----
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        shaders[Parameters::shaderIdx].use();
        if (Parameters::screenResized) {
            Parameters::screenResized = false;
            // The third component is the pixel aspect ratio, which is 1.0 if your display has square pixels.
            shaders[Parameters::shaderIdx].setVec3("iResolution", glm::vec3(Parameters::screenWidth, Parameters::screenHeight, 1.0f));
        }
        shaders[Parameters::shaderIdx].setFloat("iTime", Parameters::currentTime);
        shaders[Parameters::shaderIdx].setInt("iFrame", Parameters::frameCount);
        if (Parameters::leftMouseButtonPressed) {
            shaders[Parameters::shaderIdx].setVec4("iMouse", glm::vec4(Parameters::lastX, Parameters::lastY, 1.0f, 0.0f));
        }
        // Textures only used by snail.
        if (Parameters::shaderIdx == 3) {
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, snail_channel1);
            glActiveTexture(GL_TEXTURE2);
            glBindTexture(GL_TEXTURE_2D, snail_channel2);
            glActiveTexture(GL_TEXTURE3);
            glBindTexture(GL_TEXTURE_2D, snail_channel3);
        }
        renderQuad();

        // The CPU and GPU operate asynchronously. Use glFinish() to force synchronization
        // between them, ensuring a more accurate FPS measurement.
        glFinish();

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glfwDestroyWindow(window);
    glfwTerminate();

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
            // Positions
            -1.0f,  1.0f,
            -1.0f, -1.0f,
             1.0f,  1.0f,
             1.0f, -1.0f,
        };
        // Setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

// Process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

    Parameters::screenWidth = width;
    Parameters::screenHeight = height;
    Parameters::screenResized = true;
}

// glfw: capture the mouse click events
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            Parameters::leftMouseButtonPressed = true;
        } else {
            assert(action == GLFW_RELEASE);
            Parameters::leftMouseButtonPressed = false;
        }
    }
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void cursor_position_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    Parameters::lastX = static_cast<float>(xposIn);
    Parameters::lastY = static_cast<float>(yposIn);
    //printf("cursor_position_callback, %f, %f\n", lastX, lastY);
}

void character_callback(GLFWwindow* window, unsigned int codepoint)
{
    // Select which shader program to use.
    if ('0' <= codepoint && codepoint < '0' + Parameters::shaderNum) {
        Parameters::reset();
        Parameters::shaderIdx = codepoint - '0';
        return;
    }

    switch (codepoint) {
        case 'r':
            // Reset some globals including the system time.
            Parameters::reset();
            break;
        case 'p':
            // Pause/resume the system time
            Parameters::timePaused = !Parameters::timePaused;
            // From pause status to resume status, need to rollback the system time
            if (!Parameters::timePaused) {
                glfwSetTime(Parameters::currentTime);
            }
            break;
        default:
            break;
    }
}

// Utility function for loading a 2D texture from file
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
