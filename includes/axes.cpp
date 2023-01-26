#include <axes.h>
#include <learnopengl/shader_m.h>

// This file is only for learning purpose.
GLuint program;
GLuint VAOAxes;

void initAxes() {
	// Need a OpenG context before init shader.
	Shader ourShader("axes_shader.vs", "axes_shader.fs");
	program = ourShader.ID;

	static const GLfloat g_vertex_buffer_data[] = {
		// +X axis
		0.0f,0.0f,0.0f,
		9.0f,0.0f,0.0f,
		// -X axis
		0.0f,0.0f,0.0f,
		-9.0f,0.0f,0.0f,
		// +Y axis
		0.0f,0.0f,0.0f,
		0.0f,9.0f,0.0f,
		//- Y axis
		0.0f,0.0f,0.0f,
		0.0f,-9.0f,0.0f,
		// +Z axis
		0.0f,0.0f,0.0f,
		0.0f,0.0f,9.0f,
		// -Z axis
		0.0f,0.0f,0.0f,
		0.0f,0.0f,-9.0f,
	};
	static const GLfloat g_color_buffer_data[] = {
		// R
		1.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
		// 0.5R
		0.5f,0.0f,0.0f,
		0.5f,0.0f,0.0f,
		// G
		0.0f,1.0f,0.0f,
		0.0f,1.0f,0.0f,
		// 0.5G
		0.0f,0.5f,0.0f,
		0.0f,0.5f,0.0f,
		// B
		0.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,
		// 0.5B
		0.0f,0.0f,0.5f,
		0.0f,0.0f,0.5f,
	};

	glGenVertexArrays(1, &VAOAxes);
	glBindVertexArray(VAOAxes);

	GLuint VBOVertexAxes;
	glGenBuffers(1, &VBOVertexAxes);
	glBindBuffer(GL_ARRAY_BUFFER, VBOVertexAxes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	GLuint VBOColorAxes;
	glGenBuffers(1, &VBOColorAxes);
	glBindBuffer(GL_ARRAY_BUFFER, VBOColorAxes);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	glBindVertexArray(0);
}

void drawAxes(glm::mat4& MVP) {
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, &MVP[0][0]);

	glBindVertexArray(VAOAxes);
	glDrawArrays(GL_LINES, 0, 12);
	glBindVertexArray(0);
}