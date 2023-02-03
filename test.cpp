#define STB_IMAGE_IMPLEMENTATION
#include "render.h"
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shaders.h"
int main()
{

	gl::Okno okno(1600, 900);
	gl::Barva b(0x000000ff);
	gl::Barva ba(0x0000ffff);

	float tocke[12] =
		{
			0.5, 0.5, 0.0,
			0.5, -0.5, 0.0,
			-0.5, -0.5, 0.0,
			-0.5, 0.5, 0.0};

	gl::GameObject kvadrat(tocke, ba);

	while (!okno.aliMoramZapreti())
	{

		okno.BeriInput();
		okno.pobarvaj(b);
		float manip = sin((float)glfwGetTime());
		kvadrat.premakniMe(gl::Vec3<float>(manip, 0, 0));
		kvadrat.obrniMe(gl::Vec3<bool>(0, 0, 1), manip * 360);
		kvadrat.spremeniMiVelikost(gl::Vec3<float>(manip, manip, 0));
		kvadrat.narisiMe();
		okno.osvezi();
	}
}