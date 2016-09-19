#version 330

uniform mat4 model_matrix;
uniform mat4 projection_matrix;

layout (location = 0) in vec4 position;

void main(void)
{
    gl_Position = projection_matrix * (model_matrix * position);
}
