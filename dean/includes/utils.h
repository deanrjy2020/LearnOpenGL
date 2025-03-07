#ifndef UTILS_HPP
#define UTILS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>

// The existing code in the shader.h is to read the shader source code from file.
// Here we pass the source code as string instead of file path.
GLuint compileAndCreateProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
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
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    // check for linking errors
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

/////////////////////////////////////////////////////////////////////////////////////
// init all the utils

GLuint axesProgram;
void initAxes() {
    const char *vertexShaderSource = "#version 330 core\n"
    // Input vertex data, different for all executions of this shader.
    "layout(location = 0) in vec3 vertexPosition_modelspace;\n"
    "layout(location = 1) in vec3 vertexColor;\n"
    // Output data ; will be interpolated for each fragment.
    "out vec3 color;\n"
    // Values that stay constant for the whole mesh.
    "uniform mat4 MVP;\n"
    "void main() {\n"
        // Output position of the vertex, in clip space : MVP * position.
    "    gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);\n"
    "    color = vertexColor;\n"
    "}\n";

    const char *fragmentShaderSource = "#version 330 core\n"
    "in vec3 color;\n"
    // Ouput data
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(color, 1.0);\n"
    "}\n";

    axesProgram = compileAndCreateProgram(vertexShaderSource, fragmentShaderSource);
}

GLuint lightCubeProgram;
void initLightCube() {
    const char *vertexShaderSource = "#version 330 core\n"
    "layout(location = 0) in vec3 vertexPosition_modelspace;\n"
    "uniform mat4 MVP;\n"
    "void main() {\n"
    "    gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);\n"
    "}\n";

    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main() {\n"
    "    FragColor = vec4(1.0);\n"
    "}\n";

    lightCubeProgram = compileAndCreateProgram(vertexShaderSource, fragmentShaderSource);
}

GLuint texQuadProgram;
void initTexQuad() {
    const char *vertexShaderSource = "#version 330 core\n"
    "layout(location = 0) in vec3 aPos;\n"
    "layout(location = 1) in vec2 aTexCoords;\n"
    "uniform vec2 flipXY;\n"
    "out vec2 TexCoords;\n"
    "void main() {\n"
    "    TexCoords = aTexCoords;\n"
    "    gl_Position = vec4(aPos, 1.0);\n"
    "    gl_Position = vec4(flipXY * aPos.xy, 0.0, 1.0);\n"
    "}\n";

    const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "in vec2 TexCoords;\n"
    "uniform sampler2D sampler;\n"
    "void main() {\n"
    "    FragColor = texture(sampler, TexCoords);\n"
    "}\n";

    texQuadProgram = compileAndCreateProgram(vertexShaderSource, fragmentShaderSource);

    glUseProgram(texQuadProgram);
    glUniform1i(glGetUniformLocation(texQuadProgram, "sampler"), 0);
}

void initUtils() {
    initAxes();
    initLightCube();
    initTexQuad();
}
/////////////////////////////////////////////////////////////////////////////////////
// Draw the axes

GLuint axesVAO;
void drawAxes(glm::mat4& MVP) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // draw to default fbo
    // do the shader things
    // model matrix is identity
    glUseProgram(axesProgram);
    glUniformMatrix4fv(glGetUniformLocation(axesProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);

    // do the VAO things.
    // initialize (if necessary)
    if (axesVAO == 0) {
        GLfloat axesVertices[] = {
            // positions        // colors
            // +X axis          // R
             0.0f, 0.0f, 0.0f,  1.0f,0.0f,0.0f,
             9.0f, 0.0f, 0.0f,  1.0f,0.0f,0.0f,
            // -X axis          // 0.5R
             0.0f, 0.0f, 0.0f,  0.5f,0.0f,0.0f,
            -9.0f, 0.0f, 0.0f,  0.5f,0.0f,0.0f,
            // +Y axis          // G
             0.0f, 0.0f, 0.0f,  0.0f,1.0f,0.0f,
             0.0f, 9.0f, 0.0f,  0.0f,1.0f,0.0f,
            // -Y axis          // 0.5G
             0.0f, 0.0f, 0.0f,  0.0f,0.5f,0.0f,
             0.0f,-9.0f, 0.0f,  0.0f,0.5f,0.0f,
            // +Z axis          // B
             0.0f, 0.0f, 0.0f,  0.0f,0.0f,1.0f,
             0.0f, 0.0f, 9.0f,  0.0f,0.0f,1.0f,
            // -Z axis          // 0.5B
             0.0f, 0.0f, 0.0f,  0.0f,0.0f,0.5f,
             0.0f, 0.0f,-9.0f,  0.0f,0.0f,0.5f,
        };

        glGenVertexArrays(1, &axesVAO);

        GLuint axesVBO;
        glGenBuffers(1, &axesVBO);
        glBindBuffer(GL_ARRAY_BUFFER, axesVBO);
        // fill buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(axesVertices), axesVertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(axesVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            6 * sizeof(GLfloat),// stride
            (void*)0            // array buffer offset
        );
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    glBindVertexArray(axesVAO);
    glDrawArrays(GL_LINES, 0, 12);
    glBindVertexArray(0);
}

/////////////////////////////////////////////////////////////////////////////////////
// Draw the write light cube

GLuint lightCubeVAO;
void drawLightCube(glm::mat4& VP, glm::vec3& lightPos) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // draw to default fbo
    // do the shader things
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.1f)); // a smaller cube
    glm::mat4 MVP = VP * model;
    glUseProgram(lightCubeProgram);
    glUniformMatrix4fv(glGetUniformLocation(lightCubeProgram, "MVP"), 1, GL_FALSE, &MVP[0][0]);

    // do the VAO things.
    // initialize (if necessary)
    if (lightCubeVAO == 0) {
        GLfloat lightCubeVertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  // bottom-left
             1.0f,  1.0f, -1.0f,  // top-right
             1.0f, -1.0f, -1.0f,  // bottom-right
             1.0f,  1.0f, -1.0f,  // top-right
            -1.0f, -1.0f, -1.0f,  // bottom-left
            -1.0f,  1.0f, -1.0f,  // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  // bottom-left
             1.0f, -1.0f,  1.0f,  // bottom-right
             1.0f,  1.0f,  1.0f,  // top-right
             1.0f,  1.0f,  1.0f,  // top-right
            -1.0f,  1.0f,  1.0f,  // top-left
            -1.0f, -1.0f,  1.0f,  // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f,  // top-right
            -1.0f,  1.0f, -1.0f,  // top-left
            -1.0f, -1.0f, -1.0f,  // bottom-left
            -1.0f, -1.0f, -1.0f,  // bottom-left
            -1.0f, -1.0f,  1.0f,  // bottom-right
            -1.0f,  1.0f,  1.0f,  // top-right
            // right face
             1.0f,  1.0f,  1.0f,  // top-left
             1.0f, -1.0f, -1.0f,  // bottom-right
             1.0f,  1.0f, -1.0f,  // top-right
             1.0f, -1.0f, -1.0f,  // bottom-right
             1.0f,  1.0f,  1.0f,  // top-left
             1.0f, -1.0f,  1.0f,  // bottom-left
            // bottom face
            -1.0f, -1.0f, -1.0f,  // top-right
             1.0f, -1.0f, -1.0f,  // top-left
             1.0f, -1.0f,  1.0f,  // bottom-left
             1.0f, -1.0f,  1.0f,  // bottom-left
            -1.0f, -1.0f,  1.0f,  // bottom-right
            -1.0f, -1.0f, -1.0f,  // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  // top-left
             1.0f,  1.0f , 1.0f,  // bottom-right
             1.0f,  1.0f, -1.0f,  // top-right
             1.0f,  1.0f,  1.0f,  // bottom-right
            -1.0f,  1.0f, -1.0f,  // top-left
            -1.0f,  1.0f,  1.0f,  // bottom-left
        };

        glGenVertexArrays(1, &lightCubeVAO);

        GLuint lightCubeVBO;
        glGenBuffers(1, &lightCubeVBO);
        glBindBuffer(GL_ARRAY_BUFFER, lightCubeVBO);
        // fill buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(lightCubeVertices), lightCubeVertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(lightCubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            3 * sizeof(GLfloat),// stride
            (void*)0            // array buffer offset
        );
        glBindVertexArray(0);
    }

    glBindVertexArray(lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

/////////////////////////////////////////////////////////////////////////////////////
// Draw the small quad on the top-right of the screen.

GLuint texQuadVAO;
void drawTexQuad(GLuint textureID,
                 // full screen if leftBottom = -1
                 float leftBottom = 0.6f,
                 // flip if -1
                 glm::vec2 flipXY = glm::vec2(1.0f, 1.0f)) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // draw to default fbo
    // do the shader things
    glUseProgram(texQuadProgram);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform2fv(glGetUniformLocation(texQuadProgram, "flipXY"), 1, &flipXY[0]);

    // do the VAO things.
    // initialize (if necessary)
    if (texQuadVAO == 0) {
        GLfloat quadVertices[] = {
            // positions                       // texture Coords
            leftBottom,       1.0f, 0.0f,      0.0f, 1.0f,
            leftBottom, leftBottom, 0.0f,      0.0f, 0.0f,
             1.0f,            1.0f, 0.0f,      1.0f, 1.0f,
             1.0f,      leftBottom, 0.0f,      1.0f, 0.0f,
        };

        glGenVertexArrays(1, &texQuadVAO);

        GLuint quadVBO;
        glGenBuffers(1, &quadVBO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        // fill buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(texQuadVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            5 * sizeof(GLfloat),// stride
            (void*)0            // array buffer offset
        );
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
    }

    glBindVertexArray(texQuadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

/////////////////////////////////////////////////////////////////////////////////////
// Matrix utils

void print(const std::string &name, const glm::vec3& vec) {
    printf("%s:\n\
        (%10.6f, %10.6f, %10.6f)\n",
        name.c_str(),
        vec[0], vec[1], vec[2]);
}

void print(const std::string &name, const glm::vec4& vec) {
    printf("%s:\n\
        (%10.6f, %10.6f, %10.6f, %10.6f)\n",
        name.c_str(),
        vec[0], vec[1], vec[2], vec[3]);
}

void print(const std::string &name, const glm::mat4& mat) {
    printf("%s:\n\
        %10.6f, %10.6f, %10.6f, %10.6f,\n\
        %10.6f, %10.6f, %10.6f, %10.6f,\n\
        %10.6f, %10.6f, %10.6f, %10.6f,\n\
        %10.6f, %10.6f, %10.6f, %10.6f,\n",
        name.c_str(),
        mat[0][0], mat[1][0], mat[2][0], mat[3][0],
        mat[0][1], mat[1][1], mat[2][1], mat[3][1],
        mat[0][2], mat[1][2], mat[2][2], mat[3][2],
        mat[0][3], mat[1][3], mat[2][3], mat[3][3]);
}

/////////////////////////////////////////////////////////////////////////////////////
// dump FB

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void dumpFramebuffer(const char* filename, int width, int height)
{
    // Allocate buffer for pixel data (RGBA, 4 channels)
    std::vector<unsigned char> pixels(width * height * 4);

    // Read pixels from framebuffer 0
    glReadBuffer(GL_FRONT);  // GL_BACK if dumping after rendering
    glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    // Flip the image vertically since OpenGL's origin is bottom-left
    for (int y = 0; y < height / 2; ++y)
    {
        for (int x = 0; x < width * 4; ++x)
        {
            std::swap(pixels[y * width * 4 + x], pixels[(height - 1 - y) * width * 4 + x]);
        }
    }

    // Write to PNG file
    stbi_write_png(filename, width, height, 4, pixels.data(), width * 4);
    printf("Framebuffer dumped to %s\n", filename);
}


#endif