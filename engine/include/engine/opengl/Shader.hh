#ifndef QUARK_OPENGL_SHADER__HH
#define QUARK_OPENGL_SHADER__HH


#include <stdint.h>
#include <iostream>
#include <string>

#include <engine/Shader.hh>


namespace quark {
namespace opengl {


class Shader : public quark::Shader
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


class ShaderProgram : public quark::ShaderProgram
{
	public:
		ShaderProgram(
			const Shader &vertex,
			const Shader &fragment );

		ShaderProgram(
			const std::string &vertexCode,
			const std::string &fragmentCode );

		~ShaderProgram();

		uint32_t getId() const;

	protected:
		uint32_t programId;

		void initialize(
			const Shader &vertex,
			const Shader &fragment );
};


}}


#endif // QUARK_OPENGL_SHADER__HH
