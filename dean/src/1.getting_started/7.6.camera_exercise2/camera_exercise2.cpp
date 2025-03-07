#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <utils.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  800.0f / 2.0;
float lastY =  600.0 / 2.0;
float fov   =  45.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("7.6.camera.vs", "7.6.camera.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // load and create a texture
    // -------------------------
    unsigned int texture1, texture2;
    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    unsigned char *data = stbi_load(FileSystem::getPath("resources/textures/container.jpg").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load(FileSystem::getPath("resources/textures/awesomeface.png").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
    // -------------------------------------------------------------------------------------------
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    initAxes();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        // activate shader
        ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        // Only the mouse scroll will change the fov, and change the P matrix.
        static glm::mat4 prevProjection = glm::mat4(1.0f);
        glm::mat4 glmProjection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        // input parameters.
        float fovY = glm::radians(fov);
        float aspectRatio = (float)SCR_WIDTH / (float)SCR_HEIGHT;
        float front = 0.1f;
        float back = 100.0f;

        float tangent = tan(fovY/2);            // tangent of half fovY
        float top = front * tangent;            // half height of near plane
        float right = top * aspectRatio;        // half width of near plane
        if (prevProjection != glmProjection) {
            printf("\nProjection matrix changed, regenerating:\n");
            printf("fovY=%f, aspectRatio=%f, tangent=%f, top=%f, right=%f\n",
                fovY, aspectRatio, tangent, top, right);
        }

        // params: left, right, bottom, top, near(front), far(back)
        glm::mat4 projection = glm::mat4(1.0f);
        projection[0][0] =  front / right;
        projection[1][1] =  front / top;
        projection[2][2] = -(back + front) / (back - front);
        projection[2][3] = -1;
        projection[3][2] = -(2 * back * front) / (back - front);
        projection[3][3] =  0;

        if (prevProjection != glmProjection) {
            print("glm P matrix", glmProjection);
            print("my P matrix", projection);
            prevProjection = glmProjection;
        }

        ourShader.setMat4("projection", projection);

        // camera/view transformation
        static glm::mat4 prevView = glm::mat4(1.0f);
        glm::mat4 glmView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        // 自己生成view matrix
        // +z in camera space
        // cameraTarget = cameraPos + cameraFront; 这一直是对的, glm::lookAt的第二个参数就是cameraTarget
        // cameraBack = cameraPos - cameraTarget = -cameraFront
        glm::vec3 normCameraBack = glm::normalize(-cameraFront);
        if (prevView != glmView) {
            printf("\nView matrix changed, regenerating:\n");
            print("normCameraBack", normCameraBack);
        }
        // +x in camera space
        glm::vec3 normCameraRight = glm::normalize(glm::cross(cameraUp, normCameraBack));
        if (prevView != glmView) print("normCameraRight", normCameraRight);
        // +y in camera space
        // 这个全局变量cameraUp名字不好, 应该是worldUp, 不是camera的Up
        //glm::vec3 normCameraUp = glm::normalize(cameraUp);
        glm::vec3 normCameraUp = glm::cross(normCameraBack, normCameraRight);
        if (prevView != glmView) print("normCameraUp", normCameraUp);

        glm::mat4 translation = glm::mat4(1.0f);
        // 下面三种方法的到的translation一样.
        translation = glm::translate(translation, -cameraPos);
        // translation[3][0] = -cameraPos.x;
        // translation[3][1] = -cameraPos.y;
        // translation[3][2] = -cameraPos.z;
        // translation[3][0] = -cameraPos[0];
        // translation[3][1] = -cameraPos[1];
        // translation[3][2] = -cameraPos[2];
        if (prevView != glmView) print("translation", translation);

        glm::mat4 rotation = glm::mat4(1.0f);
        rotation[0][0] = normCameraRight.x;
        rotation[1][0] = normCameraRight.y;
        rotation[2][0] = normCameraRight.z;

        rotation[0][1] = normCameraUp.x;
        rotation[1][1] = normCameraUp.y;
        rotation[2][1] = normCameraUp.z;

        rotation[0][2] = normCameraBack.x;
        rotation[1][2] = normCameraBack.y;
        rotation[2][2] = normCameraBack.z;
        if (prevView != glmView) print("rotation", rotation);

        glm::mat4 view = rotation * translation;

        if (prevView != glmView) {
            print("glm V matrix", glmView);
            print("my V matrix", view);
            prevView = glmView;

            glm::mat4 viewportMatrix = glm::mat4(1.0f);
            viewportMatrix[0][0] = SCR_WIDTH / 2;
            viewportMatrix[3][0] = SCR_WIDTH / 2,
            viewportMatrix[1][1] = SCR_HEIGHT / 2;
            viewportMatrix[3][1] = SCR_HEIGHT / 2;
            viewportMatrix[2][2] = (back - front) / 2;
            viewportMatrix[3][2] = (back + front) / 2;
            print("my viewport matrix", viewportMatrix);

            printf("\ntest a specific vertex:\n");
            // 这个点就是运行exe后鼠标不动, 正对的camera的笑脸的右上角点.
            glm::vec4 vertex_ws = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
            glm::vec4 vertex_vs = view * vertex_ws;
            print("view space vertex", vertex_vs);
            // 在clip space里, w分量不是1.
            glm::vec4 vertex_cs = projection * vertex_vs;
            print("clip space vertex", vertex_cs);
            // NDC下, 范围是-1~1, 如x=-1, 说明点在屏幕的最左边, x=1, 在最右边
            // y=-1, 在最下面, y=1, 在最上面, 超过了范围, 说明点在屏幕外面.
            glm::vec4 vertex_ndc = vertex_cs / vertex_cs.w;
            print("NDC vertex", vertex_ndc);
            // screen space
            glm::vec4 vertex_ss = viewportMatrix * vertex_ndc;
            print("screen space vertex", vertex_ss);

            // inverse
            glm::mat4 viewportMatrixInverse = glm::inverse(viewportMatrix);
            print("my viewportMatrixInverse", viewportMatrixInverse);
            // back to ndc
            glm::vec4 vertex_ndc_inv = viewportMatrixInverse * vertex_ss;
            print("NDC vertex inverse", vertex_ndc_inv);

            // back to clip space
            // assume the gl_FragCoord.w = 1/w_clipspace is known in the FS.
            float gl_FragCoord_w = 0.4;
            float w_clipspace = 1 / gl_FragCoord_w;
            glm::vec4 vertex_cs_inv = vertex_ndc_inv * w_clipspace;
            print("clip space vertex inverse", vertex_cs_inv);
            // back to view space
            glm::mat4 projectionMatrixInverse = glm::inverse(projection);
            print("my P matrix inverse", projectionMatrixInverse);
            glm::vec4 vertex_vs_inv = projectionMatrixInverse * vertex_cs_inv;
            print("view space vertex inverse", vertex_vs_inv);
            // back to world sapce
            glm::mat4 viewMatrixInverse = glm::inverse(view);
            print("my V matrix inverse", viewMatrixInverse);
            glm::vec4 vertex_ws_inv = viewMatrixInverse * vertex_vs_inv;
            print("world space vertex inverse", vertex_ws_inv);
        }

        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // model matrix is identity
        glm::mat4 MVP = projection * view;
        drawAxes(MVP);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(2.5 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

    static glm::vec3 prevCameraPos;
    if (prevCameraPos != cameraPos) {
        printf("processInput: output cameraPos = (%f, %f, %f)\n", cameraPos[0], cameraPos[1], cameraPos[2]);
        prevCameraPos = cameraPos;
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

    // 思路:
    // 1, 鼠标只能在二维屏幕左右上下移动, 得到xoffset和yoffset,
    // 2, 将xoffset和yoffset分别累加到yaw和pitch上.
    // 3, 把yaw/pitch计算得到新的cameraFront, 替换原来的.

    // 1, 第一次动的时候两个offset都是0.
    // 假设第二次鼠标向右移动了一个单位, xoffset=1, 即camera向右偏, camera默认向-z看, 向右偏就是向+x偏
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    // 2, yaw增加了一个单位. 初始yaw=-90度=270度, 即向-z看. 增加了一个单位后271度, 偏向+x
    yaw += xoffset;
    // yoffset同理.
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    // 这里注意, camera向上到了89度就卡主了, 理解为人只能转到(接近)正上方, 不能再接着转了.
    // 向下同理.
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);

    printf("mouse_callback: input mouse pos = (%f, %f), output cameraFront = (%f, %f, %f)\n",
        xpos, ypos, cameraFront[0], cameraFront[1], cameraFront[2]);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;

    printf("scroll_callback: input yoffset = %f, output fov = %f\n",
        yoffset, fov);
}
