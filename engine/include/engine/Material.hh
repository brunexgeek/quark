#ifndef ENGINE_MATERIAL_HH
#define ENGINE_MATERIAL_HH


#include <stdint.h>


class Material
{
	public:
		Material();

		virtual ~Material();

		virtual uint32_t getId() const = 0;

	protected:
		Material(
			const Material& object );
};


#endif // ENGINE_MATERIAL_HH
