#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    vec3 normal;
} gs_in[];

const float MAGNITUDE = 0.2;
vec3 calcNormal = vec3(1.0);

uniform mat4 projection;
uniform bool useCalculatedNormal;

vec3 GetNormal()
{
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(b, a)); // the order is from result.
}

void GenerateLine(int index)
{
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    if (useCalculatedNormal) {
        gl_Position = projection * (gl_in[index].gl_Position + vec4(calcNormal, 0.0) * MAGNITUDE);
        //gl_Position = projection * (gl_in[index].gl_Position + vec4(-gs_in[index].normal, 0.0) * MAGNITUDE);
    } else {
        gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    }
    EmitVertex();
    EndPrimitive();
}

void main()
{
    if (useCalculatedNormal) {
        calcNormal = GetNormal();
    }
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}