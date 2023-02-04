#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "ostalo.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
namespace gl
{
	template <typename t>
	class Vec3
	{
	public:
		t x, y, z;
		Vec3(t _x, t _y, t _z)
		{
			x = _x;
			z = _z;
			y = _y;
		}
	};
	class Barva
	{
	public:
		float r, g, b, a;
		Barva(int hexCode)
		{
			a = hexCode & 0x000000ff;
			hexCode >>= 8;
			b = hexCode & 0x0000ff;
			hexCode >>= 8;
			g = hexCode & 0x00ff;
			hexCode >>= 8;
			r = hexCode & 0xff;
			a /= 0xff;
			r /= 0xff;
			g /= 0xff;
			b /= 0xff;
		}
	};
	class Okno
	{
	public:
		Okno(int sirina, int visina)
		{
			if (!glfwInit())
				io::izpis("ni glfw-ja", io::type::error);
			io::izpis("glfw deluje", io::type::msg);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			okno = glfwCreateWindow(sirina, visina, "Pong", NULL, NULL);
			if (okno == NULL)
			{
				io::izpis("ni okna", io::type::error);
				glfwTerminate();
			}
			io::izpis("okno deluje", io::type::msg);
			glfwMakeContextCurrent(okno);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
				io::izpis("ni glada", io::type::error);
			io::izpis("glad deljue", io::type::msg);
			glfwSetFramebufferSizeCallback(okno, frameBufferSizeCallBack);
			io::izpis("vse dela", io::type::msg);
		}
		bool aliMoramZapreti()
		{
			return glfwWindowShouldClose(okno);
		}
		void pobarvaj(Barva barva)
		{
			glClearColor(barva.r, barva.g, barva.b, barva.a);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		void osvezi()
		{
			glfwSwapBuffers(okno);
			glfwPollEvents();
		}
		~Okno()
		{
			glfwTerminate();
		}
		int BeriInput()
		{
			if (glfwGetKey(okno, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			{
				io::izpis("KONEC PROGRAMA!", io::type::msg);
				glfwSetWindowShouldClose(okno, true);
			}
			if (glfwGetKey(okno, GLFW_KEY_UP))
				return 1;
			if (glfwGetKey(okno, GLFW_KEY_DOWN))
				return 2;
			return 0;
		}

	private:
		GLFWwindow *okno;
		static void frameBufferSizeCallBack(GLFWwindow *window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
	};

	class GameObject
	{
	public:
		GameObject(float tockePoz[12], Barva barva)
			: _barvaObjekta(0xffffff)
		{
			for (int i = 0; i < 24; i += 6)
			{
				for (int j = 0; j < 3; j++)
				{
					_tocke[i + j] = tockePoz[i / 2 + j];
				}
				_tocke[i + 3] = barva.r;
				_tocke[i + 4] = barva.g;
				_tocke[i + 5] = barva.b;
			}

			glGenVertexArrays(1, &_VAO);
			glBindVertexArray(_VAO);

			glGenBuffers(1, &_EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indeksi), _indeksi, GL_DYNAMIC_DRAW);

			glGenBuffers(1, &_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, _VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(_tocke), _tocke, GL_DYNAMIC_DRAW);
			int succes;
			_vertexShader = glCreateShader(GL_VERTEX_SHADER);
			glShaderSource(_vertexShader, 1, &_vertexShaderSource, NULL);
			glCompileShader(_vertexShader);

			glGetShaderiv(_vertexShader, GL_COMPILE_STATUS, &succes);
			char log[512];
			if (!succes)
			{
				glGetShaderInfoLog(_vertexShader, 512, NULL, log);
				for (int i = 0; i < 512; i++)
					std::cout << log[i];
				io::izpis("ni vertex shaderja", io::type::error);
			}
			io::izpis("vertex shader deluje", io::type::msg);

			_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			glShaderSource(_fragmentShader, 1, &_fragmentShaderSource, NULL);
			glCompileShader(_fragmentShader);
			glGetShaderiv(_fragmentShader, GL_COMPILE_STATUS, &succes);

			if (!succes)
				io::izpis("ni fragment shaderja", io::type::error);
			io::izpis("fragment shader deluje", io::type::msg);

			_shaderProgram = glCreateProgram();
			glAttachShader(_shaderProgram, _vertexShader);
			glAttachShader(_shaderProgram, _fragmentShader);
			glLinkProgram(_shaderProgram);
			glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &succes);
			if (!succes)
				io::izpis("ni shader programa", io::type::error);
			io::izpis("shader program deluje", io::type::msg);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);
			_lokPozicija = glGetUniformLocation(_shaderProgram, "transform");
			_lokRotacija = glGetUniformLocation(_shaderProgram, "rotacija");
			_lokVelikost = glGetUniformLocation(_shaderProgram, "povecava");
			_vleikost = glm::mat4(1);
			_rotacija = glm::mat4(1);
			_pozicija = glm::mat4(1);
		}
		void narisiMe()
		{
			glUniformMatrix4fv(_lokPozicija, 1, GL_FALSE, glm::value_ptr(_pozicija));
			glUniformMatrix4fv(_lokRotacija, 1, GL_FALSE, glm::value_ptr(_rotacija));
			glUniformMatrix4fv(_lokVelikost, 1, GL_FALSE, glm::value_ptr(_vleikost));
			glUseProgram(_shaderProgram);
			glBindVertexArray(_VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			_vleikost = glm::mat4(1);
			_rotacija = glm::mat4(1);
			_pozicija = glm::mat4(1);
		}
		void premakniMe(Vec3<float> KateraOs)
		{
			_pozicija = glm::mat4(1);
			_pozicija = glm::translate(_pozicija, glm::vec3(KateraOs.x, KateraOs.y, KateraOs.z));
		}
		void spremeniMiVelikost(Vec3<float> KateraOs)
		{
			_vleikost = glm::mat4(1);
			_vleikost = glm::scale(_vleikost, glm::vec3(KateraOs.x, KateraOs.y, KateraOs.z));
		}
		void obrniMe(Vec3<bool> KateraOs, float kot)
		{
			_rotacija = glm::mat4(1);
			_rotacija = glm::rotate(_rotacija, glm::radians(kot), glm::vec3((float)KateraOs.x, (float)KateraOs.y, (float)KateraOs.z));
		}

	private:
		Barva _barvaObjekta;
		float _tocke[24];
		unsigned int _indeksi[6] = {0, 1, 3, 1, 2, 3};
		unsigned int _VAO, _VBO, _EBO, _vertexShader, _fragmentShader;
		unsigned int _shaderProgram;
		unsigned int _lokRotacija, _lokPozicija, _lokVelikost;
		glm::mat4 _vleikost = glm::mat4(1);
		glm::mat4 _rotacija = glm::mat4(1);
		glm::mat4 _pozicija = glm::mat4(1);
		const char *_vertexShaderSource = R"(
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
		const char *_fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
in vec3 oBarva;
void main()
{
    FragColor =vec4(oBarva,1);
} 
	)";
	};
}