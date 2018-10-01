#ifndef ENGINE_SHADER__HH
#define ENGINE_SHADER__HH


#include <stdint.h>
#include <iostream>
#include <string>


enum class ShaderType
{
	SHADER_VERTEX,
	SHADER_FRAGMENT
};

// TODO: create two specializations for colored and textured shaders
class Shader
{
	public:
		Shader(
			ShaderType type,
			const std::string &code );

		Shader(
			ShaderType type,
			std::istream &in );

		~Shader();

		uint32_t getId() const;

	protected:
		uint32_t shaderId;

		void compileShader(
			ShaderType type,
			const std::string &code );
};


class ShaderProgram
{
	public:
		ShaderProgram(
			const Shader &vertex,
			const Shader &fragment );

		~ShaderProgram();

		uint32_t getId() const;

	protected:
		uint32_t programId;
};


#endif // ENGINE_SHADER__HH
