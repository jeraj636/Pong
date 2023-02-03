#define STB_IMAGE_IMPLEMENTATION
#include "render.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"
#include "shaders.h"
int main()
{
	int width, height, nrChannels;
	unsigned char *imgData = stbi_load("Images/amongUs.png", &width, &height, &nrChannels, 0);

	GL::Okno okno(1600, 900);
	GL::Barve::Barva b(0x000000ff);

	float tocke[] =
	    {
		0.5, 0.5, 0.0, 1, 1, 1,
		0.5, -0.5, 0.0, 1, 1, 1,
		-0.5, -0.5, 0.0, 1, 1, 1,
		-0.5, 0.5, 0.0, 1, 1, 1};
	unsigned int indeces[] = {
	    0, 1, 3,
	    1, 2, 3};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tocke), tocke, GL_STATIC_DRAW);

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	int success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	char log[512];
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, log);
		for (int i = 0; i < 512; i++)
			std::cout << log[i];
		io::izpis("ni vertex shaderja", io::type::error);
	}
	io::izpis("vertex shader deluje", io::type::msg);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
		io::izpis("ni fragment shaderja", io::type::error);
	io::izpis("fragment shader deluje", io::type::msg);

	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
		io::izpis("ni shader programa", io::type::error);
	io::izpis("shader program deluje", io::type::msg);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GL::Barve::Barva trikotnikBarva(0x0000ffff);
	int premikLok = glGetUniformLocation(shaderProgram, "transform");
	int rotacijaLok = glGetUniformLocation(shaderProgram, "rotacija");
	int povecavaLok = glGetUniformLocation(shaderProgram, "povecava");

	glm::mat4 premik = glm::mat4(1);
	glm::mat4 rotacija = glm::mat4(1);
	glm::mat4 povecava = glm::mat4(1);
	float prejsniCas = glfwGetTime();
	int FPS = 0;
	while (!okno.aliMoramZapreti())
	{

		okno.BeriInput();
		okno.pobarvaj(b);
		glUseProgram(shaderProgram);

		premik = glm::mat4(1);
		rotacija = glm::mat4(1);
		povecava = glm::mat4(1);
		if (glfwGetTime() - prejsniCas >= 1)
		{
			std::cout << FPS << '\n';
			prejsniCas = glfwGetTime();
			FPS = 0;
		}
		FPS++;
		rotacija = glm::rotate(rotacija, glm::radians((float)sin(glfwGetTime()) * 360), glm::vec3(0.0, 0.0, 1.0));
		premik = glm::translate(premik, glm::vec3(sin(glfwGetTime()), 0, 0));
		povecava = glm::scale(povecava, glm::vec3(sin(glfwGetTime()), sin(glfwGetTime()), 0));

		glUniformMatrix4fv(premikLok, 1, GL_FALSE, glm::value_ptr(premik));
		glUniformMatrix4fv(rotacijaLok, 1, GL_FALSE, glm::value_ptr(rotacija));
		glUniformMatrix4fv(povecavaLok, 1, GL_FALSE, glm::value_ptr(povecava));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		okno.osvezi();
	}
}