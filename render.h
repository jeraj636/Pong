#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "ostalo.h"
#include <iostream>
namespace GL
{
	namespace Barve
	{

	class Barva
	{
		public:
		float r,g,b,a;
		Barva(int hexCode)
		{
			a=hexCode&0x000000ff;
			hexCode>>=8;
			b=hexCode&0x0000ff;
			hexCode>>=8;
			g=hexCode&0x00ff;
			hexCode>>=8;
			r=hexCode&0xff;
			a/=0xff;
			r/=0xff;
			g/=0xff;
			b/=0xff;
		}
	};
	}
	class Okno
	{
	public:
		Okno(int sirina,int visina)
		{
			if (!glfwInit())
			io::izpis("ni glfw-ja",io::type::error);
		io::izpis("glfw deluje",io::type::msg);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			okno = glfwCreateWindow(sirina, visina, "Pong", NULL, NULL);
			if (okno == NULL)
			{
			io::izpis("ni okna",io::type::error);
				glfwTerminate();
			}
		io::izpis("okno deluje",io::type::msg);
			glfwMakeContextCurrent(okno);

			if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
			io::izpis("ni glada",io::type::error);
		io::izpis("glad deljue",io::type::msg);
			glfwSetFramebufferSizeCallback(okno, frameBufferSizeCallBack);
		io::izpis("vse dela",io::type::msg);
		}
		bool aliMoramZapreti()
		{
			return glfwWindowShouldClose(okno);
		}
		void pobarvaj(Barve::Barva barva)
		{
			glClearColor(barva.r,barva.g,barva.b,barva.a);
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
		void BeriInput()
		{
			if(glfwGetKey(okno,GLFW_KEY_ESCAPE)==GLFW_PRESS)
			{
		io::izpis("KONEC PROGRAMA!",io::type::msg);
			glfwSetWindowShouldClose(okno,true);
			}
		}
	private:
		GLFWwindow *okno;
		static void frameBufferSizeCallBack(GLFWwindow *window, int width, int height)
		{
			glViewport(0, 0, width, height);
		}
	};
}