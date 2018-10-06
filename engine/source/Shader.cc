#include <engine/Shader.hh>
#include <engine/Exception.hh>
#include <GL/glew.h>
#include <sstream>


using std::string;
using std::stringstream;


namespace quark {


Shader::Shader(
	ShaderType type ) : type_(type)
{
}


Shader::~Shader()
{
}


ShaderType Shader::getType() const
{
	return type_;
}


ShaderProgram::ShaderProgram()
{
}


ShaderProgram::~ShaderProgram()
{
}


}