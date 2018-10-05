#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <engine/Renderer.hh>
#include <engine/Camera.hh>
#include <engine/Material.hh>
#include <engine/Shader.hh>
#include <engine/Timer.hh>


using std::string;
using std::ostream;

namespace quark {

Renderer::Renderer(
	Camera &camera,
	Light &light,
	uint32_t width,
	uint32_t height,
	uint32_t frameRate ) : width(width), height(height), lastDraw(0), program(NULL), camera_(camera), light(light)
{
	frameTime = 1000 / frameRate;
}


Renderer::~Renderer()
{
}


void Renderer::refresh()
{
	/*SDL_GL_SwapWindow(window);
	glFinish();*/

	uint64_t delta = Timer::getTime() - lastDraw;
	//std::cout << "Frame time: " << delta << std::endl;
	if(delta < frameTime)
		SDL_Delay( (uint32_t) (frameTime - delta) );
	lastDraw = (uint32_t) Timer::getTime();;
}


uint32_t Renderer::getWidth() const
{
	return width;
}


uint32_t Renderer::getHeight() const
{
	return height;
}


void Renderer::setActiveShader(
	const quark::ShaderProgram &program )
{
	this->program = &program;
}


void Renderer::prepare()
{
}


Camera &Renderer::getCamera()
{
	return camera_;
}


}