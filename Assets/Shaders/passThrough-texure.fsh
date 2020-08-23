in vec2 v_uv;

out vec4 out_color;

uniform vec4 uniform_color;
uniform sampler2D uniform_texture;

void main()
{
	out_color = texture2D(uniform_texture, v_uv) * uniform_color;
}