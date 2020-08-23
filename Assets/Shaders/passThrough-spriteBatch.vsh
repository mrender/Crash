in vec2 in_vertices;
in vec2 in_uv;
in vec4 in_color;

out vec2 v_uv;
out vec4 v_color;

uniform mat4 uniform_model;
uniform mat4 uniform_view;
uniform mat4 uniform_projection;

void main()
{
  gl_Position = uniform_projection * uniform_view * uniform_model * vec4(in_vertices, 0.0, 1.0);
  v_uv = in_uv;
  v_color = in_color;
}