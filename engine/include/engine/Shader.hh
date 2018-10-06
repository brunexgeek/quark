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
		virtual ~Shader();

		ShaderType getType() const;

	protected:
		ShaderType type_;

		Shader(
			ShaderType type );
};


class ShaderProgram
{
	public:
		virtual ~ShaderProgram();

	protected:
		ShaderProgram();
};

}

#endif // QUARK_SHADER_HH
