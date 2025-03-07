#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings, this could be changed when re-sizing the window.
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;

// drawback is that we can't add any comment in the vs when using '\'
const char *vertexShaderSource = "#version 330 core\n\
    layout (location = 0) in vec3 aPos;\
    layout (location = 1) in vec2 aTexCoords;\
    out vec2 TexCoords;\
    void main()\
    {\
      TexCoords = aTexCoords;\
      gl_Position = vec4(aPos, 1.0);\
    }";
const char *fragmentShaderSource = "#version 330 core\n"
    "in vec2 TexCoords;\n"
    "out vec4 FragColor;\n"
    //"float viewportRatioSq = 800.0f * 800 / 600 / 600;\n"
    //"float viewportRatioSq = 600.0f * 600 / 800 / 800;\n"
    "uniform float viewportRatioSq;\n"

    "vec4 greenColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);\n"
    "vec4 redColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "vec4 yellowColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
    "vec4 blueColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n"

    "vec2 center1 = ivec2(60, 60);\n"
    "float radius1 = 50;\n"
    "vec2 center2 = vec2(0.1, 0.3);\n"
    "float radius2 = 0.05;\n"
    "vec2 center3 = vec2(0.21, 0.3);\n"
    "float radius3 = 0.05;\n"
    "vec2 center4 = vec2(0.31, 0.3);\n"
    "float radius4 = 0.05;\n"

    "void main()\n"
    "{\n"
    // opt1, use the gl_FragCoord to draw the circle 1.
    "  float dist1 = distance(gl_FragCoord.xy, center1);\n"
    // opt2, use the TexCoords to draw the circle 2 (actually oval if viewportW != viewportH).
    "  vec2 shiftedTexCoords2 = TexCoords - center2;\n"
    "  float dist2 = distance(shiftedTexCoords2, vec2(0.0f, 0.0f));\n" // distance to the circle 2 center.
    // opt3, fix the oval of opt2.
    "  vec2 shiftedTexCoords3 = TexCoords - center3;\n"
    "  float distSq3 = shiftedTexCoords3.x * shiftedTexCoords3.x + shiftedTexCoords3.y * shiftedTexCoords3.y * viewportRatioSq;\n"
    // opt4, optimize based on the opt3, and let the circle to be smoother.
    "  vec2 shiftedTexCoords4 = TexCoords - center4;\n"
    "  float distSq4 = shiftedTexCoords4.x * shiftedTexCoords4.x + shiftedTexCoords4.y * shiftedTexCoords4.y * viewportRatioSq;\n"
    "  float blurLen = 0.01;"
    // d4=0 if inside of blurLen, d4=1 if outside of radius4.
    "  float d4 = smoothstep(radius4 - blurLen, radius4, sqrt(distSq4));\n"

    // draw circle 1 2 3 4...
    "  if (dist1 < radius1) {\n"
    "    FragColor = greenColor;\n" // green circle 1
    "  } else if (dist2 < radius2) {\n"
    "    FragColor = yellowColor;\n" // yellow curcle 2
    "  } else if (distSq3 < radius3 * radius3) {\n"
    "    FragColor = redColor;\n" // red curcle 3
    "  } else if (distSq4 < radius4 * radius4) {\n"
    "    FragColor = mix(redColor, blueColor, d4);\n" // mix the red curcle 4 and blue background.
    "  } else {\n"
    "    FragColor = blueColor;\n" // blue background
    "  }"
    "}\n\0";

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

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);
    // check for linking errors
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    // add a new set of vertices to form a second triangle (a total of 6 vertices); the vertex attribute configuration remains the same (still one 3-float position vector per vertex)
    float vertices[] = {
        // first triangle
        // pos               // tex coords
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // left bottom
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // right bottom
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, // left top
        // second triangle
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, // left top
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // right bottom
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f, // right top
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shader);
        float viewportRatioSq = (float)SCR_HEIGHT * SCR_HEIGHT / SCR_WIDTH / SCR_WIDTH;
        glUniform1f(glGetUniformLocation(shader, "viewportRatioSq"), viewportRatioSq);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 6); // set the count to 6 since we're drawing 6 vertices now (2 triangles); not 3!
        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shader);

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
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);

    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}