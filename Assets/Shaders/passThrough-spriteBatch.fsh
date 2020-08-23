in vec2 v_uv;
in vec4 v_color;
out vec4 out_color;

uniform sampler2D uniform_texture;

void main()
{
	out_color = texture2D(uniform_texture, v_uv) * v_color;
}