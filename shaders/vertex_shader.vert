#version 400

uniform mat4 P;
uniform mat4 R;

in vec4 in_Position;

out vec3 in_color;

void main()
{
	vec4 trans = vec4(0.0f, 0.0f, -10000.0f, 0.0f);
	gl_Position = P * (vec4((in_Position.xyz), 1.0) * R + trans);
	gl_PointSize = 2.0;
	in_color = in_Position.xyz;
}  
