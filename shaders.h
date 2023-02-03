const char *vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 barva;

out vec3 oBarva;
uniform mat4 transform;
uniform mat4 rotacija;
uniform mat4 povecava;
void main()
{
        gl_Position = transform*rotacija*povecava*vec4(aPos, 1.0);
        oBarva = barva;
}
                )";
const char *fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec3 oBarva;
void main()
{
    FragColor =vec4(oBarva,1);
} 
	)";