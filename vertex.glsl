#version 330 core
layout (location = 0) in vec3 aPos;
layout (location=1)in vec3 barva;
out vec3 oBarva;
uniform mat4 transform;
uniform mat4 rotacija;
void main()
{
    gl_Position = transform*rotacija vec4(aPos,1.0);
oBarva=barva;
}
	