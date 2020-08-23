in vec2 in_vertices;
in vec2 in_uv;

out vec2 v_uv;

uniform vec2 uniform_anchor;
uniform vec2 uniform_size;
uniform mat4 uniform_model;
uniform mat4 uniform_view;
uniform mat4 uniform_projection;

void main()
{
  vec2 finalpos = (in_vertices * uniform_size);
  vec2 offsetPos = finalpos - vec2(uniform_size.x * uniform_anchor.x, uniform_size.y * uniform_anchor.y);
  gl_Position = uniform_projection * uniform_view * uniform_model * vec4(offsetPos, 0.0, 1.0);
  v_uv = in_uv;
}