#include <engine/Shader.hh>
#include <engine/Exception.hh>
#include <GL/glew.h>
#include <sstream>


using std::string;
using std::stringstream;


namespace quark {


Shader::Shader(
	ShaderType type,
	const string &code ) : type_(type)
{
	(void)code;
}


Shader::Shader(
	ShaderType type,
	std::istream &in ) : type_(type)
{
	(void)in;
}


Shader::~Shader()
{
}


ShaderType Shader::getType() const
{
	return type_;
}


ShaderProgram::ShaderProgram(
	const Shader &vertex,
	const Shader &fragment )
{
	(void)vertex;
	(void)fragment;
}


ShaderProgram::~ShaderProgram()
{
}


}