/*
 * Renderer.cc
 *
 *  Created on: Jan 30, 2016
 *      Author: bruno
 */

#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include <engine/Renderer.hh>
#include <engine/Camera.hh>
#include <engine/Material.hh>
#include <engine/Shader.hh>
#include <engine/Timer.hh>


using std::string;
using std::ostream;


Renderer::Renderer(
	Camera &camera,
	Light &light,
	uint32_t width,
	uint32_t height,
	uint32_t frameRate ) : width(width), height(height), window(NULL),
		lastDraw(0), program(NULL), camera_(camera), light(light)
{
	// creates the SDL window
	window = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_OPENGL);
	if (window == NULL) throw 1;

	// initializes the OpenGL context
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	context = SDL_GL_CreateContext(window);
	if (context == NULL) throw 1;
	SDL_GL_SetSwapInterval(1);
	// initializes the OpenGL Extension Wrangler
	glewExperimental = GL_TRUE;
	if (glewInit() != 0) throw 1;
	// create the default vertex array
	glGenVertexArrays(1, (GLuint*) &vertexArrayID);
	glBindVertexArray(vertexArrayID);
	// enable depth test
	glEnable(GL_DEPTH_TEST);
	// accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);
	// set the clear color and blank the screen
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// enables face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	//projMatrix = glm::perspective(66.0f, ((float)width / (float)height), 0.1f, 100.0f);

	/*const float BIAS_H = 9.0f;
	const float BIAS_V = BIAS_H * ( (float) height / (float) width );
	projMatrix = glm::ortho(-BIAS_H, BIAS_H, (-BIAS_V)-2, BIAS_V-2, 0.01f, 100.0f);*/
	//updateCamera();

	frameTime = 1000 / frameRate;
}


Renderer::~Renderer()
{
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}


void Renderer::refresh()
{
	SDL_GL_SwapWindow(window);
	glFinish();

	uint64_t delta = Timer::getTime() - lastDraw;
	//std::cout << "Frame time: " << delta << std::endl;
	if(delta < frameTime)
		SDL_Delay( frameTime - delta );
	lastDraw = Timer::getTime();;
}


/*uint32_t Renderer::getElapsedTime() const
{
	return Renderer::getTimer() - lastRender;
}*/


uint32_t Renderer::getWidth() const
{
	return width;
}


uint32_t Renderer::getHeight() const
{
	return height;
}


void Renderer::draw(
	const Mesh &mesh,
	//const Material &material,
	const Transform &transform )
{
	uint32_t vertexId = mesh.getVertexId();
	uint32_t normalId = mesh.getNormalId();
	uint32_t colorId  = mesh.getColorId();

	// attribute buffer 0: vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vertexId);
	glVertexAttribPointer(
		0,                  // attribute index
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		NULL                // array buffer offset
	);

	// attribute buffer 1: UV or colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colorId);
	glVertexAttribPointer(
		1,                  // attribute index
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		NULL                // array buffer offset
	);

	// attribute buffer 2: normals
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, normalId);
	glVertexAttribPointer(
		2,                                // attribute index
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		NULL                              // array buffer offset
	);

	// activates the mesh texture
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, texture.getTexture());
	glUniform1i(textureId, 0);
	// creates the model matrix
	/*glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix[3][0] = position.x;
	modelMatrix[3][1] = position.y;
	modelMatrix[3][2] = position.z;
	#define __90DEGREES ( 90 * 3.1415 ) / 180
	modelMatrix[1][1] = std::cos(__90DEGREES);
	modelMatrix[1][2] = -std::sin(__90DEGREES);
	modelMatrix[2][1] = std::sin(__90DEGREES);
	modelMatrix[2][2] = std::cos(__90DEGREES);*/
	glUniformMatrix4fv(mId, 1, GL_FALSE, (const GLfloat*) transform.getMatrix().data);

	// draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}


void Renderer::drawLine(
	float from0,
	float from1,
	float from2,
	float to0,
	float to1,
	float to2,
	float r,
	float g,
	float b )
{
	GLuint vHandle, cHandle, nHandle;

	float vertices[] =
	{
		from0, from1, from2,
		to0,   to1,   to2,
	};
	float colors[] =
	{
		r, g, b,
		r, g, b
	};
	float normals[] =
	{
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};
	// attribute buffer 0: vertices
	glEnableVertexAttribArray(0);
	glGenBuffers(1, &vHandle);
	glBindBuffer(GL_ARRAY_BUFFER, vHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute index
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		NULL                // array buffer offset
	);

	// attribute buffer 1: colors
	glEnableVertexAttribArray(1);
	glGenBuffers(1, &cHandle);
	glBindBuffer(GL_ARRAY_BUFFER, cHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,                  // attribute index
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		NULL                // array buffer offset
	);

	// attribute buffer 2: normals
	glEnableVertexAttribArray(2);
	glGenBuffers(1, &nHandle);
	glBindBuffer(GL_ARRAY_BUFFER, nHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(
		2,                                // attribute index
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		NULL                              // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_LINES, 0, 2);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDeleteBuffers(1, &vHandle);
	glDeleteBuffers(1, &cHandle);
	glDeleteBuffers(1, &nHandle);
}


void Renderer::setActiveShader(
	const ShaderProgram &program )
{
	this->program = &program;
	glUseProgram(program.getId());

	textureId  = glGetUniformLocation(program.getId(), "textureSample");
	gId        = glGetUniformLocation(program.getId(), "G");
	mId        = glGetUniformLocation(program.getId(), "M");
	vId        = glGetUniformLocation(program.getId(), "V");
	lightId    = glGetUniformLocation(program.getId(), "LightPosition_worldspace");
}


void Renderer::prepare()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//updateCamera();

	//glUseProgram(shaderId);
	const Vector3f &lightPosition = light.getPosition();
	//std::cout << "Light " << &light << " is [x: " << lightPosition.x << ", y: " << lightPosition.y << ", z: " << lightPosition.z << "]" << std::endl;;
	glUniform3f(lightId, lightPosition.x, lightPosition.y, lightPosition.z);

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	glUniformMatrix4fv(gId, 1, GL_FALSE, &camera_.getGlobalMatrix()[0][0]);
	glUniformMatrix4fv(vId, 1, GL_FALSE, &camera_.getMatrix()[0][0]);
}


ostream &operator<<(
	ostream &out,
	const Renderer &renderer )
{
	out << "    OpenGL: " << glGetString(GL_VERSION) << std::endl;
	out << "Resolution: " << renderer.width << 'x' << renderer.height << std::endl;
}


Camera &Renderer::getCamera()
{
	return camera_;
}


