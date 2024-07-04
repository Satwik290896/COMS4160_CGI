#version 140

in vec4 vertex_color;
out vec4 FragColor;

void main(void)
{
  FragColor = vertex_color;
}
