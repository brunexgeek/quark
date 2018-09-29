/*
 * Object.cc
 *
 *  Created on: Jan 30, 2016
 *      Author: bruno
 */


#include <iostream>
#include <SDL2/SDL.h>

#include <engine/Object.hh>
#include <engine/Mesh.hh>
#include <engine/Renderer.hh>
#include <engine/Timer.hh>


Object::Object(
	const Mesh &mesh ) : lastUpdate(0), mesh(mesh)
{
	position.x = position.y = position.z = 0.0f;
	// TODO: compute dimension using mesh information
	dimension.x = dimension.y = dimension.z = 1.0f;
}


Object::Object(
	const Object &object ) : lastUpdate(0), mesh(object.mesh),
		position(object.position), dimension(object.dimension)
{
	// nothing to do
}


Object::~Object()
{
	// nothing to do
}


void Object::update()
{
	lastUpdate = Timer::getTime();
}


void Object::setPosition(
	const Vector3f &value )
{
	position = value;
}


const Vector3f &Object::getPosition() const
{
	return position;
}


const Vector3f &Object::getDimension() const
{
	return dimension;
}


void Object::draw(
	Renderer &renderer )
{
	renderer.draw(mesh, transform);
}


Transform& Object::getTransform()
{
	return transform;
}


const Transform& Object::getTransform() const
{
	return transform;
}