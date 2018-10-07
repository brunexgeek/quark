#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <engine/opengl/Renderer.hh>
#include <engine/Camera.hh>
#include <engine/Material.hh>
#include <engine/Shader.hh>
#include <engine/Timer.hh>

#include <engine/opengl/Mesh.hh>
#include <engine/opengl/Texture.hh>
#include <engine/opengl/Shader.hh>


#define SEND_TRANSPOSED GL_TRUE

#define COLOR_VERTEX_SHADER \
	"#version 330 core\n" \
	"layout(location = 0) in vec3 VertexPositionWS;\n" \
	"void main(){gl_Position = vec4(VertexPositionWS, 1);}"

#define COLOR_FRAGMENT_SHADER \
	"#version 330 core\n" \
	"out vec3 color;\n" \
	"void main(){color = vec3(1,1,1);}"


using std::string;
using std::ostream;

namespace quark {
namespace opengl {


Renderer::Renderer(
	Camera &camera,
	Light &light,
	uint32_t width,
	uint32_t height,
	uint32_t frameRate ) : quark::Renderer(camera, light, width, height, frameRate),
		window(nullptr), gridProgram(nullptr)
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
	glClearColor(.5, .0, .5, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// enables face culling
	//glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	// resources used by grid rendering
	prepareGrid();
}


Renderer::~Renderer()
{
	glDeleteVertexArrays(1, (GLuint*) &vertexArrayID);
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}


void Renderer::refresh()
{
	SDL_GL_SwapWindow(window);
	glFinish();

	quark::Renderer::refresh();
	/*uint64_t delta = Timer::getTime() - lastDraw;
	//std::cout << "Frame time: " << delta << std::endl;
	if(delta < frameTime)
		SDL_Delay( (uint32_t) (frameTime - delta) );
	lastDraw = (uint32_t) Timer::getTime();;*/
}


void Renderer::draw(
	const quark::Mesh &mesh,
	const quark::Texture &texture,
	const Transform &transform )
{
	draw(mesh, &texture, transform);
}


void Renderer::draw(
	const quark::Mesh &mesh,
	const Transform &transform )
{
	draw(mesh, nullptr, transform);
}


void Renderer::draw(
	const quark::Mesh &mesh,
	const quark::Texture *texture,
	const Transform &transform )
{
	uint32_t vh = ((quark::opengl::Mesh&) mesh).getVertexHandle();
	uint32_t nh = ((quark::opengl::Mesh&) mesh).getNormalHandle();
	uint32_t uvh = ((quark::opengl::Mesh&) mesh).getUvHandle();
	uint32_t fh = ((quark::opengl::Mesh&) mesh).getFaceHandle();

	// attribute buffer 0: vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vh);
	glVertexAttribPointer(
		0,                  // attribute index
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		NULL                // array buffer offset
	);

	// attribute buffer 1: normals
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, nh);
	glVertexAttribPointer(
		1,                                // attribute index
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		NULL                              // array buffer offset
	);

	// attribute buffer 2: UV
	if (texture != nullptr)
	{
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, uvh);
		glVertexAttribPointer(
			2,                  // attribute index
			2,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			NULL                // array buffer offset
		);
		// activates the mesh texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ((quark::opengl::Texture*)texture)->getHandler());
	}

	glUniform1i(textureId, 0);
	// creates the model matrix
	glUniformMatrix4fv(mId, 1, SEND_TRANSPOSED, (const GLfloat*) transform.getMatrix().data);

#if 0
	// draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, mesh.getVertexCount());
#else
	// attribute buffer 1: UV or colors
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fh);
	glDrawElements(
		GL_TRIANGLES,      // mode
		mesh.getFaceCount() * 3,    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
 	);
#endif
	if (texture != nullptr)
	{
		glDisableVertexAttribArray(2);
	}
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

/*
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
	setActiveShader(*gridProgram);

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
}*/


void Renderer::prepareGrid()
{
	// resources used by grind rendering
	//gridProgram = new ShaderProgram(COLOR_VERTEX_SHADER, COLOR_FRAGMENT_SHADER);
	/*static const size_t textureSide = 128;
	uint8_t *pixels = new uint8_t[textureSide * textureSide]();
	for (int j = 0; j < textureSide; ++j)
    	for (int i = 0; i < textureSide; ++i)
        	pixels[j*textureSide + i] = (i < textureSide / 16 || j < textureSide / 16) ? 255 : 0;
	gridTexture = new Texture(128, 128, pixels);
	delete[] pixels;*/

	// grid lines
	static const size_t LINES = 11; // keep this odd
	static const float SQUARE_SIZE = 1.0F;
	gridPoints = LINES * 2 * 2; // 2 points per line, 2 axis
	Vector3f *gridLines = new Vector3f[gridPoints];

	size_t p = 0;
	float x = float(LINES / 2) * -SQUARE_SIZE;
	// X axis
	while (p < gridPoints / 2)
	{
		gridLines[p++] = Vector3f(x, 0, -(LINES / 2 * SQUARE_SIZE));
		gridLines[p++] = Vector3f(x, 0,  (LINES / 2 * SQUARE_SIZE));
		x += SQUARE_SIZE;
	}
	// Z axis
	float z = float(LINES / 2) * -SQUARE_SIZE;
	while (p < gridPoints)
	{
		gridLines[p++] = Vector3f(-(LINES / 2 * SQUARE_SIZE), 0, z);
		gridLines[p++] = Vector3f( (LINES / 2 * SQUARE_SIZE), 0, z);
		z += SQUARE_SIZE;
	}
	gridPoints = (uint32_t) p;
/*	for (size_t i = 0; i < LINES * 2; i += 2)
	{
		gridLines[i]     = Vector3f(i - SQUARE_SIZE, 0, -SQUARE_SIZE);
		gridLines[i + 1] = Vector3f(i - SQUARE_SIZE, 0,  SQUARE_SIZE);
	}
	for (size_t i = LINES * 2; i < LINES * 4; i += 2)
	{
		gridLines[i]     = Vector3f(-SQUARE_SIZE, 0, i - LINES * 2.0F - SQUARE_SIZE);
		gridLines[i + 1] = Vector3f( SQUARE_SIZE, 0, i - LINES * 2.0F - SQUARE_SIZE);
	}*/
	// upload grid lines to GPU
	glGenBuffers(1, &gridHandle);
	glBindBuffer(GL_ARRAY_BUFFER, gridHandle);
	glBufferData(GL_ARRAY_BUFFER, gridPoints * sizeof(Vector3f), gridLines, GL_STATIC_DRAW);
	delete[] gridLines;
}


void Renderer::drawGrid()
{
	//setActiveShader(*gridProgram);

	// attribute buffer 0: vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, gridHandle);
	glVertexAttribPointer(
		0,                  // attribute index
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		NULL                // array buffer offset
	);

	// attribute buffer 1: UVs
	/*glEnableVertexAttribArray(1);
	glGenBuffers(1, &uvHandle);
	glBindBuffer(GL_ARRAY_BUFFER, uvHandle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,                  // attribute index
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		NULL                // array buffer offset
	);
	// activates the mesh texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gridTexture->getHandler());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);*/

	// Draw the triangle !
	glDrawArrays(GL_LINES, 0, gridPoints);
	glDisableVertexAttribArray(0);
}


void Renderer::setActiveShader(
	const quark::ShaderProgram &program )
{
	quark::Renderer::setActiveShader(program);

	glUseProgram( ((quark::opengl::ShaderProgram&)program).getId());

	textureId  = glGetUniformLocation(((quark::opengl::ShaderProgram&)program).getId(), "textureSample");
	gId        = glGetUniformLocation(((quark::opengl::ShaderProgram&)program).getId(), "G");
	mId        = glGetUniformLocation(((quark::opengl::ShaderProgram&)program).getId(), "M");
	vId        = glGetUniformLocation(((quark::opengl::ShaderProgram&)program).getId(), "V");
	lightId    = glGetUniformLocation(((quark::opengl::ShaderProgram&)program).getId(), "LightPositionWS");
}


void Renderer::prepare()
{
	quark::Renderer::prepare();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//updateCamera();

	//glUseProgram(shaderId);
	const Vector3f &lightPosition = light.getPosition();
	//std::cout << "Light " << &light << " is [x: " << lightPosition.x << ", y: " << lightPosition.y << ", z: " << lightPosition.z << "]" << std::endl;;
	glUniform3f(lightId, lightPosition.x, lightPosition.y, lightPosition.z);

	// Send our transformation to the currently bound shader,
	// in the "MVP" uniform
	glUniformMatrix4fv(gId, 1, SEND_TRANSPOSED, camera_.getGlobalMatrix().data);
	glUniformMatrix4fv(vId, 1, SEND_TRANSPOSED, camera_.getMatrix().data);
}

/*
ostream &operator<<(
	ostream &out,
	const Renderer &renderer )
{
	out << "    OpenGL: " << glGetString(GL_VERSION) << std::endl;
	out << "Resolution: " << renderer.width << 'x' << renderer.height << std::endl;
	return out;
}*/


}}