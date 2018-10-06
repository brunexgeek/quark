#ifndef QUARK_OPENGL_RENDERER_HH
#define QUARK_OPENGL_RENDERER_HH


#include <string>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <engine/Texture.hh>
//#include <glm/glm.hpp>

#include <engine/Mesh.hh>
#include <engine/Light.hh>
#include <engine/Camera.hh>
#include <engine/Renderer.hh>
#include <engine/Transform.hh>

struct SDL_Rect;
struct SDL_Window;
struct SDL_Surface;
struct SDL_Renderer;

namespace quark {
namespace opengl {

class Renderer : public quark::Renderer
{
	public:
		Renderer(
			quark::Camera &camera,
			quark::Light &light,
			uint32_t width,
			uint32_t height,
			uint32_t frameRate = 60 );

		~Renderer();

		uint32_t compileShaders(
			const std::string &vertexShader,
			const std::string &fragmentShader );

		void refresh();

		void draw(
			const quark::Mesh &mesh,
			const quark::Texture &texture,
			const Transform &transform );

		void draw(
			const quark::Mesh &mesh,
			const Transform &transform );

		static uint32_t loadShaders(
			const std::string &vertex_file_path,
			const std::string &fragment_file_path);

		void drawLine(
			float from0,
			float from1,
			float from2,
			float to0,
			float to1,
			float to2,
			float r,
			float g,
			float b  );

		void prepare();

		void setCamera(
			const Camera &position );

		void setActiveShader(
			const ShaderProgram &program );

		Camera &getCamera();

		void drawGrid();

	private:
		SDL_Window *window;
		void *context;
		uint32_t vertexArrayID;
		uint32_t lastDraw;

		ShaderProgram *gridProgram;
		//Texture *gridTexture;
		//std::vector<Vector3f> gridLines;
		uint32_t gridHandle;
		uint32_t gridPoints;

		// shader uniforms
		int32_t textureId;
		int32_t gId;
		int32_t mId;
		int32_t vId;
		int32_t lightId;

		void draw(
			const quark::Mesh &mesh,
			const quark::Texture *texture,
			const Transform &transform );

		void prepareGrid();

		//void updateCamera();
};


}}

#endif // QUARK_OPENGL_RENDERER_HH
