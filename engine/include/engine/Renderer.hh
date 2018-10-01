/*
 * Renderer.hh
 *
 *  Created on: Jan 30, 2016
 *      Author: bruno
 */

#ifndef ENGINE_RENDERER_HH_
#define ENGINE_RENDERER_HH_


#include <string>
#include <vector>
#include <stdint.h>
#include <iostream>
#include <engine/Texture.hh>
//#include <glm/glm.hpp>

#include <engine/Mesh.hh>
#include <engine/Light.hh>
#include <engine/Transform.hh>


class Sprite;
class Level;
class Scene;
class Material;
class ShaderProgram;
class Camera;

struct SDL_Rect;
struct SDL_Window;
struct SDL_Surface;
struct SDL_Renderer;

class Renderer
{
	public:
		Renderer(
			Camera &camera,
			Light &light,
			uint32_t width,
			uint32_t height,
			uint32_t frameRate = 60 );

		~Renderer();

		uint32_t compileShaders(
			const std::string &vertexShader,
			const std::string &fragmentShader );

		/*void render(
			Sprite &sprite );*/

		/*void draw(
			uint32_t x,
			uint32_t y,
			const Texture2D &texture,
			const SDL_Rect &rect );*/

		SDL_Surface *getSurface();

		void refresh();

		void draw(
			const Mesh &mesh,
			const Texture &texture,
			const Transform &transform );

		void draw(
			const Mesh &mesh,
			const Transform &transform );

		static uint32_t loadShaders(
			const std::string &vertex_file_path,
			const std::string &fragment_file_path);

		//uint32_t getElapsedTime() const;

		friend std::ostream &operator<<(
			std::ostream &out,
			const Renderer &renderer );

		uint32_t getWidth() const;

		uint32_t getHeight() const;

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

		/*Texture2D *createTexture(
			SDL_Surface *image );*/

		void prepare();

		void setCamera(
			const Camera &position );

		void setActiveShader(
			const ShaderProgram &program );

		Camera &getCamera();

	private:
		uint32_t width;
		uint32_t height;
		SDL_Window *window;
		uint64_t frameTime;
		void *context;
		uint32_t vertexArrayID;
		uint32_t lastDraw;
		const ShaderProgram *program;
		Camera &camera_;

		// shader uniforms
		int32_t textureId;
		int32_t gId;
		int32_t mId;
		int32_t vId;
		int32_t lightId;

		//glm::mat4 projMatrix;
		//glm::mat4 cameraMatrix;
		//glm::mat4 globalMatrix;

		Light &light;

		void draw(
			const Mesh &mesh,
			const Texture *texture,
			const Transform &transform );

		//void updateCamera();
};


std::ostream &operator<<(
	std::ostream &out,
	const Renderer &renderer );

#endif /* ENGINE_RENDERER_HH_ */
