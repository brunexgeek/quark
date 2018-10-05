#include <engine/Material.hh>


namespace quark {


BasicMaterial::BasicMaterial(
	Shader &shader,
	const Texture &texture ) : Material(shader)
{
	texture_ = &texture;
	weights_ = Vector2f(1.0F, 0.0F).normalize();
}

BasicMaterial::BasicMaterial(
	Shader &shader,
	const Vector3f &color ) : Material(shader)
{
	color_ = color;
	texture_ = nullptr;
	weights_ = Vector2f(0.0F, 1.0F).normalize();
}

BasicMaterial::BasicMaterial(
	Shader &shader,
	const Texture &texture,
	float textureWeight,
	const Vector3f &color,
	float colorWeight ) : Material(shader)
{
	texture_ = &texture;
	color_   = color;
	weights_ = Vector2f(textureWeight, colorWeight).normalize();
}


BasicMaterial::BasicMaterial(
	const BasicMaterial& object ) : Material(object.shader_)
{
	texture_ = object.texture_;
	color_   = object.color_;
	weights_ = object.weights_;
}


BasicMaterial::~BasicMaterial()
{
	// nothing to do
}


} // namespace quark