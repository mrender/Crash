in vec2 in_vertices;
in vec4 in_color;
//in float in_pointSize;

out vec4 v_color;

uniform vec2 uniform_anchor;
uniform vec2 uniform_size;
uniform mat4 uniform_model;
uniform mat4 uniform_view;
uniform mat4 uniform_projection;

void main()
{
  vec2 offsetPos = in_vertices - vec2(uniform_size.x * uniform_anchor.x, uniform_size.y * uniform_anchor.y);
  gl_Position = uniform_projection * uniform_view * uniform_model * vec4(offsetPos, 0.0, 1.0);
  //gl_PointSize = a_pointSize;
  v_color = in_color;
}