#include <engine/Shader.hh>
#include <engine/Exception.hh>
#include <GL/glew.h>
#include <sstream>


using std::string;
using std::stringstream;


Shader::Shader(
	ShaderType type,
	const string &code )
{
	compileShader(type, code);
}


Shader::Shader(
	ShaderType type,
	std::istream &in )
{
	if(!in.good())
		EXCEPTION(ERR_IO_READ, 0, "Can not read from the given input stream");

	std::string Line = "";
	stringstream ss;
	while(getline(in, Line))
		ss << "\n" + Line;

	compileShader(type, ss.str());
}


Shader::~Shader()
{
	glDeleteShader(shaderId);
}


uint32_t Shader::getId() const
{
	return shaderId;
}


void Shader::compileShader(
	ShaderType type,
	const string &code )
{
	uint32_t shaderType;
	if (type == ShaderType::SHADER_VERTEX)
		shaderType = GL_VERTEX_SHADER;
	else
		shaderType = GL_FRAGMENT_SHADER;

	shaderId = glCreateShader(shaderType);
	if (shaderId == 0) throw 1;

	// compiles the shader source code
	const char * temp = code.c_str();
	glShaderSource(shaderId, 1, &temp, NULL);
	glCompileShader(shaderId);
	// Check Vertex Shader
	int32_t result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if ( (bool) result ) return;
	// at this point the got an error, so we need to try
	// get the error message
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &result);
	if (result > 1)
	{
		char *message = new char[result + 1];
		glGetShaderInfoLog(shaderId, result, NULL, message);
		// TODO: put message into exception
		std::cout << message << std::endl;
	}
	throw 1;
}


ShaderProgram::ShaderProgram(
	const Shader &vertex,
	const Shader &fragment )
{
	programId = glCreateProgram();
	glAttachShader(programId, vertex.getId());
	glAttachShader(programId, fragment.getId());
	glLinkProgram(programId);

	GLint result;
	GLint length;

	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
	if (length > 1)
	{
		char *message = new char[length + 1];
		glGetProgramInfoLog(programId, length, NULL, message);
		std::cerr << "Shader compilation error: " << message << std::endl;
		delete[] message;
	}
	glDetachShader(programId, vertex.getId());
	glDetachShader(programId, fragment.getId());

	if (result == 0) throw 1;
}


ShaderProgram::~ShaderProgram()
{
	glDeleteProgram(programId);
}


uint32_t ShaderProgram::getId() const
{
	return programId;
}

