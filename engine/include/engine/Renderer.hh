/*
 * Renderer.hh
 *
 *  Created on: Jan 30, 2016
 *      Author: bruno
 */

#ifndef QUARK_RENDERER_HH
#define QUARK_RENDERER_HH


#include <string>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <engine/Texture.hh>
//#include <glm/glm.hpp>

#include <engine/Mesh.hh>
#include <engine/Light.hh>
#include <engine/Camera.hh>
#include <engine/Transform.hh>

using quark::Mesh;
using quark::Texture;

struct SDL_Rect;
struct SDL_Window;
struct SDL_Surface;
struct SDL_Renderer;


namespace quark {


class ShaderProgram;
class Camera;


class Renderer
{
	public:
		Renderer(
			Camera &camera,
			Light &light,
			uint32_t width,
			uint32_t height,
			uint32_t frameRate );

		~Renderer();

		/*uint32_t compileShaders(
			const std::string &vertexShader,
			const std::string &fragmentShader );*/

		virtual void refresh();

		virtual void draw(
			const Mesh &mesh,
			const Texture &texture,
			const Transform &transform ) = 0;

		virtual void draw(
			const Mesh &mesh,
			const Transform &transform ) = 0;

		uint32_t getWidth() const;

		uint32_t getHeight() const;

		virtual void prepare();

		void setCamera(
			const Camera &position );

		void setActiveShader(
			const ShaderProgram &program );

		Camera &getCamera();

		virtual void drawGrid() = 0;

	protected:
		uint32_t width;
		uint32_t height;
		uint64_t frameTime;
		void *context;
		uint32_t lastDraw;
		const quark::ShaderProgram *program;
		Camera &camera_;
		Light &light;
};


}

#endif // QUARK_RENDERER_HH

