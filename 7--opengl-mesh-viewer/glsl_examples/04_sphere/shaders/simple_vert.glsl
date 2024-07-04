#version 140

// input vertex attributes
in vec3 position;
in vec3 color;

// uniform attributes
uniform mat4 mv_matrix;
uniform mat4 norm_matrix;
uniform mat4 proj_matrix;

// output attributes
out vec3 vertex_position;
out vec4 vertex_color;

void main(void)
{
  gl_Position = proj_matrix * mv_matrix * vec4(position, 1);
  vertex_color = vec4(color, 1);
}
