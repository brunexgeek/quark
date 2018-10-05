#ifndef ENGINE_MATERIAL_HH
#define ENGINE_MATERIAL_HH


#include <type_traits>
#include <stdint.h>
#include <engine/Texture.hh>
#include <engine/Vector.hh>
#include <engine/Shader.hh>

using quark::Texture;
using quark::Shader;

namespace quark {

template <typename S>
class Material
{
	static_assert(std::is_base_of<Shader, S>::value, "S must inherit from Shader");

	public:
		virtual ~Material()        {}
		S& getShader()             { return shader_; }
		const S& getShader() const { return shader_; }

	protected:
		S &shader_;
		Material( S &shader ) : shader_(shader) {}
};


class BasicMaterial : public Material<Shader>
{
	public:
		BasicMaterial(
			Shader &shader,
			const Texture &texture );

		BasicMaterial(
			Shader &shader,
			const Vector3f &color );

		BasicMaterial(
			Shader &shader,
			const Texture &texture,
			float textureWeight,
			const Vector3f &color,
			float colorWeight );

		BasicMaterial(
			const BasicMaterial& object );

		~BasicMaterial();

		Vector3f getColor() const { return color_; }

		const Texture *getTexture() const { return texture_; }

		Vector2f getWeights() const { return weights_; }
		float getColorWeight() const { return weights_.y; }
		float getTextureWeight() const { return weights_.x; }

	protected:
		const Texture *texture_;
		Vector3f color_;
		Vector2f weights_;
};


} // namespace quark

#endif // ENGINE_MATERIAL_HH
