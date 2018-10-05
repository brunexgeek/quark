#ifndef QUARK_SHADER_HH
#define QUARK_SHADER_HH

#include <stdint.h>
#include <iostream>
#include <string>


namespace quark {

enum class ShaderType
{
	SHADER_VERTEX,
	SHADER_FRAGMENT
};


class Shader
{
	public:
		Shader(
			ShaderType type,
			const std::string &code );

		Shader(
			ShaderType type,
			std::istream &in );

		virtual ~Shader();

		ShaderType getType() const;

	protected:
		ShaderType type_;
};


class ShaderProgram
{
	public:
		ShaderProgram(
			const Shader &vertex,
			const Shader &fragment );

		virtual ~ShaderProgram();
};

}

#endif // QUARK_SHADER_HH
