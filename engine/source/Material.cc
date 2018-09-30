#include <iostream>
#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <engine/Object.hh>
#include <engine/Material.hh>
#include <engine/Renderer.hh>
#include <engine/Exception.hh>


Material::Material()
{
	// nothing to do
}


Material::~Material()
{
	// nothing to do
}


Material::Material(
	const Material& object )
{
	(void) object;
	throw EXCEPTION(ERR_INVALID_ARGUMENT, 0, "Copy constructor can not be invoked");
}

